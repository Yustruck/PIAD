#pragma once
//Shit

class Globales :
	public FilterBase
{
public:
	Globales();
	~Globales();

	void getFilteredImg(Mat frame, Mat &out);
	void setOption(int num, int value);
};

Globales::Globales()
{
}

Globales::~Globales()
{
}

void Globales::getFilteredImg(Mat frame, Mat &out) {
	if (!turnedOff)
		toSepia(frame, out);
}

void Globales::setOption(int num, int value) {

}

class Histograma :
	public FilterBase {
public:
	Histograma();
	~Histograma();

	int type = 0;

	void getFilteredImg(Mat frame, Mat &out);
	void setOption(int num, int value);

};

Histograma::Histograma() {

	name = L"Blanco y Negro";
	setOption(0, 0);

	comboBox = true;
	comboName = L"Tipo:";
	comboBoxValues.push_back(L"Histograma normal");
	comboBoxValues.push_back(L"Ecualizacion Simple");
	comboBoxValues.push_back(L"Ecualizacion Exponencial");
}

Histograma::~Histograma() {

}

void Histograma::getFilteredImg(Mat frame, Mat &out) {
	if (turnedOff)
		return;

	switch (type)
	{
	case 0:
		histogramaMat(frame, out);
		break;
	case 1:
		histogramaEqSimple(frame, out);
		break;
	case 2:
		//histogramaEqExp(frame, out);
		break;
	default:
		break;
	}
}

void Histograma::setOption(int num, int value) {
	switch (num)
	{
		case 0:
		{
			type = value;
			comboValue = value;
			break;
		}
		default:
			break;
	}
}