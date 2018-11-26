#pragma once

#define ID_TIMER 6910294

//Global variables
Mat frame; //aqui guardaremos el frame
bool exito;
VideoCapture camara; //tipo de dato correspondiente a la camara
Mat gris2;

void setDefaultValues() {
	filterIntensity = 0;
	BnW = 0;
	Sharp = 0;
	DdO = 0;
	Direction = 0;
	Sepia = 0;
}

void setDefaultCB(HWND hWnd) {

	for (int i = 0; i < 6; i++) {
		wstring putaMadre = L"x" + to_wstring(i);
		SendMessage(GetDlgItem(hWnd, IDC_FILTERINTENSITY), CB_ADDSTRING, 0, (LPARAM)putaMadre.c_str());
	}

	SendMessage(GetDlgItem(hWnd, IDC_BnW), CB_ADDSTRING, 0, (LPARAM)L"Off");
	SendMessage(GetDlgItem(hWnd, IDC_BnW), CB_ADDSTRING, 0, (LPARAM)L"On");

	SendMessage(GetDlgItem(hWnd, IDC_SEPIA), CB_ADDSTRING, 0, (LPARAM)L"Off");
	SendMessage(GetDlgItem(hWnd, IDC_SEPIA), CB_ADDSTRING, 0, (LPARAM)L"On");

	SendMessage(GetDlgItem(hWnd, IDC_SHARP), CB_ADDSTRING, 0, (LPARAM)L"Off");
	SendMessage(GetDlgItem(hWnd, IDC_SHARP), CB_ADDSTRING, 0, (LPARAM)L"On");

	SendMessage(GetDlgItem(hWnd, IDC_DdO), CB_ADDSTRING, 0, (LPARAM)L"Off");
	SendMessage(GetDlgItem(hWnd, IDC_DdO), CB_ADDSTRING, 0, (LPARAM)L"1");
	SendMessage(GetDlgItem(hWnd, IDC_DdO), CB_ADDSTRING, 0, (LPARAM)L"2");
	SendMessage(GetDlgItem(hWnd, IDC_DdO), CB_ADDSTRING, 0, (LPARAM)L"3");
	SendMessage(GetDlgItem(hWnd, IDC_DdO), CB_ADDSTRING, 0, (LPARAM)L"C");
	SendMessage(GetDlgItem(hWnd, IDC_DdO), CB_ADDSTRING, 0, (LPARAM)L"F");

	SendMessage(GetDlgItem(hWnd, IDC_Direction), CB_ADDSTRING, 0, (LPARAM)L"Off");
	SendMessage(GetDlgItem(hWnd, IDC_Direction), CB_ADDSTRING, 0, (LPARAM)L"N");
	SendMessage(GetDlgItem(hWnd, IDC_Direction), CB_ADDSTRING, 0, (LPARAM)L"S");
	SendMessage(GetDlgItem(hWnd, IDC_Direction), CB_ADDSTRING, 0, (LPARAM)L"E");
	SendMessage(GetDlgItem(hWnd, IDC_Direction), CB_ADDSTRING, 0, (LPARAM)L"O");

	SendMessage(GetDlgItem(hWnd, IDC_FILTERINTENSITY), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_BnW), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_SEPIA), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_SHARP), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_DdO), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_Direction), CB_SETCURSEL, 0, 0);
}

//webcam callback
BOOL CALLBACK colback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		/*
		//Set transparency, not exactly what I wanted
		SetWindowLong(hWnd,
		GWL_EXSTYLE,
		GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		// Make this window 70% alpha
		SetLayeredWindowAttributes(hWnd, 0, (255 * 70) / 100, LWA_ALPHA);
		*/

		//Define drawing region of dialog

		RECT rect;
		GetWindowRect(hWnd, &rect);
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;
		/*
		//  Create the top ellipse
		//HRGN rgn1 = CreateEllipticRgn(1, 1, w, h / 2 + 30);
		//HRGN rgn1 = CreateEllipticRgn(-w, 0, w*2, h);
		HRGN rgn1 = CreateRoundRectRgn(0, 0, w, h, w/10, h/5);

		//  Create the bottom ellipse
		//HRGN rgn2 = CreateEllipticRgn(1, h / 2 - 30, w, h);

		//CombineRgn(rgn1, rgn1, rgn2, RGN_OR); //Combina los dos en rgn1

		SetWindowRgn(hWnd, rgn1, true); //Ponle a la venta la figura

		*/
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
			resize(frame, frame, Size(640, 480), 0, 0, INTER_CUBIC);
		}

		HWND pictureCntr = GetDlgItem(hWnd, IDC_TEST_PC);

		int xH = (h - frame.rows) / 2;

		MoveWindow(pictureCntr, 0, xH, frame.cols, frame.rows, true);

		SetTimer(hWnd, ID_TIMER, 1000 / 24, (TIMERPROC)NULL);

		//wstring putaMadre[] = { L"x1", L"x2", L"x3", L"x4", L"x5" };
		//(LPARAM)putaMadre[i].c_str()

		setDefaultCB(hWnd);

		setDefaultValues();


	}break;

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

			frame = getFilteredImage(frame);

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
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_FILTERINTENSITY:
		{
			//MessageBox(hWnd, L"", L"", MB_OK);
			wmId = HIWORD(wParam);
			switch (wmId) {
			case CBN_SELCHANGE:
			{
				wchar_t newFliterIntensity[_MAX_PATH];
				HWND combohWnd = GetDlgItem(hWnd, IDC_FILTERINTENSITY);
				//SendMessage(combohWnd, CB_GETLBTEXT, SendMessage(combohWnd, CB_GETCURSEL, 0, 0), (LPARAM)newFliterIntensity);
				
				filterIntensity = SendMessage(combohWnd, CB_GETCURSEL, 0, 0);
				//filterIntensity *= 3;

				//(LPCWSTR)newFliterIntensity
				/*
				wchar_t * pEnd;
				int x = wcstol(newFliterIntensity + 1, &pEnd, 10);
				filterIntensity = x;
				*/

			}
			break;
			}
		}
		break;

		case IDC_BnW:
		{
			wmId = HIWORD(wParam);
			switch (wmId) {
			case CBN_SELCHANGE:
			{
				HWND combohWnd = GetDlgItem(hWnd, IDC_BnW);
				BnW = SendMessage(combohWnd, CB_GETCURSEL, 0, 0);
			}
			break;
			}
		}
		break;

		case IDC_SHARP:
		{
			wmId = HIWORD(wParam);
			switch (wmId) {
			case CBN_SELCHANGE:
			{
				HWND combohWnd = GetDlgItem(hWnd, IDC_SHARP);
				Sharp = SendMessage(combohWnd, CB_GETCURSEL, 0, 0);
			}
			break;
			}
		}
		break;

		case IDC_DdO:
		{
			HWND combohWnd = GetDlgItem(hWnd, IDC_DdO);
			DdO = SendMessage(combohWnd, CB_GETCURSEL, 0, 0);
		}
		break;

		case IDC_Direction:
		{
			HWND combohWnd = GetDlgItem(hWnd, IDC_Direction);
			Direction = SendMessage(combohWnd, CB_GETCURSEL, 0, 0);
		}
		break;

		case IDC_SEPIA:
		{
			HWND combohWnd = GetDlgItem(hWnd, IDC_SEPIA);
			Sepia = SendMessage(combohWnd, CB_GETCURSEL, 0, 0);
		}
		break;

		case IDC_HOME:
		case IDCANCEL:
		{
			camara.release();
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
		/*

		hdcMem01 = CreateCompatibleDC(hdc);
		oldBitmap01 = SelectObject(hdcMem01, hBitmap01);

		GetObject(hBitmap01, sizeof(bitmap01), &bitmap01);
		BitBlt(hdc, 0, 0, bitmap01.bmWidth, bitmap01.bmHeight, hdcMem01, 0, 0, SRCCOPY);

		SelectObject(hdcMem01, oldBitmap01);
		DeleteDC(hdcMem01);
		EndPaint(hWnd, &ps);
		*/

		/*
		RECT LpRectt;
		GetWindowRect(GetDlgItem(hWnd, IDC_STATIC), &LpRectt);
		HDC hdcText = BeginPaint(GetDlgItem(hWnd, IDC_STATIC), &ps);

		SetTextColor(hdcText, RGB(200, 10, 60));
		SetBkMode(hdcText, TRANSPARENT);

		Rectangle(hdcText, LpRectt.left, LpRectt.top, LpRectt.right, LpRectt.bottom);
		DrawText(hdcText, L"teststring", 10, &LpRectt, 0);

		EndPaint(GetDlgItem(hWnd, IDC_STATIC), &ps);
		*/

	}
	break;
	default:
		return false;
	}
	return false;
}