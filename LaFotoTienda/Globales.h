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