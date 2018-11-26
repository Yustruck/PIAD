#pragma once
class FilterBase
{
public:
	FilterBase();
	~FilterBase();

	bool turnedOff = false;

	wstring name = L"";
	wstring comboName = L"";
	bool comboBox = false;
	int comboValue;
	vector<wstring> comboBoxValues;
	
	wstring otherComboName = L"";
	bool otherCombo = false;
	int otherComboValue;
	vector<wstring> otherComboValues;

	virtual void getFilteredImg(Mat frame, Mat &out) = 0;
	virtual void setOption(int num, int value) = 0;
	void setComboBox(HWND combo, HWND staticText);
	void setOtherCombo(HWND other, HWND staticText);
};

void FilterBase::setComboBox(HWND combo, HWND staticText) {
	if (comboBox) {
		EnableWindow(combo, true);
		for (wstring &value : comboBoxValues)
			SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)value.c_str());

		SendMessage(combo, CB_SETCURSEL, comboValue, 0);
		SendMessage(staticText, WM_SETTEXT, 0, (LPARAM)comboName.c_str());
	}
	else {
		EnableWindow(combo, false);
		SendMessage(staticText, WM_SETTEXT, 0, (LPARAM)L"No opcion disponible...");
	}
}

void FilterBase::setOtherCombo(HWND other, HWND staticText) {
	if (otherCombo) {
		EnableWindow(other, true);

		for (wstring &value : otherComboValues)
			SendMessage(other, CB_ADDSTRING, 0, (LPARAM)value.c_str());

		SendMessage(other, CB_SETCURSEL, comboValue, 0);
		SendMessage(staticText, WM_SETTEXT, 0, (LPARAM)otherComboName.c_str());
	}
	else {
		EnableWindow(other, false);
		SendMessage(staticText, WM_SETTEXT, 0, (LPARAM)L"No disponible...");
	}
}

FilterBase::FilterBase()
{
}

FilterBase::~FilterBase()
{
}
