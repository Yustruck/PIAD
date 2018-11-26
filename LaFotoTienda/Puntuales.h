#pragma once
//BnW

class BlancoYNegro :
	public FilterBase
{
public:
	BlancoYNegro();
	~BlancoYNegro();

	int type = 0;
	
	void getFilteredImg(Mat frame, Mat &out);
	void setOption(int num, int value);
};

BlancoYNegro::BlancoYNegro()
{
	name = L"Blanco y Negro";
	setOption(0, 0);

	comboBox = true;
	comboName = L"Tipo:";
	comboBoxValues.push_back(L"Promedio");
	comboBoxValues.push_back(L"Iluminacion");
	comboBoxValues.push_back(L"Desaturacion");

}

BlancoYNegro::~BlancoYNegro()
{
}

void BlancoYNegro::getFilteredImg(Mat frame, Mat &out) {
	if (turnedOff)
		return;

	switch (type)
	{
	case 0:
		toGray(frame, out);
		break;
	case 1:
		toGray1(frame, out);
		break;
	case 2:
		toGray2(frame, out);
		break;
	default:
		break;
	}
}

void BlancoYNegro::setOption(int num, int value) {
	switch (num)
	{
	case 0:
		type = value;
		comboValue = value;
	default:
		break;
	}
}


//Sepia

class Sepia :
	public FilterBase
{
public:
	Sepia();
	~Sepia();

	void getFilteredImg(Mat frame, Mat &out);
	void setOption(int num, int value);
};

Sepia::Sepia()
{
	name = L"Sepia";
}

Sepia::~Sepia()
{
}

void Sepia::getFilteredImg(Mat frame, Mat &out) {
	if (!turnedOff)
		toSepia(frame, out);
}

void Sepia::setOption(int num, int value) {
	
}