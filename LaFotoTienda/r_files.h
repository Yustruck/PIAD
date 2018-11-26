#pragma once

Mat oriImg;
Mat noAlteredImg;

bool imgFile;

OPENFILENAME ofn;
wchar_t szFileName[MAX_PATH] = L"";
wchar_t szFileTitle[MAX_PATH] = L"";
wchar_t szFileExt[MAX_PATH] = L"";
wstring fileName = L"";

void setImage(HWND hWnd, int h) {
	if (oriImg.empty())
		return;

	if (!turnOffAllFilters)
		frame = getListFilterImg(oriImg);
	else
		frame = oriImg.clone();

	HWND pictureCntr = GetDlgItem(hWnd, IDC_TEST_PC);

	int xH = (h - frame.rows) / 2;

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

		lista = GetDlgItem(hWnd, IDC_FILTROLISTA);
		combo = GetDlgItem(hWnd, IDC_COMBOFILTROS);
		comboStatic = GetDlgItem(hWnd, IDC_OP1);
		listaStatic = GetDlgItem(hWnd, IDC_OP2);

		ZeroMemory(&(ofn), sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = szFileName; //
		ofn.lpstrFileTitle = szFileTitle;
		ofn.nMaxFileTitle = MAX_PATH;
		ofn.hwndOwner = hWnd;

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
			/*
			bool exito = camara.read(frame); // lee un frame
			if (!exito) //si no se pudo lastima de nuevo
			{
				cout << "no pude leer!" << endl;
				break;
			}

			if (!turnOffAllFilters)
				frame = getListFilterImg(frame);

			//Show final image shit
			HBITMAP hicon = ConvertCVMatToBMP(frame);
			SendDlgItemMessage(hWnd, IDC_TEST_PC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hicon);
			*/
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
			setImage(hWnd, h);
		}
		break;

		case IDC_FILTERUP:
		{
			int elItem = SendMessage(lista, LB_GETCURSEL, 0, 0);
			upwardsList(lista, elItem);
			setImage(hWnd, h);
		}
		break;

		case IDC_FILTERDOWN:
		{
			int elItem = SendMessage(lista, LB_GETCURSEL, 0, 0);
			downwardsList(lista, elItem);
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
			setImage(hWnd, h);
		}
		break;

		case IDC_FILTROCOMBO:
		{
			int elItem = SendDlgItemMessage(hWnd, IDC_FILTROCOMBO, CB_GETCURSEL, 0, 0);
			selectedFiltro->setOption(0, elItem);
			setImage(hWnd, h);
		}
		break;

		case IDC_FILTROOTRO:
		{
			int elItem = SendDlgItemMessage(hWnd, IDC_FILTROOTRO, CB_GETCURSEL, 0, 0);
			selectedFiltro->setOption(1, elItem);
			setImage(hWnd, h);
		}
		break;

		case IDC_LOAD:
		{
			ofn.lpstrFilter = L"image file (*.bmp)\0*.bmp\0video file (*.mpg4)\0.mpg4";

			ofn.nMaxFile = MAX_PATH;

			ofn.Flags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

			if (GetOpenFileName(&ofn)) {
				SetDlgItemText(hWnd, IDC_FILENAME, ofn.lpstrFile);

				fileName = ofn.lpstrFileTitle;

				wstring filext = PathFindExtension(fileName.c_str());

				if (filext.compare(L".bmp")==0) {
					imgFile = true;
				}
				else {
					imgFile = false;
					SetTimer(hWnd, ID_TIMER, 1000 / 24, (TIMERPROC)NULL);
				}

				GetWindowRect(hWnd, &rect);
				w = rect.right - rect.left;
				h = rect.bottom - rect.top;

				wstring fileName = ofn.lpstrFile;
				string aFileName = ws2s(fileName);

				cv::String theFileName(aFileName);

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
					//VideoCapture camara(theFileName);
					//camara.open(theFileName, CAP_ANY);
					camara.open(theFileName, CAP_ANY);
					//camara.open()

					if (!camara.isOpened()) {
						cout << "Error opening video stream or file" << endl;
						return -1;
					}

					camara >> frame;

					if (frame.rows > 480 || frame.cols > 640) {
						resize(frame, frame, cv::Size(640, 480), 0, 0, INTER_CUBIC);
					}

					oriImg = frame.clone();
				}


				setImage(hWnd, h);
				enableAll();
				simpleFilterIni();
				cleanFilterList();
				SendMessage(lista, LB_RESETCONTENT, 0, 0);
				cleanEditedFiltro(hWnd);

				EnableWindow(GetDlgItem(hWnd, IDC_SAVE), true);
			}
		}
		break;

		case IDC_SAVE:
		{
			frame = getListFilterImg(oriImg);
			HBITMAP hicon = ConvertCVMatToBMP(frame);

			GdiplusStartupInput gdiplusStartupInput;
			ULONG_PTR gdiplusToken;
			GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

			HBITMAP hBitmap = hicon;
			Bitmap *image = new Bitmap(hBitmap, NULL);

			CLSID myClsId;
			int retVal = GetEncoderClsid(L"image/bmp", &myClsId);

			wstring output = L"";
			output = L"output - " + fileName + L".bmp";

			image->Save(output.c_str(), &myClsId);
			delete image;

			GdiplusShutdown(gdiplusToken);
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