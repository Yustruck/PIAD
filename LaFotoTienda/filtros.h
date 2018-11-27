#pragma once

struct myclass {
	bool operator() (uchar i, uchar j) { return (i<j); }
} myobject;


void toGray(Mat frame, Mat &out) {
	out = frame.clone();

	int channels = frame.channels();

	int nRows = frame.rows;
	//las columnas efectivas de la imagen
	int nCols = frame.cols * channels;
	
	int i, j, k = 0;
	//punteros para manejar a la imagen
	uchar *p, *q;
	for (i = 0; i < nRows; ++i)
	{
		p = frame.ptr<uchar>(i);
		q = out.ptr<uchar>(i);
		
		for (j = 0; j < nCols; j += 3)
		{
			uchar value = (p[j] + p[j + 1] + p[j + 2]) / 3;
			q[j + 0] = value;
			q[j + 1] = value;
			q[j + 2] = value;
		}
	}
}


void toGray1(Mat frame, Mat &out) {
	out = frame.clone();

	int channels = frame.channels();

	int nRows = frame.rows;
	//las columnas efectivas de la imagen
	int nCols = frame.cols * channels;

	int i, j, k = 0;
	//punteros para manejar a la imagen
	uchar *p, *q;
	for (i = 0; i < nRows; ++i)
	{
		p = frame.ptr<uchar>(i);
		q = out.ptr<uchar>(i);

		for (j = 0; j < nCols; j += 3)
		{
			int value = p[j + 2] * 0.2126f + p[j + 1] * 0.7512f + p[j] * 0.0722f;

			if (value > 255)
				value = 255;
			if (value < 0)
				value = 0;

			q[j + 0] = value;
			q[j + 1] = value;
			q[j + 2] = value;
		}
	}
}

uchar Max(uchar one, uchar two, uchar three) {
	uchar returne=0;
	if (one > returne) {
		returne = one;
	}
	if (two > returne) {
		returne = two;
	}
	if (three > returne) {
		returne = three;
	}

	return returne;
}
uchar Min(uchar one, uchar two, uchar three) {
	uchar returne = 255;
	if (one < returne) {
		returne = one;
	}
	if (two < returne) {
		returne = two;
	}
	if (three < returne) {
		returne = three;
	}

	return returne;
}
void toGray2(Mat frame, Mat &out) {
	out = frame.clone();

	int channels = frame.channels();

	int nRows = frame.rows;
	//las columnas efectivas de la imagen
	int nCols = frame.cols * channels;

	int i, j, k = 0;
	//punteros para manejar a la imagen
	uchar *p, *q;
	for (i = 0; i < nRows; ++i)
	{
		p = frame.ptr<uchar>(i);
		q = out.ptr<uchar>(i);

		for (j = 0; j < nCols; j += 3)
		{
			uchar value = (Max(p[j], p[j + 1], p[j + 2]) + Min(p[j], p[j + 1], p[j + 2]) )/ 2;

			q[j + 0] = value;
			q[j + 1] = value;
			q[j + 2] = value;
		}
	}
}


void toSepia(Mat frame, Mat &out) {
	out = frame.clone();

	int channels = frame.channels();

	int nRows = frame.rows;
	//las columnas efectivas de la imagen
	int nCols = frame.cols * channels;

	int i, j, k = 0;
	//punteros para manejar a la imagen
	uchar *p, *q;
	for (i = 0; i < nRows; ++i)
	{
		p = frame.ptr<uchar>(i);
		q = out.ptr<uchar>(i);

		for (j = 0; j < nCols; j += 3)
		{
			int r = p[j + 2] * .393 + p[j + 1] * .769 + p[j] * .189;
			int g = p[j + 2] * .349 + p[j + 1] * .686 + p[j] * .168;
			int b = p[j + 2] * .272 + p[j + 1] * .534 + p[j] * .131;

			if (r > 255)
				r = 255;
			if (r < 0)
				r = 0;

			if (g > 255)
				g = 255;
			if (g < 0)
				g = 0;

			if (b > 255)
				b = 255;
			if (b < 0)
				b = 0;

			q[j + 0] = b;
			q[j + 1] = g;
			q[j + 2] = r;
		}
	}
}


void useThisMat(Mat frame, Mat &out, Mat mat, char matSum = 0, char matX = 0, char matY = 0) {
	out = frame.clone();

	int channels = frame.channels();
	int nRows = frame.rows;
	//las columnas efectivas de la imagen
	int nCols = frame.cols * channels;


	if (matSum == 0) {

		for (int i = 0; i < matY; i++)
			for (int j = 0; j < matX; j++)
				matSum += mat.ptr<char>(i)[j]; //mat[i][j];
		
		if (matSum < 1)
			matSum = 1;
	}

	int i, j, k = 0;

	//punteros para manejar a la imagen
	uchar **p = new uchar*[matY];
	uchar *safeP;
	uchar *q;

	int matYHalf = matY / 2;
	int matXHalf = matX / 2;

	for (i = 0; i < nRows; ++i)
	{
		int rowGet = i-matYHalf;
		for (int ix = 0; ix < matY; ix++) {
			p[ix] = (rowGet>0 && rowGet<nRows-matYHalf) ? frame.ptr<uchar>(rowGet) : NULL;
			rowGet++;
		}
		bool pIsNotNull = true;

		for (int we = 0; we < matY; we++)
			if (p[we] == NULL)
				pIsNotNull = false;

		safeP = frame.ptr<uchar>(i);
		q = out.ptr<uchar>(i);

		for (j = 0; j < nCols; j += 3)
		{			
			if (pIsNotNull && j>0 && j<nCols - 3) {
				uchar value[3];

				for (k = 0; k < 3; k++) {
					int val = 0;

					for(int we = 0; we < matY; we++)
						for (int oof = 0; oof < matX; oof++) {
							val += p[we][j + k + (-3*matXHalf) + (3*oof)] * mat.ptr<char>(we)[oof];
						}

					val = val / matSum;

					if (val > 255)
						value[k] = 255;
					else
						if (val < 0)
							value[k] = 0;
						else
							value[k] = val;
				}

				q[j + 0] = value[0];
				q[j + 1] = value[1];
				q[j + 2] = value[2];
			}
			else {
				q[j + 0] = safeP[j + 0];
				q[j + 1] = safeP[j + 1];
				q[j + 2] = safeP[j + 2];
			}
		}
	}

	for (i = 0; i < matY; i++) {
		p[i] = NULL;
	}
	delete[] p;
}



void filtroMediana(Mat frame, Mat &out, char matX = 0, char matY = 0) {
	out = frame.clone();

	int channels = frame.channels();
	int nRows = frame.rows;
	int nCols = frame.cols * channels;

	int i, j, k = 0;

	uchar **p = new uchar*[matY];
	uchar *safeP;
	uchar *q;

	int matYHalf = matY / 2;
	int matXHalf = matX / 2;

	for (i = 0; i < nRows; ++i)
	{
		int rowGet = i - matYHalf;
		for (int ix = 0; ix < matY; ix++) {
			p[ix] = (rowGet>0 && rowGet<nRows - matYHalf) ? frame.ptr<uchar>(rowGet) : NULL;
			rowGet++;
		}
		bool pIsNotNull = true;

		for (int we = 0; we < matY; we++)
			if (p[we] == NULL)
				pIsNotNull = false;

		safeP = frame.ptr<uchar>(i);
		q = out.ptr<uchar>(i);

		for (j = 0; j < nCols; j += 3)
		{
			if (pIsNotNull && j>0 && j<nCols - 3) {
				uchar value[3];

				for (k = 0; k < 3; k++) {
					int val = 0;

					vector<uchar> losJajas;

					//Este metodo es extremadamento lento, mejorar luego

					for (int we = 0; we < matY; we++)
						for (int oof = 0; oof < matX; oof++) {
							losJajas.push_back(p[we][j + k + (-3 * matXHalf) + (3 * oof)]);
						}

					std::sort(losJajas.begin(), losJajas.end(), myobject);

					//val = val / matSum;

					val = losJajas[((matX*matY)/2)-1];
					

					if (val > 255)
						value[k] = 255;
					else
						if (val < 0)
							value[k] = 0;
						else
							value[k] = val;
				}

				q[j + 0] = value[0];
				q[j + 1] = value[1];
				q[j + 2] = value[2];
			}
			else {
				q[j + 0] = safeP[j + 0];
				q[j + 1] = safeP[j + 1];
				q[j + 2] = safeP[j + 2];
			}
		}
	}

	for (i = 0; i < matY; i++) {
		p[i] = NULL;
	}
	delete[] p;
}


Mat makeGaussianoMat(float sigma = 2.0, int matSize = 3) {

	double *w = new double[matSize*matSize];

	double negativeVal = floor(matSize / 2);

	double e = 2.718;
	double pi = 3.1415;
	double smallValue = 255000.0f;

	int i = 0;

	for (int s = 0; s < matSize; s++) {
		for (int t = 0; t < matSize; t++) {
			double value;
			value = pow(s- negativeVal, 2.0f) + pow(t- negativeVal, 2.0f);
			value = pow(e, - value / pow(sigma, 2.0f));
			value = value / (2.0f*pi*sigma);
			w[i] = value;
			if (smallValue > value)
				smallValue = value;

			i++;
		}
	}

	char *aaa = new char[matSize*matSize];

	i = 0;

	for (int s = 0; s < matSize; s++) {
		for (int t = 0; t < matSize; t++) {
			aaa[i] = w[i] / smallValue;
			i++;
		}
	}

	Mat other(matSize, matSize, CV_8S, aaa);
	Mat ooo = other.clone();

	delete[] aaa;
	delete[] w;
	return ooo.clone();
}


string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}


wstring s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

void histogramaMat(Mat frame, Mat &out) {
	out = frame.clone();

	int channels = frame.channels();

	int nRows = frame.rows;
	//las columnas efectivas de la imagen
	int nCols = frame.cols * channels;

	int i, j, k = 0;
	int histoSize = 0;
	uchar *histoR, *histoG, *histoB;
	float valueR, valueG, valueB = 0;
	//punteros para manejar a la imagen
	uchar *p, *q;
	for (i = 0; i < nRows; ++i)
	{
		p = frame.ptr<uchar>(i);
		q = out.ptr<uchar>(i);

		for (j = 0; j < nCols; j += 3)
		{
			histoB[histoSize] = p[j];
			histoG[histoSize] = p[j + 1];
			histoR[histoSize] = p[j + 2];
			histoSize++;
		}
	}
	heapify(histoR, histoSize, 0);
	heapify(histoG, histoSize, 0);
	heapify(histoB, histoSize, 0);
	int tmp;
	for (i = 0; i < nRows; ++i)
	{

		for (j = 0; j < nCols; j += 3)
		{
			float v1 = histoR[tmp] - histoR[0];
			float v2 = tmp - histoR[0];
			valueR = v1 / v2 * 255;

			float v1 = histoG[tmp] - histoG[0];
			float v2 = tmp - histoG[0];
			valueG = v1 / v2 * 255;

			float v1 = histoB[tmp] - histoB[0];
			float v2 = tmp - histoB[0];
			valueB = v1 / v2 * 255;
			tmp++;

			q[j + 0] = (int)valueR;
			q[j + 1] = (int)valueG;
			q[j + 2] = (int)valueB;
		}
	}

}

void histogramaEqSimple(Mat frame, Mat &out) {
	out = frame.clone();

	int channels = frame.channels();

	int nRows = frame.rows;
	//las columnas efectivas de la imagen
	int nCols = frame.cols * channels;

	int i, j, k = 0;
	int histoSize = 0;
	uchar *histoR, *histoG, *histoB;
	int valueR, valueG, valueB = 0;
	//punteros para manejar a la imagen
	uchar *p, *q;
	for (i = 0; i < nRows; ++i)
	{
		p = frame.ptr<uchar>(i);
		q = out.ptr<uchar>(i);

		for (j = 0; j < nCols; j += 3)
		{
			histoB[histoSize] = p[j];
			histoG[histoSize] = p[j + 1];
			histoR[histoSize] = p[j + 2];
			histoSize++;
		}
	}
	heapify(histoR, histoSize, 0);
	heapify(histoG, histoSize, 0);
	heapify(histoB, histoSize, 0);
	int tmp;

	float nkR, nkG, nkB = 1;
	for (i = 0; i < nRows; ++i)
	{

		for (j = 0; j < nCols; j += 3)
		{
			float v1 = histoR[histoSize] - histoR[0];
			for (m = 0; m < histoSize; m++) {
				if (histoR[tmp] == histoR[tmp + 1]) {
					nkR++;
				}
				float v2 = histoSize / nkR;
				float v3 = v2 + histoR[0];
				valueR = v1 * v3;
			}

			float v1 = histoG[histoSize] - histoG[0];
			for (m = 0; m < histoSize; m++) {
				if (histoG[tmp] == histoG[tmp + 1]) {
					nkG++;
				}
				float v2 = histoSize / nkG;
				float v3 = v2 + histoG[0];
				valueG = v1 * v3;
			}

			float v1 = histoB[histoSize] - histoB[0];
			for (m = 0; m < histoSize; m++) {
				if (histoB[tmp] == histoB[tmp + 1]) {
					nkB++;
				}
				float v2 = histoSize / nkB;
				float v3 = v2 + histoB[0];
				valueB = v1 * v3;
			}
			tmp++;

			q[j + 0] = valueR;
			q[j + 1] = valueG;
			q[j + 2] = valueB;
		}
	}

}

void heapify(int arr[], int n, int i)
{
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;

	if (l < n && arr[l] > arr[largest])
		largest = l;

	if (r < n && arr[r] > arr[largest])
		largest = r;

	if (largest != i)
	{
		swap(arr[i], arr[largest]);

		heapify(arr, n, largest);
	}
}