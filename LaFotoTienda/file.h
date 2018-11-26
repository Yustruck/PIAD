#pragma once

OPENFILENAME ofn;
wchar_t szFileName[MAX_PATH] = L"";

Mat originalFrame;

void setDefaultCBF(HWND hWnd) {

	for (int i = 0; i < 6; i++) {
		wstring putaMadre = L"x" + to_wstring(i);
		SendMessage(GetDlgItem(hWnd, IDC_FILTERINTENSITY_F), CB_ADDSTRING, 0, (LPARAM)putaMadre.c_str());
	}

	SendMessage(GetDlgItem(hWnd, IDC_BnW_F), CB_ADDSTRING, 0, (LPARAM)L"Off");
	SendMessage(GetDlgItem(hWnd, IDC_BnW_F), CB_ADDSTRING, 0, (LPARAM)L"On");

	SendMessage(GetDlgItem(hWnd, IDC_SEPIA_F), CB_ADDSTRING, 0, (LPARAM)L"Off");
	SendMessage(GetDlgItem(hWnd, IDC_SEPIA_F), CB_ADDSTRING, 0, (LPARAM)L"On");

	SendMessage(GetDlgItem(hWnd, IDC_SHARP_F1), CB_ADDSTRING, 0, (LPARAM)L"Off");
	SendMessage(GetDlgItem(hWnd, IDC_SHARP_F1), CB_ADDSTRING, 0, (LPARAM)L"On");

	SendMessage(GetDlgItem(hWnd, IDC_DdO_F), CB_ADDSTRING, 0, (LPARAM)L"Off");
	SendMessage(GetDlgItem(hWnd, IDC_DdO_F), CB_ADDSTRING, 0, (LPARAM)L"1");
	SendMessage(GetDlgItem(hWnd, IDC_DdO_F), CB_ADDSTRING, 0, (LPARAM)L"2");
	SendMessage(GetDlgItem(hWnd, IDC_DdO_F), CB_ADDSTRING, 0, (LPARAM)L"3");
	SendMessage(GetDlgItem(hWnd, IDC_DdO_F), CB_ADDSTRING, 0, (LPARAM)L"C");
	SendMessage(GetDlgItem(hWnd, IDC_DdO_F), CB_ADDSTRING, 0, (LPARAM)L"F");

	SendMessage(GetDlgItem(hWnd, IDC_Direction_F), CB_ADDSTRING, 0, (LPARAM)L"Off");
	SendMessage(GetDlgItem(hWnd, IDC_Direction_F), CB_ADDSTRING, 0, (LPARAM)L"N");
	SendMessage(GetDlgItem(hWnd, IDC_Direction_F), CB_ADDSTRING, 0, (LPARAM)L"S");
	SendMessage(GetDlgItem(hWnd, IDC_Direction_F), CB_ADDSTRING, 0, (LPARAM)L"E");
	SendMessage(GetDlgItem(hWnd, IDC_Direction_F), CB_ADDSTRING, 0, (LPARAM)L"O");

	SendMessage(GetDlgItem(hWnd, IDC_FILTERINTENSITY_F), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_BnW_F), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_SEPIA_F), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_SHARP_F1), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_DdO_F), CB_SETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hWnd, IDC_Direction_F), CB_SETCURSEL, 0, 0);
}

void setImage(HWND hWnd, int h) {

	if (originalFrame.empty())
		return;

	frame = getFilteredImage(originalFrame);

	HWND pictureCntr = GetDlgItem(hWnd, IDC_PC);

	int xH = (h - frame.rows) / 2;

	MoveWindow(pictureCntr, 0, xH, frame.cols, frame.rows, true);

	HBITMAP hicon = ConvertCVMatToBMP(frame);
	SendMessage(pictureCntr, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hicon);
}

BOOL CALLBACK fileCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	UNREFERENCED_PARAMETER(lParam);

	static RECT rect;
	static int w = 0;
	static int h = 0;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		int w = rect.right - rect.left;
		int h = rect.bottom - rect.top;

		HICON hicon = (HICON)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_LAFOTOTIENDA));
		SendMessageW(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);

		ZeroMemory(&(ofn), sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = szFileName;
		ofn.hwndOwner = hWnd;

		setDefaultValues();
		setDefaultCBF(hWnd);

		Mat empty;

		originalFrame = empty.clone();

	}
	break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case IDC_OPENFILE:
		{
			ofn.lpstrFilter = L"image file (*.bmp)\0*.bmp\0video file (*.mp4)\0.mp4";

			ofn.nMaxFile = MAX_PATH;

			ofn.Flags = OFN_HIDEREADONLY;

			if (GetOpenFileName(&ofn)) {
				SetDlgItemText(hWnd, IDC_FILENAME, ofn.lpstrFile);

				GetWindowRect(hWnd, &rect);
				w = rect.right - rect.left;
				h = rect.bottom - rect.top;

				wstring fileName = ofn.lpstrFile;
				string aFileName = ws2s(fileName);
				
				cv::String theFileName(aFileName);

				frame = imread(theFileName);

				if (frame.empty()) // Check for invalid input
				{
					cout << "Could not open or find the image" << std::endl;
					return -1;
				}

				/*

				camara.open(theFileName);

				if (!camara.isOpened())  // si no se pudo ahi muere, lastima!
				{
					cout << "No se pudo abrir el archivo" << endl;
					break;
				}

				exito = camara.read(frame); // lee un frame
				if (!exito) //si no se pudo lastima de nuevo
				{
					cout << "No pude leer!" << endl;
					break;
				}
				*/
				if (frame.rows > 480) {
					resize(frame, frame, Size(640, 480), 0, 0, INTER_CUBIC);
				}

				originalFrame = frame.clone();

				setImage(hWnd, h);

			}
		}
		break;

		case IDC_FILTERINTENSITY_F:
		{
			//MessageBox(hWnd, L"", L"", MB_OK);
			wmId = HIWORD(wParam);
			switch (wmId) {
			case CBN_SELCHANGE:
			{
				wchar_t newFliterIntensity[_MAX_PATH];
				HWND combohWnd = GetDlgItem(hWnd, IDC_FILTERINTENSITY_F);
				//SendMessage(combohWnd, CB_GETLBTEXT, SendMessage(combohWnd, CB_GETCURSEL, 0, 0), (LPARAM)newFliterIntensity);

				filterIntensity = SendMessage(combohWnd, CB_GETCURSEL, 0, 0);
				//filterIntensity *= 3;

				//(LPCWSTR)newFliterIntensity
				/*
				wchar_t * pEnd;
				int x = wcstol(newFliterIntensity + 1, &pEnd, 10);
				filterIntensity = x;
				*/
				setImage(hWnd, h);
			}
			break;
			}
		}
		break;

		case IDC_BnW_F:
		{
			wmId = HIWORD(wParam);
			switch (wmId) {
			case CBN_SELCHANGE:
			{
				HWND combohWnd = GetDlgItem(hWnd, IDC_BnW_F);
				BnW = SendMessage(combohWnd, CB_GETCURSEL, 0, 0);
				setImage(hWnd, h);
			}
			break;
			}
		}
		break;

		case IDC_SHARP_F1:
		{
			wmId = HIWORD(wParam);
			switch (wmId) {
			case CBN_SELCHANGE:
			{
				HWND combohWnd = GetDlgItem(hWnd, IDC_SHARP_F1);
				Sharp = SendMessage(combohWnd, CB_GETCURSEL, 0, 0);
				setImage(hWnd, h);
			}
			break;
			}
		}
		break;

		case IDC_DdO_F:
		{
			HWND combohWnd = GetDlgItem(hWnd, IDC_DdO_F);
			DdO = SendMessage(combohWnd, CB_GETCURSEL, 0, 0);
			setImage(hWnd, h);
		}
		break;

		case IDC_Direction_F:
		{
			HWND combohWnd = GetDlgItem(hWnd, IDC_Direction_F);
			Direction = SendMessage(combohWnd, CB_GETCURSEL, 0, 0);
			setImage(hWnd, h);
		}
		break;

		case IDC_SEPIA_F:
		{
			HWND combohWnd = GetDlgItem(hWnd, IDC_SEPIA_F);
			Sepia = SendMessage(combohWnd, CB_GETCURSEL, 0, 0);
			setImage(hWnd, h);
		}
		break;

		case IDC_HOME:
		case IDCANCEL:
		{
			EndDialog(hWnd, true);
			return true;
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
	}

	return 0;
}