#pragma once

Mat oriImg;
Mat noAlteredImg;

bool imgFile;

bool playVideo = true;

int famInVideo;

wstring fileName = L"";
wstring filext =L"";

void setImage(HWND hWnd, int h) {
	if (oriImg.empty())
		return;

	if (!turnOffAllFilters)
		frame = getListFilterImg(oriImg);
	else
		frame = oriImg.clone();

	HWND pictureCntr = GetDlgItem(hWnd, IDC_TEST_PC);

	int xH = (h - frame.rows) / 3;

	MoveWindow(pictureCntr, 0, xH, frame.cols, frame.rows, true);

	HBITMAP hicon = ConvertCVMatToBMP(frame);
	SendMessage(pictureCntr, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hicon);
}

//files callback
BOOL CALLBACK reFiles(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	static HWND lista;
	static HWND combo;
	static HWND comboStatic;
	static HWND listaStatic;

	static RECT rect;
	static int w = 0;
	static int h = 0;

	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		rect;
		GetWindowRect(hWnd, &rect);
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;

		//Set Dialog Icon
		HICON hicon = (HICON)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_LAFOTOTIENDA));
		SendMessageW(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);
		SendMessageW(hWnd, WM_SETTEXT, 0, (LPARAM)L"Archivos");

		lista = GetDlgItem(hWnd, IDC_FILTROLISTA);
		combo = GetDlgItem(hWnd, IDC_COMBOFILTROS);
		comboStatic = GetDlgItem(hWnd, IDC_OP1);
		listaStatic = GetDlgItem(hWnd, IDC_OP2);

		updateList(lista);

		addComboBox(combo);

		turnOffAllFilters = false;

		SendDlgItemMessage(hWnd, IDC_TURNFILTERS, WM_SETTEXT, 0, (LPARAM)L"Apagar filtros");
		ShowWindow(GetDlgItem(hWnd, IDC_SAVE), SW_SHOW);
		ShowWindow(GetDlgItem(hWnd, IDC_LOAD), SW_SHOW);
		EnableWindow(GetDlgItem(hWnd, IDC_SAVE), false);


		Mat emp;
		oriImg = emp.clone();

		ventanaActual = hWnd;

		ventanaActual = hWnd;

		simpleFilterIni();
		cleanFilterList();
		SendMessage(lista, LB_RESETCONTENT, 0, 0);
		cleanEditedFiltro(hWnd);

		disableAll();
	}
	break;

	case WM_TIMER:
	{
		switch (wParam)
		{
		case ID_TIMER:
		{
			if (imgFile)
				break;

			if (playVideo) {
				int fam = camara.get(CAP_PROP_POS_FRAMES);
				if (fam >= famInVideo) {
					camara.set(CAP_PROP_POS_AVI_RATIO, 0.0);
				}
				camara >> frame;
				if (frame.empty()) {
					camara.set(CAP_PROP_POS_AVI_RATIO, 0.0);
				}
				if (frame.rows > 480 || frame.cols > 640) {
					resize(frame, frame, cv::Size(640, 480), 0, 0, INTER_CUBIC);
				}
				oriImg = frame.clone();
			}
			else {
				frame = oriImg.clone();
			}
			

			if (!turnOffAllFilters)
				frame = getListFilterImg(frame);
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

		case IDC_TURNFILTERS:
		{
			turnOffAllFilters = !turnOffAllFilters;

			if (turnOffAllFilters) {
				SendDlgItemMessage(hWnd, IDC_TURNFILTERS, WM_SETTEXT, 0, (LPARAM)L"Encender filtros");
			}
			else {
				SendDlgItemMessage(hWnd, IDC_TURNFILTERS, WM_SETTEXT, 0, (LPARAM)L"Apagar filtros");
			}
			if(imgFile)
				setImage(hWnd, h);
		}
		break;

		case IDC_FILTERUP:
		{
			int elItem = SendMessage(lista, LB_GETCURSEL, 0, 0);
			upwardsList(lista, elItem);
			if (imgFile)
				setImage(hWnd, h);
		}
		break;

		case IDC_FILTERDOWN:
		{
			int elItem = SendMessage(lista, LB_GETCURSEL, 0, 0);
			downwardsList(lista, elItem);
			if (imgFile)
				setImage(hWnd, h);
		}
		break;

		case IDC_ADDFILTRO:
		{
			int elItem = SendMessage(combo, CB_GETCURSEL, 0, 0);
			addFiltro(lista, elItem);
			elItem = filtros.size() - 1;
			editFiltro(lista, elItem);
			SendMessage(lista, LB_SETCURSEL, elItem, 0);
			if (imgFile)
				setImage(hWnd, h);
		}
		break;

		case IDC_DELETEFILTRO:
		{
			int elItem = SendMessage(lista, LB_GETCURSEL, 0, 0);
			deleteFiltro(lista, elItem);
			elItem--;
			if (elItem >= 0) {
				editFiltro(lista, elItem);
				SendMessage(lista, LB_SETCURSEL, elItem, 0);
			}
			if (imgFile)
				setImage(hWnd, h);
		}
		break;

		case IDC_FILTROLISTA:
		{
			switch (hwId)
			{
			case LBN_SELCHANGE:
			{
				int elItem = SendMessage(lista, LB_GETCURSEL, 0, 0);
				editFiltro(lista, elItem);
			}
			break;

			default:
			{

			}
			break;
			}
		}
		break;

		case IDC_VISIBLE:
		{
			changeTurnedOff();
			if (imgFile)
				setImage(hWnd, h);
		}
		break;

		case IDC_FILTROCOMBO:
		{
			int elItem = SendDlgItemMessage(hWnd, IDC_FILTROCOMBO, CB_GETCURSEL, 0, 0);
			selectedFiltro->setOption(0, elItem);
			if (imgFile)
				setImage(hWnd, h);
		}
		break;

		case IDC_FILTROOTRO:
		{
			int elItem = SendDlgItemMessage(hWnd, IDC_FILTROOTRO, CB_GETCURSEL, 0, 0);
			selectedFiltro->setOption(1, elItem);
			if (imgFile)
				setImage(hWnd, h);
		}
		break;

		case IDC_RESUMEVIDEO:
		{
			playVideo = true;
		}
		break;

		case IDC_PAUSEVIDEO:
		{
			playVideo = false;
		}
		break;

		case IDC_RESETVIDEO:
		{
			camara.set(CAP_PROP_POS_AVI_RATIO, 0.0);
			playVideo = true;
		}
		break;

		case IDC_LOAD:
		{

			OPENFILENAME ofn;
			wchar_t szFileName[MAX_PATH] = L"";
			wchar_t szFileTitle[MAX_PATH] = L"";
			wchar_t szFileExt[MAX_PATH] = L"";
			ZeroMemory(&(ofn), sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = szFileName;
			ofn.lpstrFileTitle = szFileTitle;
			ofn.nMaxFileTitle = MAX_PATH;
			ofn.hwndOwner = hWnd;

			ofn.lpstrFilter = 
				L"Imagenes (*.bmp, *.jpg)\0*.bmp;*.jpg;*.jpeg\Videos (*.mp4, *.avi)\0*.mp4;*.avi\0";

			ofn.nMaxFile = MAX_PATH;

			ofn.Flags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

			if (GetOpenFileName(&ofn)) {
				SetDlgItemText(hWnd, IDC_FILENAME, ofn.lpstrFile);

				fileName = ofn.lpstrFileTitle;

				filext = PathFindExtension(ofn.lpstrFileTitle);

				if (filext.compare(L".bmp")==0 || filext.compare(L".jpg") == 0 || filext.compare(L".jpeg") == 0) {
					imgFile = true;
					ShowWindow(GetDlgItem(hWnd, IDC_PAUSEVIDEO), SW_HIDE);
					ShowWindow(GetDlgItem(hWnd, IDC_RESUMEVIDEO), SW_HIDE);
					ShowWindow(GetDlgItem(hWnd, IDC_RESETVIDEO), SW_HIDE);
				}
				else {
					if (filext.compare(L".mp4") == 0 || filext.compare(L".avi") == 0) {
						imgFile = false;
						ShowWindow(GetDlgItem(hWnd, IDC_PAUSEVIDEO), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_RESUMEVIDEO), SW_SHOW);
						ShowWindow(GetDlgItem(hWnd, IDC_RESETVIDEO), SW_SHOW);
						playVideo = true;
					}
					else {
						MessageBox(hWnd, L"El archivo seleccionado no valido ", L"Error", MB_ICONERROR | MB_OK);
						break;
					}
				}

				GetWindowRect(hWnd, &rect);
				w = rect.right - rect.left;
				h = rect.bottom - rect.top;

				wstring fileName = ofn.lpstrFile;
				string aFileName = ws2s(fileName);

				cv::String theFileName(aFileName);

				KillTimer(hWnd, ID_TIMER);

				if (imgFile) {
					frame = imread(theFileName);

					if (frame.empty()) // Check for invalid input
					{
						cout << "Could not open or find the image" << std::endl;
						return -1;
					}
					noAlteredImg = frame.clone();

					if (frame.rows > 480 || frame.cols > 640) {
						resize(frame, frame, cv::Size(640, 480), 0, 0, INTER_CUBIC);
					}

					oriImg = frame.clone();
				}
				else {
					cv::String oaoo = aFileName;
					camara.open(oaoo);					
					
					if (!camara.isOpened()) {
						cout << "Error opening video stream or file" << endl;
						MessageBox(hWnd, L"Un error se a cometido al cargar su video", L"Error", MB_ICONERROR | MB_OK);
						return -1;
					}
					famInVideo = camara.get(CAP_PROP_FRAME_COUNT);
					camara >> frame;
					if (frame.rows > 480 || frame.cols > 640) {
						resize(frame, frame, cv::Size(640, 480), 0, 0, INTER_CUBIC);
					}
					HWND pictureCntr = GetDlgItem(hWnd, IDC_TEST_PC);
					int xH = (h - frame.rows) / 3;
					MoveWindow(pictureCntr, 0, xH, frame.cols, frame.rows, true);

					double fpsCam = camara.get(CAP_PROP_FPS);
					double fps = 1000.0 / fpsCam;

					SetTimer(hWnd, ID_TIMER, fps, (TIMERPROC)NULL);
				}


				setImage(hWnd, h);
				enableAll();
				simpleFilterIni();
				cleanFilterList();
				SendMessage(lista, LB_RESETCONTENT, 0, 0);
				cleanEditedFiltro(hWnd);

				//EnableWindow(GetDlgItem(hWnd, IDC_SAVENAME), true);
				EnableWindow(GetDlgItem(hWnd, IDC_SAVE), true);
			}
		}
		break;

		case IDC_SAVE:
		{
			OPENFILENAME ofn;
			wchar_t szFileName[MAX_PATH] = L"";
			wchar_t szFileTitle[MAX_PATH] = L"";
			wchar_t szFileExt[MAX_PATH] = L"";
			ZeroMemory(&(ofn), sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = szFileName;
			ofn.lpstrFileTitle = szFileTitle;
			ofn.nMaxFileTitle = MAX_PATH;
			ofn.hwndOwner = hWnd;

			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST;

			if (imgFile) {
				ofn.lpstrFilter =
					L"Image file (*.bmp)\0*.bmp\0";
				ofn.lpstrDefExt = (LPCWSTR)L"bmp";

				if (GetSaveFileName(&ofn)) {

					if (!turnOffAllFilters)
						frame = getListFilterImg(oriImg);
					else
						frame = oriImg.clone();

					HBITMAP hicon = ConvertCVMatToBMP(frame);

					GdiplusStartupInput gdiplusStartupInput;
					ULONG_PTR gdiplusToken;
					GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

					HBITMAP hBitmap = hicon;
					Bitmap *image = new Bitmap(hBitmap, NULL);

					CLSID myClsId;
					int retVal = GetEncoderClsid(L"image/bmp", &myClsId);

					/*
					wstring output = L"";
					WCHAR ooo[MAX_PATH];
					SendDlgItemMessage(hWnd, IDC_SAVENAME, WM_GETTEXT, MAX_PATH, (WPARAM)&ooo);
					output = ooo;
					output = output + L".bmp";
					*/
					wstring output = L"";
					output = ofn.lpstrFile;
					String nameOut = ws2s(output);

					image->Save(output.c_str(), &myClsId);
					delete image;

					output = L"Archivo se ha salvado exitosamente";
					MessageBox(hWnd, output.c_str(), L"Salvado", MB_OK);

					GdiplusShutdown(gdiplusToken);
				}
			}
			else {

				ofn.lpstrFilter =
					L"video file (*.avi)\0*.avi\0";
				ofn.lpstrDefExt = (LPCWSTR)L"avi";

				ofn.nMaxFile = MAX_PATH;

				ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST;

				if (GetSaveFileName(&ofn)) {
					VideoWriter outputVideo;
					int ex = static_cast<int>(camara.get(CV_CAP_PROP_FOURCC));
					cv::Size S = cv::Size((int)camara.get(CV_CAP_PROP_FRAME_WIDTH), (int)camara.get(CV_CAP_PROP_FRAME_HEIGHT));

					wstring output = L"";
					output = ofn.lpstrFile;
					String nameOut = ws2s(output);

					outputVideo.open(nameOut, 0, camara.get(CV_CAP_PROP_FPS), S, true);
					if (!outputVideo.isOpened())
					{
						cout << "Could not open the output video for write: " << nameOut << endl;
						return -1;
					}

					Mat src;

					camara.set(CAP_PROP_POS_AVI_RATIO, 0.0);
					while (true)
					{
						camara >> src;
						if (src.empty()) break;

						if (!turnOffAllFilters)
							src = getListFilterImg(src);

						outputVideo.write(src.clone());
					}

					cout << "Finished writing" << endl;
					outputVideo.release();

					output = L"Archivo se ha salvado exitosamente";
					MessageBox(hWnd, output.c_str(), L"Salvado", MB_OK);
				}
			}
		}
		break;
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