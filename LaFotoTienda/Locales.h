#pragma once
//Media

class Media : public FilterBase
{
public:
	Media();
	~Media();

	void getFilteredImg(Mat frame, Mat &out);
	void setOption(int num, int value);

private:
	char *charMat;
	Mat matriz;
	int blurMatSize;

};

Media::Media()
{
	name = L"Media";
	setOption(0, 0);

	comboBox = true;
	comboName = L"Intensidad:";
	comboBoxValues.push_back(L"3");
	comboBoxValues.push_back(L"4");
	comboBoxValues.push_back(L"5");
	comboBoxValues.push_back(L"6");
	comboBoxValues.push_back(L"7");

	/*
	otherCombo = true;
	otherComboName = L"Intensidad:";
	otherComboValues.push_back(L"1");
	otherComboValues.push_back(L"2");
	otherComboValues.push_back(L"3");
	otherComboValues.push_back(L"4");
	otherComboValues.push_back(L"5");
	*/
}

Media::~Media()
{
	delete[] charMat;
}

void Media::setOption(int num, int value) {
	switch (num)
	{
	case 0: {
		blurMatSize = value +3;
		comboValue = value;

		if (charMat)
			delete[] charMat;
		charMat = new char[blurMatSize*blurMatSize];
		for (int i = 0; i < blurMatSize*blurMatSize; i++)
			charMat[i] = 1;

		Mat otro(blurMatSize, blurMatSize, CV_8S, charMat);

		matriz = otro.clone();
	}
		break;
	default:
		break;
	}
}

void Media::getFilteredImg(Mat frame, Mat &out) {
	if (turnedOff)
		return;

	useThisMat(frame, out, matriz, 0, blurMatSize, blurMatSize);
}

//Media

class MediaPon : public FilterBase
{
public:
	MediaPon();
	~MediaPon();

	void getFilteredImg(Mat frame, Mat &out);
	void setOption(int num, int value);

private:
	char *charMat;
	Mat matriz;
	int blurMatSize;

};

MediaPon::MediaPon()
{
	name = L"Media Ponderada";
	setOption(0, 0);

	comboBox = true;
	comboName = L"Intensidad:";
	comboBoxValues.push_back(L"3");
	comboBoxValues.push_back(L"4");
	comboBoxValues.push_back(L"5");
	comboBoxValues.push_back(L"6");
	comboBoxValues.push_back(L"7");

	/*
	otherCombo = true;
	otherComboName = L"Intensidad:";
	otherComboValues.push_back(L"1");
	otherComboValues.push_back(L"2");
	otherComboValues.push_back(L"3");
	otherComboValues.push_back(L"4");
	otherComboValues.push_back(L"5");
	*/
}

MediaPon::~MediaPon()
{
	delete[] charMat;
}

void MediaPon::setOption(int num, int value) {
	switch (num)
	{
	case 0: {
		blurMatSize = value + 3;
		comboValue = value;

		if (charMat)
			delete[] charMat;
		charMat = new char[blurMatSize*blurMatSize];
		for (int i = 0; i < blurMatSize*blurMatSize; i++)
			charMat[i] = 1;

		switch(blurMatSize) 
		{
		case 3:
		{
			charMat[4] = 2;
		}
		break;

		case 4:
		{ //5, 6, 9, 10
			//charMat[5] = 2;
			charMat[6] = 2;
			//charMat[9] = 2;
			//charMat[10] = 2;
		}
		break;

		case 5:
		{
			charMat[7] = 2;
		}
		break;

		case 6:
		{
			charMat[8] = 2;
		}
		break;

		case 7:
		{
			charMat[10] = 2;
		}
		break;
		}

		Mat otro(blurMatSize, blurMatSize, CV_8S, charMat);

		matriz = otro.clone();
	}
			break;
	default:
		break;
	}
}

void MediaPon::getFilteredImg(Mat frame, Mat &out) {
	if (turnedOff)
		return;

	useThisMat(frame, out, matriz, 0, blurMatSize, blurMatSize);
}

//Detector de Orilla

class DetectorDeOrilla : public FilterBase
{
public:
	DetectorDeOrilla();
	~DetectorDeOrilla();

	void getFilteredImg(Mat frame, Mat &out);
	void setOption(int num, int value);

private:
	int type;
	Mat matriz;

};

DetectorDeOrilla::DetectorDeOrilla()
{
	name = L"Detector de Orillas";
	setOption(0, 0);

	comboBox = true;
	comboName = L"Tipo:";
	comboBoxValues.push_back(L"1");
	comboBoxValues.push_back(L"2");
	comboBoxValues.push_back(L"Laprasiano");
	comboBoxValues.push_back(L"C");
	comboBoxValues.push_back(L"F");
}

DetectorDeOrilla::~DetectorDeOrilla()
{

}

void DetectorDeOrilla::setOption(int num, int value) {
	switch (num)
	{
	case 0:
		type = value;
		comboValue = value;

		switch (type)
		{
		case 0:
		{
			char charMat[9] = {
				-1, -1, -1,
				-1, 8, -1,
				-1, -1, -1
			};
			Mat other(3, 3, CV_8S, charMat);
			matriz = other.clone();
		}
		break;
		case 1:
		{
			char aaa[9] = {
				1, 0, -1,
				0, 0, 0,
				-1, 0, 1
			};
			Mat other(3, 3, CV_8S, aaa);
			matriz = other.clone();
		}
		break;
		case 2: //Laprasiano
		{
			char aaa[9] = {
				0, 1, 0,
				1, -4, 1,
				0, 1, 0
			};

			Mat other(3, 3, CV_8S, aaa);
			matriz = other.clone();
		}
		break;
		case 3:
		{
			char aaa[9] = {
				-1, 0, +1,
				-2, 0, +2,
				-1, 0, +1
			};
			Mat other(3, 3, CV_8S, aaa);
			matriz = other.clone();
		}
		break;
		case 4:
		{
			char aaa[9] = {
				-1, -2, -1,
				0, 0, 0,
				1, 2, 1
			};
			Mat other(3, 3, CV_8S, aaa);
			matriz = other.clone();
		}
		break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void DetectorDeOrilla::getFilteredImg(Mat frame, Mat &out) {
	if (turnedOff)
		return;

	useThisMat(frame, out, matriz, 0, 3, 3);
}


//Sharp

class Sharp : public FilterBase
{
public:
	Sharp();
	~Sharp();

	void getFilteredImg(Mat frame, Mat &out);
	void setOption(int num, int value);

private:
	int type;
	Mat matriz;

};

Sharp::Sharp()
{
	name = L"Sharp";
	setOption(0, 0);

	comboBox = true;
	comboName = L"Tipo:";
	comboBoxValues.push_back(L"Menos Laprasiano");
}

Sharp::~Sharp()
{

}

void Sharp::setOption(int num, int value) {
	switch (num)
	{
	case 0:
		type = value;
		comboValue = value;

		switch (type)
		{
		case 0: //Menos Laplaciano
		{
			char aaa[9] = {
				0, -1, 0,
				-1, 5, -1,
				0, -1, 0
			};

			Mat other(3, 3, CV_8S, aaa);
			matriz = other.clone();
		}
		break;
		default:
			break;
		}
	}
}

void Sharp::getFilteredImg(Mat frame, Mat &out) {
	if (turnedOff)
		return;

	useThisMat(frame, out, matriz, 0, 3, 3);
}


//Direction

class Direction : public FilterBase
{
public:
	Direction();
	~Direction();

	void getFilteredImg(Mat frame, Mat &out);
	void setOption(int num, int value);

private:
	int type;
	Mat matriz;

};

Direction::Direction()
{
	name = L"Direction";
	setOption(0, 0);

	comboBox = true;
	comboName = L"Direccion:";
	comboBoxValues.push_back(L"Norte");
	comboBoxValues.push_back(L"Sur");
	comboBoxValues.push_back(L"Este");
	comboBoxValues.push_back(L"Oeste");
}

Direction::~Direction()
{

}

void Direction::setOption(int num, int value) {
	switch (num)
	{
	case 0:
		type = value;
		comboValue = value;

		switch (type)
		{
		case 0:
		{
			char aaa[9] = {
				1, 1, 1,
				1, -2, 1,
				-1, -1, -1
			};

			Mat other(3, 3, CV_8S, aaa);
			matriz = other.clone();
		}
		break;
		case 1:
		{
			char aaa[9] = {
				-1, -1, -1,
				1, -2, 1,
				1, 1, 1
			};

			Mat other(3, 3, CV_8S, aaa);
			matriz = other.clone();
		}
		break;
		case 2:
		{
			char aaa[9] = {
				-1, 1, 1,
				-1, -2, 1,
				-1, 1, 1
			};

			Mat other(3, 3, CV_8S, aaa);
			matriz = other.clone();
		}
		break;
		case 3:
		{
			char aaa[9] = {
				1, 1, -1,
				1, -2, -1,
				1, 1, -1
			};

			Mat other(3, 3, CV_8S, aaa);
			matriz = other.clone();
		}
		break;

		default:
			break;
		}
	}
}

void Direction::getFilteredImg(Mat frame, Mat &out) {
	if (turnedOff)
		return;

	useThisMat(frame, out, matriz, 0, 3, 3);
}


//Mediana de mierda

class Mediana : public FilterBase
{
public:
	Mediana();
	~Mediana();

	void getFilteredImg(Mat frame, Mat &out);
	void setOption(int num, int value);

private:
	int blurMatSize;

};

Mediana::Mediana()
{
	name = L"Mediana";
	setOption(0, 0);

	comboBox = true;
	comboName = L"Intensidad:";
	comboBoxValues.push_back(L"3");
	comboBoxValues.push_back(L"4");
	comboBoxValues.push_back(L"5");
	comboBoxValues.push_back(L"6");
	comboBoxValues.push_back(L"7");
}

Mediana::~Mediana()
{
}

void Mediana::setOption(int num, int value) {
	switch (num)
	{
	case 0: {
		blurMatSize = value+3;
		comboBox = value;
	}
			break;
	default:
		break;
	}
}

void Mediana::getFilteredImg(Mat frame, Mat &out) {
	if (turnedOff)
		return;

	filtroMediana(frame, out, blurMatSize, blurMatSize);
}


//Gaussiano

class Gaussiano : public FilterBase
{
public:
	Gaussiano();
	~Gaussiano();

	float sigma = 0.5;
	float matSize = 3;

	void getFilteredImg(Mat frame, Mat &out);
	void setOption(int num, int value);

private:
	Mat matriz;
};

Gaussiano::Gaussiano()
{
	name = L"Gaussiano";
	setOption(0, 0);
	setOption(1, 0);
	
	comboBox = true;
	comboName = L"Intensidad 1:";
	comboBoxValues.push_back(L"2");
	comboBoxValues.push_back(L"2.5");
	comboBoxValues.push_back(L"3");
	

	
	otherCombo = true;
	otherComboName = L"Intensidad 2:";
	otherComboValues.push_back(L"3");
	otherComboValues.push_back(L"5");
	
}

Gaussiano::~Gaussiano()
{
}

void Gaussiano::setOption(int num, int value) {
	switch (num)
	{
	case 0: 
	{
		sigma = (value*0.5) + 2;
		matriz = makeGaussianoMat(sigma, matSize);
	}
	break;
	case 1:
	{
		matSize = (value*2.0) + 3;
		matriz = makeGaussianoMat(sigma, matSize);
	}
	break;
	default:
		break;
	}
}

void Gaussiano::getFilteredImg(Mat frame, Mat &out) {
	if (turnedOff)
		return;

	useThisMat(frame, out, matriz, 0, matSize, matSize);
}