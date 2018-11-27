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

class CDFHistograma{

public:
	uchar pixel=0;
	int CdF=0;
	uchar newPixel=0;
};

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

	name = L"Histograma";
	setOption(0, 0);

	comboBox = true;
	comboName = L"Tipo:";
	comboBoxValues.push_back(L"Histograma normal");
	comboBoxValues.push_back(L"Ecualizacion Simple");
	comboBoxValues.push_back(L"Ecualizacion Uniforme");
	comboBoxValues.push_back(L"Ecualizacion Exponencial");
}

Histograma::~Histograma() {

}

void Histograma::getFilteredImg(Mat frame, Mat &out) {
	if (turnedOff)
		return;

	switch (type)
	{
	case 0: {
		/*
		Mat bgr[3];
		split(frame, bgr);

		Mat bgrEqua[3];
		for (int i = 0; i < 3; i++) {
			equalizeHist(bgr[i], bgrEqua[i]);
		}
		vector<Mat> channels;
		channels.push_back(bgrEqua[0]);
		channels.push_back(bgrEqua[1]);
		channels.push_back(bgrEqua[2]);
		merge(channels, out);
		*/

		histogramaShit(frame, out);
		break;
	}
	case 1: {
		/*
		Mat bgr[3];
		split(frame, bgr);

		Mat bgrEqua[3];
		for (int i = 0; i < 3; i++) {
			equalizeHist(bgr[i], bgrEqua[i]);
		}
		vector<Mat> channels;
		channels.push_back(bgrEqua[0]);
		channels.push_back(bgrEqua[1]);
		channels.push_back(bgrEqua[2]);
		merge(channels, out);
		break;
		*/
		histogramaSimpleShit(frame, out);
		break;
		
	}
	case 2:
		histogramaUniformeShit(frame, out);
		break;
	case 3:
		histogramaExponencialShit(frame, out);
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