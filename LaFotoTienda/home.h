#pragma once

INT_PTR CALLBACK homeCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_INITDIALOG:
	{
		//Set Dialog Icon
		HICON hicon = (HICON)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_LAFOTOTIENDA));
		SendMessageW(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);
		return (INT_PTR)TRUE;

	}break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{

		case IDC_WEBCAM:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_R_WEBCAM), hWnd, (DLGPROC)reWebcam);
		}
		break;

		case IDC_FILE:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_R_WEBCAM), hWnd, (DLGPROC)reFiles);
		}
		break;

		case IDC_GROUP:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_GROUP), hWnd, (DLGPROC)reGroup);
		}
		break;

		case IDC_EXIT:
		case IDCANCEL:
		{
			DestroyWindow(hWnd);
			return (INT_PTR)TRUE;
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
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (INT_PTR)FALSE;
}