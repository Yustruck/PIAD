#pragma once

//definicion de los haarcascades
String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye.xml";

//contenedores de los haarcascades
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

Mat CaraYOjos(Mat frame, float escala)
{
	//arreglo de rectangulos de caras detectadas
	std::vector<cv::Rect> faces;

	//nuevo frame se utilizara para la conversion a gris
	Mat frame_gray;

	//creamos una matriz de pixeles menor segun la escala
	//para procesar mas rapido
	Mat nuevo(cvRound(frame.rows / escala), cvRound(frame.cols / escala), CV_8UC1);
	//aqui convertimos a escala de gris
	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	//aqui generamos un cuadro menor de tama�o con filtro lineal
	resize(frame_gray, nuevo, nuevo.size(), 0, 0, INTER_LINEAR);
	//acentuamos los contrastes con equalizacion de historgrama
	equalizeHist(nuevo, nuevo);

	//este contenedor con el metodo detectMultiScale busca en la imagen y entrega un arreglo de rectangulos
	//donde cada uno encierra una cara
	face_cascade.detectMultiScale(nuevo, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

	//le damos una recorrida a cada rectangulo y aprovechamos sus datos para generar circunferencias 
	//una para cada cara
	for (int i = 0; i < faces.size(); i++)
	{
		//definimos el centro y compensamos con la escala
		cv::Point center((faces[i].x + faces[i].width*0.5)*escala, (faces[i].y + faces[i].height*0.5)*escala);
		
		cv::Rect r = faces.at(i);
		r.x *= escala;
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8*escala);
		r.y = r.y*escala;
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8*escala);
		rectangle(frame, r.tl(), r.br(), cv::Scalar(10, 10, 125), 3);

		Mat faceROI = nuevo(faces[i]);
	}

	wstring caras=L"";

	caras = L"Caras detectadas : " + to_wstring(faces.size());

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

		if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading\n"); exit(0); };
		if (!eyes_cascade.load(eyes_cascade_name)) { printf("--(!)Error loading\n"); exit(0); };

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

		int xH = (h - frame.rows) / 2;

		MoveWindow(pictureCntr, 0, xH, frame.cols, frame.rows, true);

		SetTimer(hWnd, ID_TIMER, 1000 / 24, (TIMERPROC)NULL);

		ventanaActual = hWnd;
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
				frame = CaraYOjos(frame, 1);
			}
			else
			{
				printf("sin informacion");
				break;
			}

			//Show final image shit
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