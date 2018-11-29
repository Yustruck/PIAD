#pragma once

HOGDescriptor hog; //Histograma de Gradientes Orientados
Mat aux;
Mat aux2;

float escala = 1.0f;

class faceRememberer
{
public:
	cv::Scalar color;
	int x;
	int y;
	bool wasThisFaceReFound;

	faceRememberer(int X, int Y, int B, int G, int R) {
		x = X;
		y = Y;
		color = cv::Scalar(B, G, R);
		wasThisFaceReFound = true;
	}

	bool isThisCloneEnough(int X, int Y) {

		if (wasThisFaceReFound)
			return false;

		if (X<x + 50 && X>x - 50) {
			if (Y<y + 75 && Y>y-75) {
				wasThisFaceReFound = true;
				x = X;
				y = Y;
				return true;
			}
		}
		return false;
	}
};

vector<faceRememberer*> remFaces;

void removeThisRem1(int id) {
	delete remFaces.at(id);
}

void removeThisRem2(int id) {
	remFaces.erase(remFaces.begin() + id);
}

String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye.xml";

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

Mat rotate(Mat src, double angle)
{
	Mat dst;
	Point2f pt(src.cols / 2., src.rows / 2.);
	Mat r = getRotationMatrix2D(pt, angle, 1.0);
	warpAffine(src, dst, r, cv::Size(src.cols, src.rows));
	return dst;
}

Mat Caras(Mat frame, float escala)
{
	std::vector<cv::Rect> faces;
	Mat frame_gray;
	Mat nuevo(cvRound(frame.rows / escala), cvRound(frame.cols / escala), CV_8UC1);

	cvtColor(frame, frame_gray, CV_BGR2GRAY);

	resize(frame_gray, nuevo, nuevo.size(), 0, 0, INTER_LINEAR);

	equalizeHist(nuevo, nuevo);

	face_cascade.detectMultiScale(nuevo, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

	for (int j = 0; j < remFaces.size(); j++) {
		remFaces[j]->wasThisFaceReFound = false;
	}
	vector<int> thingNotfound;

	for (int i = 0; i < faces.size(); i++)
	{
		faceRememberer *caraActual=NULL;
		for (int j = 0; j < remFaces.size(); j++) {
			if (remFaces[j]->isThisCloneEnough(faces[i].x, faces[i].y)) {
				caraActual = remFaces[j];
			}
		}

		if (!caraActual) {
			caraActual = new faceRememberer(faces[i].x, faces[i].y, rand()%254, rand() % 254, rand() % 254);
			remFaces.push_back(caraActual);
		}

		cv::Point center((faces[i].x + faces[i].width*0.5)*escala, (faces[i].y + faces[i].height*0.5)*escala);
		
		cv::Rect r = faces.at(i);
		r.x *= escala;
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8*escala);
		r.y = r.y*escala;
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8*escala);
		rectangle(frame, r.tl(), r.br(), caraActual->color, 3);

		/*
		Mat faceROI = nuevo(faces[i]);
		std::vector<cv::Rect> eyes;
		//busca los ojos en la cara segun su haarcascade
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
		//de los ojos detectados creara un circulo por cada uno
		for (int j = 0; j < eyes.size(); j++)
		{
			cv::Point center((faces[i].x + eyes[j].x + eyes[j].width*0.5)*escala, (faces[i].y + eyes[j].y + eyes[j].height*0.5)*escala);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25 *escala);
			circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
		}
		*/

	}

	for (int j = 0; j < remFaces.size(); j++) {
		if (!remFaces[j]->wasThisFaceReFound) {
			thingNotfound.push_back(j);
		}
	}

	for (int j = 0; j < thingNotfound.size(); j++) {
		removeThisRem1(thingNotfound[j]);
	}

	for (int j = thingNotfound.size()-1; j >=0; j--) {
		removeThisRem2(thingNotfound[j]);
	}

	thingNotfound.clear();

	wstring caras=L"";

	caras = L"Caras detectadas : " + to_wstring(remFaces.size());

	SendDlgItemMessage(ventanaActual, IDC_FACECOUNT, WM_SETTEXT, 0, (LPARAM)caras.c_str());

	return frame;
}

Mat CuerpoCompleto(Mat frame) {
	resize(frame, aux, aux.size(), 0, 0, INTER_LINEAR);
	cvtColor(aux, aux2, CV_BGR2GRAY);
	if (!frame.data)
		return frame;

	vector<cv::Rect> found, found_filtered;

	hog.detectMultiScale(aux2, found, 0, cv::Size(8, 8), cv::Size(32, 32), 1.05, 2);

	size_t i, j;

	for (i = 0; i < found.size(); i++)
	{
		cv::Rect r = found[i];

		for (j = 0; j < found.size(); j++)
			if (j != i && (r & found[j]) == r)
				break;

		if (j == found.size())
			found_filtered.push_back(r);
	}
	/*
	for (int jF = 0; jF < remFaces.size(); jF++) {
		remFaces[jF]->wasThisFaceReFound = false;
	}
	vector<int> thingNotfound;
	*/

	for (i = 0; i < found_filtered.size(); i++)
	{
		/*
		faceRememberer *caraActual = NULL;
		for (int jF = 0; jF < remFaces.size(); jF++) {
			if (remFaces[jF]->isThisCloneEnough(found_filtered[i].x, found_filtered[i].y)) {
				caraActual = remFaces[jF];
			}
		}

		if (!caraActual) {
			caraActual = new faceRememberer(found_filtered[i].x, found_filtered[i].y, rand() % 254, rand() % 254, rand() % 254);
			remFaces.push_back(caraActual);
		}
		*/


		cv::Rect r = found_filtered[i];

		r.x *= escala;
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8*escala);
		r.y = r.y*escala;
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8*escala);
		rectangle(frame, r.tl(), r.br(), cv::Scalar(255, 0, 0), 3);
		//rectangle(frame, r.tl(), r.br(), caraActual->color, 3);
	}

	/*
	for (int jF = 0; jF < remFaces.size(); jF++) {
		if (!remFaces[jF]->wasThisFaceReFound) {
			thingNotfound.push_back(jF);
		}
	}

	for (int jF = 0; jF < thingNotfound.size(); jF++) {
		removeThisRem1(thingNotfound[jF]);
	}

	for (int jF = thingNotfound.size() - 1; jF >= 0; jF--) {
		removeThisRem2(thingNotfound[jF]);
	}

	thingNotfound.clear();
	*/
	

	wstring caras = L"";

	caras = L"Personas detectadas : " + to_wstring(found_filtered.size());

	SendDlgItemMessage(ventanaActual, IDC_FACECOUNT, WM_SETTEXT, 0, (LPARAM)caras.c_str());

	return frame;
}

BOOL CALLBACK reGroup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	static HWND lista;
	static HWND combo;
	static HWND comboStatic;
	static HWND listaStatic;

	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;

		//Set Dialog Icon
		HICON hicon = (HICON)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_LAFOTOTIENDA));
		SendMessageW(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);
		SendMessageW(hWnd, WM_SETTEXT, 0, (LPARAM)L"Contador de personas");

		camara.open(0);

		if (!camara.isOpened())  // si no se pudo ahi muere, lastima!
		{
			cout << "No se pudo abrir la camara" << endl;
			break;
		}

		exito = camara.read(frame); // lee un frame
		if (!exito) //si no se pudo lastima de nuevo
		{
			cout << "no pude leer!" << endl;
			break;
		}

		if (frame.rows > 480) {
			resize(frame, frame, cv::Size(640, 480), 0, 0, INTER_CUBIC);
		}

		HWND pictureCntr = GetDlgItem(hWnd, IDC_TEST_PC);

		int xH = (h - frame.rows) / 3;

		MoveWindow(pictureCntr, 0, xH, frame.cols, frame.rows, true);

		SetTimer(hWnd, ID_TIMER, 1000 / 24, (TIMERPROC)NULL);

		ventanaActual = hWnd;

		Mat auxTempoo(frame.rows / escala, frame.cols / escala, CV_8U);
		Mat auxTempoo2(frame.rows / escala, frame.cols / escala, CV_8U);

		aux = auxTempoo.clone();
		aux2 = auxTempoo2.clone();
	}
	break;

	case WM_TIMER:
	{
		switch (wParam)
		{
		case ID_TIMER:
		{
			bool exito = camara.read(frame); // lee un frame
			if (!exito) //si no se pudo lastima de nuevo
			{
				cout << "no pude leer!" << endl;
				break;
			}

			if (!frame.empty())
			{
				//frame = Caras(frame, 1);
				frame = CuerpoCompleto(frame);
			}
			else
			{
				printf("sin informacion");
				break;
			}

			HBITMAP hicon = ConvertCVMatToBMP(frame);
			SendDlgItemMessage(hWnd, IDC_TEST_PC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hicon);
		}
		break;

		}
	}
	break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		int hwId = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_HOME:
		case IDCANCEL:
		{
			camara.release();
			for (faceRememberer* &filtro : remFaces)
			{
				delete filtro;
			}
			remFaces.clear();
			cleanFilterList();
			EndDialog(hWnd, true);
			return true;
		}
		break;
		default:
		{
			return false;
		}
		break;
		}
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BITMAP      bitmap01;
		HGDIOBJ		oldBitmap01;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC         hdcMem01;
		EndPaint(hWnd, &ps);
	}
	break;
	default:
		return false;
	}
	return false;
}