#pragma once

//webcam callback
BOOL CALLBACK reWebcam(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

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

		simpleFilterIni();

		lista = GetDlgItem(hWnd, IDC_FILTROLISTA);
		combo = GetDlgItem(hWnd, IDC_COMBOFILTROS);
		comboStatic = GetDlgItem(hWnd, IDC_OP1);
		listaStatic = GetDlgItem(hWnd, IDC_OP2);

		updateList(lista);

		addComboBox(combo);

		turnOffAllFilters = false;

		SendDlgItemMessage(hWnd, IDC_TURNFILTERS, WM_SETTEXT, 0, (LPARAM)L"Apagar filtros");

		ventanaActual = hWnd;

		cleanEditedFiltro(hWnd);
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

			if(!turnOffAllFilters)
				frame = getListFilterImg(frame);

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

		case IDC_TURNFILTERS:
		{
			turnOffAllFilters = !turnOffAllFilters;

			if (turnOffAllFilters) {
				SendDlgItemMessage(hWnd, IDC_TURNFILTERS, WM_SETTEXT, 0, (LPARAM)L"Encender filtros");
			}
			else {
				SendDlgItemMessage(hWnd, IDC_TURNFILTERS, WM_SETTEXT, 0, (LPARAM)L"Apagar filtros");
			}
		}
		break;

		case IDC_FILTERUP:
		{
			int elItem = SendMessage(lista, LB_GETCURSEL, 0, 0);
			upwardsList(lista, elItem);
		}
		break;

		case IDC_FILTERDOWN:
		{
			int elItem = SendMessage(lista, LB_GETCURSEL, 0, 0);
			downwardsList(lista, elItem);
		}
		break;

		case IDC_ADDFILTRO:
		{
			int elItem = SendMessage(combo, CB_GETCURSEL, 0, 0);
			addFiltro(lista, elItem);
			elItem = filtros.size() - 1;
			editFiltro(lista, elItem);
			SendMessage(lista, LB_SETCURSEL, elItem, 0);
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
		}
		break;

		case IDC_FILTROCOMBO:
		{
			int elItem = SendDlgItemMessage(hWnd, IDC_FILTROCOMBO, CB_GETCURSEL, 0, 0);
			selectedFiltro->setOption(0, elItem);
		}
		break;

		case IDC_FILTROOTRO:
		{
			int elItem = SendDlgItemMessage(hWnd, IDC_FILTROOTRO, CB_GETCURSEL, 0, 0);
			selectedFiltro->setOption(1, elItem);
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
