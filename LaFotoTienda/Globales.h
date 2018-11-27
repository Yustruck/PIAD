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

	Histograma();
	~Histograma();

	void getFilteredImg(Mat frame, Mat &out);
	void setOption(int num, int value);

private:
	Mat matriz;
};

Histograma::Histograma() {

	name = L"Histograma";
	setOption(0, 0);

}

Histograma::~Histograma() {

}

void Histograma::getFilteredImg(Mat frame, Mat &out) {
	if (turnedOff)
		return;

	useThisMat(frame, out, matriz, 0, 3, 3);
}

void Histograma::setOption(int num, int value) {
	switch (num)
	{
	case 0:
	{

			char tmpMat[9] = {
				0, -1, 0,
				-1, 5, -1,
				0, -1, 0
			};

			Mat other(3, 3, CV_8S, tmpMat);
			matriz = other.clone();
		}
		break;
	default:
		break;
		
	}
}