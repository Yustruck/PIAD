#pragma once

vector<FilterBase*> filtros;
FilterBase* selectedFiltro;

HWND ventanaActual;

void cleanEditedFiltro(HWND hWnd);

//Como agregar filtros a la lista
void simpleFilterIni() {
	FilterBase* boo;

	/*

	boo = new BlancoYNegro();
	boo->setOption(0, 0);
	filtros.push_back(boo);

	boo = new Sharp();
	filtros.push_back(boo);

	*/

	/*
	boo = new Mediana();
	filtros.push_back(boo);
	*/

	//boo = new Sepia();
	//filtros.push_back(boo);

}

Mat getListFilterImg(Mat frame) {
	for (FilterBase* &filtro : filtros)
	{
		filtro->getFilteredImg(frame, frame);
	}
	return frame;
}

void cleanFilterList() {
	for (FilterBase* &filtro : filtros)
	{
		delete filtro;
	}
	filtros.clear();
	selectedFiltro = NULL;
}

void updateList(HWND &lista) {
	SendMessage(lista, LB_RESETCONTENT, 0, 0);
	for (FilterBase* &filtro : filtros)
	{
		SendMessage(lista, LB_ADDSTRING, 0, (LPARAM)filtro->name.c_str());
	}
}

void upwardsList(HWND &lista, int id) {
	if (id <= 0)
		return;

	iter_swap(filtros.begin() + id, filtros.begin() + id-1);

	updateList(lista);

	SendMessage(lista, LB_SETCURSEL, id - 1, 0);
}

void downwardsList(HWND &lista, int id) {
	if (id < 0)
		return;
	if (id >= filtros.size()-1)
		return;

	iter_swap(filtros.begin() + id, filtros.begin() + id + 1);

	updateList(lista);

	SendMessage(lista, LB_SETCURSEL, id + 1, 0);
}

void addComboBox(HWND &combo) {
	SendMessage(combo, CB_RESETCONTENT, 0, 0);

	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Blanco y Negro");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Sepia");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Media");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Detector de Orillas");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Sharp");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Direction");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Mediana");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Media Ponderada");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Gaussiano");
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)L"Histograma");

	SendMessage(combo, CB_SETCURSEL, 0, 0);
}

void addFiltro(HWND &lista, int id) {
	FilterBase* boo;

	switch (id) {
		case 0:
		{
			boo = new BlancoYNegro();
		}
		break;
		case 1:
		{
			boo = new Sepia();
		}
		break;
		case 2:
		{
			boo = new Media();
		}
		break;
		case 3:
		{
			boo = new DetectorDeOrilla();
		}
		break;
		case 4:
		{
			boo = new Sharp();
		}
		break;
		case 5:
		{
			boo = new Direction();
		}
		break;
		case 6:
		{
			boo = new Mediana();
		}
		break;

		case 7:
		{
			boo = new MediaPon();
		}
		break;

		case 8:
		{
			boo = new Gaussiano();
		}
		break;

		case 9:
		{
			boo = new Histograma();
		}
		break;

		default:
			return;
			break;
	}

	filtros.push_back(boo);

	updateList(lista);
}

void deleteFiltro(HWND &lista, int id) {

	if (id < 0)
		return;

	if (filtros.at(id) == selectedFiltro)
		cleanEditedFiltro(ventanaActual);

	delete filtros.at(id);

	filtros.erase(filtros.begin() + id);

	updateList(lista);
}

void cleanEditedFiltro(HWND hWnd) {
	selectedFiltro = NULL;
	
	SendDlgItemMessage(hWnd, IDC_OP1, WM_SETTEXT, 0, (LPARAM)L"Opcion 1:");
	SendDlgItemMessage(hWnd, IDC_OP2, WM_SETTEXT, 0, (LPARAM)L"Opcion 2:");
	SendDlgItemMessage(hWnd, IDC_FILTROCOMBO, WM_CLEAR, 0, 0);
	SendDlgItemMessage(hWnd, IDC_FILTROCOMBO, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hWnd, IDC_FILTROOTRO, WM_CLEAR, 0, 0);
	SendDlgItemMessage(hWnd, IDC_FILTROOTRO, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hWnd, IDC_NAMEFILTRO, WM_SETTEXT, 0, (LPARAM)L"Opciones de filtros");
	EnableWindow(GetDlgItem(ventanaActual, IDC_FILTROCOMBO), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_FILTROOTRO), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_VISIBLE), false);
	SendDlgItemMessage(ventanaActual, IDC_VISIBLE, BM_SETCHECK, BST_UNCHECKED, 0);

}

void editFiltro(HWND &lista, int id) {

	if (id < 0)
		return;

	selectedFiltro = filtros.at(id);

	SendDlgItemMessage(ventanaActual, IDC_NAMEFILTRO, WM_SETTEXT, 0, (WPARAM)selectedFiltro->name.c_str());

	SendDlgItemMessage(ventanaActual, IDC_FILTROCOMBO, WM_CLEAR, 0, 0);
	SendDlgItemMessage(ventanaActual, IDC_FILTROCOMBO, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(ventanaActual, IDC_FILTROOTRO, WM_CLEAR, 0, 0);
	SendDlgItemMessage(ventanaActual, IDC_FILTROOTRO, CB_RESETCONTENT, 0, 0);

	selectedFiltro->setComboBox(GetDlgItem(ventanaActual, IDC_FILTROCOMBO), GetDlgItem(ventanaActual, IDC_OP1));
	selectedFiltro->setOtherCombo(GetDlgItem(ventanaActual, IDC_FILTROOTRO), GetDlgItem(ventanaActual, IDC_OP2));

	SendDlgItemMessage(ventanaActual, IDC_VISIBLE, BM_SETCHECK, (selectedFiltro->turnedOff) ? BST_UNCHECKED : BST_CHECKED, 0);

	EnableWindow(GetDlgItem(ventanaActual, IDC_VISIBLE), true);	
}

void changeTurnedOff() {
	if (selectedFiltro) {
		selectedFiltro->turnedOff = !selectedFiltro->turnedOff;
	}
}

void disableAll() {
	EnableWindow(GetDlgItem(ventanaActual, IDC_COMBOFILTROS), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_ADDFILTRO), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_FILTERUP), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_FILTERDOWN), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_DELETEFILTRO), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_TURNFILTERS), false);
}

void enableAll() {
	EnableWindow(GetDlgItem(ventanaActual, IDC_COMBOFILTROS), true);
	EnableWindow(GetDlgItem(ventanaActual, IDC_ADDFILTRO), true);
	EnableWindow(GetDlgItem(ventanaActual, IDC_FILTERUP), true);
	EnableWindow(GetDlgItem(ventanaActual, IDC_FILTERDOWN), true);
	EnableWindow(GetDlgItem(ventanaActual, IDC_DELETEFILTRO), true);
	EnableWindow(GetDlgItem(ventanaActual, IDC_TURNFILTERS), true);
}

void trulyDisableAll() {
	disableAll();
	EnableWindow(GetDlgItem(ventanaActual, IDC_PAUSEVIDEO), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_RESUMEVIDEO), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_RESETVIDEO), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_COMBOFILTROS), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_FILTROLISTA), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_FILTROCOMBO), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_FILTROOTRO), false);
	EnableWindow(GetDlgItem(ventanaActual, IDC_VISIBLE), false);

}

void trulyEnableAll() {
	enableAll();
	EnableWindow(GetDlgItem(ventanaActual, IDC_PAUSEVIDEO), true);
	EnableWindow(GetDlgItem(ventanaActual, IDC_RESUMEVIDEO), true);
	EnableWindow(GetDlgItem(ventanaActual, IDC_RESETVIDEO), true);
	EnableWindow(GetDlgItem(ventanaActual, IDC_COMBOFILTROS), true);
	EnableWindow(GetDlgItem(ventanaActual, IDC_FILTROLISTA), true);
	EnableWindow(GetDlgItem(ventanaActual, IDC_FILTROCOMBO), true);
	EnableWindow(GetDlgItem(ventanaActual, IDC_FILTROOTRO), true);
	EnableWindow(GetDlgItem(ventanaActual, IDC_VISIBLE), true);
}