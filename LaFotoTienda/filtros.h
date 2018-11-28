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

					unsigned char *deVerdad = new unsigned char[matY*matX];
					int deDeveras = 0;

					for (int we = 0; we < matY; we++)
						for (int oof = 0; oof < matX; oof++) {
							deVerdad[deDeveras] = p[we][j + k + (-3 * matXHalf) + (3 * oof)];
							deDeveras++;
						}

					heapSort(deVerdad, matY*matX);

					val = deVerdad[(int)((matX*matY) / 2)-1];

					delete[] deVerdad;

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

void histogramaShit(Mat frame, Mat &out) {
	out = frame.clone();

	int channels = frame.channels();

	int nRows = frame.rows;
	//las columnas efectivas de la imagen
	int nCols = frame.cols * channels;

	int i, j, k = 0;
	int histoSize = 0;
	unsigned char *histoR, *histoG, *histoB;
	int rowsandcols = frame.rows*frame.cols;
	histoR = new unsigned char[rowsandcols];
	histoG = new unsigned char[rowsandcols];
	histoB = new unsigned char[rowsandcols];

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

	heapSort(histoR, rowsandcols);
	heapSort(histoG, rowsandcols);
	heapSort(histoB, rowsandcols);

	vector<CDFHistograma> CDF_R;
	vector<CDFHistograma> CDF_G;
	vector<CDFHistograma> CDF_B;

	//OBTIENE TODOS LOS CDF
	for (i = 0; i < rowsandcols; i++) {
		CDFHistograma cdfActual;
		for (j = 0; j < CDF_R.size(); j++) {
			if (histoR[i] == CDF_R.at(j).pixel) {
				CDF_R.at(j).CdF++;
				cdfActual = CDF_R.at(j);
				break;
			}
		}
		if (cdfActual.CdF == 0) {

			cdfActual.CdF = 1;
			cdfActual.pixel = histoR[i];
			CDF_R.push_back(cdfActual);
		}
		cdfActual.CdF = 0;
		for (j = 0; j < CDF_B.size(); j++) {
			if (histoB[i] == CDF_B.at(j).pixel) {
				CDF_B.at(j).CdF++;
				cdfActual = CDF_B.at(j);
				break;
			}
		}
		if (cdfActual.CdF == 0) {
			cdfActual.CdF = 1;
			cdfActual.pixel = histoB[i];
			CDF_B.push_back(cdfActual);
		}

		cdfActual.CdF = 0;
		for (j = 0; j < CDF_G.size(); j++) {
			if (histoG[i] == CDF_G.at(j).pixel) {
				CDF_G.at(j).CdF++;
				cdfActual = CDF_G.at(j);
				break;
			}
		}
		if (cdfActual.CdF == 0)
		{
			cdfActual.CdF = 1;
			cdfActual.pixel = histoG[i];
			CDF_G.push_back(cdfActual);
		}
	}

	int minCdF_R = CDF_R.at(0).CdF;
	int minCdF_G = CDF_G.at(0).CdF;
	int minCdF_B = CDF_B.at(0).CdF;

	for (j = 0; j < CDF_R.size(); j++) {
		if (j == 0)
			continue;
		CDF_R.at(j).CdF += CDF_R.at(j-1).CdF;
	}
	for (j = 0; j < CDF_G.size(); j++) {
		if (j == 0)
			continue;
		CDF_G.at(j).CdF += CDF_G.at(j - 1).CdF;
	}
	for (j = 0; j < CDF_B.size(); j++) {
		if (j == 0)
			continue;
		CDF_B.at(j).CdF += CDF_B.at(j - 1).CdF;
	}

	int MaxCdF;
	MaxCdF = rowsandcols;
	MaxCdF -= minCdF_R;
	for (j = 0; j < CDF_R.size(); j++) {
		int CdF = CDF_R.at(j).CdF;
		CdF -= minCdF_R;
		double v = ((double)CdF / (double)MaxCdF)*255.0;
		CDF_R.at(j).newPixel = (uchar)floor(v);
	}

	MaxCdF = rowsandcols;
	MaxCdF -= minCdF_G;
	for (j = 0; j < CDF_G.size(); j++) {
		int CdF = CDF_G.at(j).CdF;
		CdF -= minCdF_G;
		double v = ((double)CdF / (double)MaxCdF)*255.0;
		CDF_G.at(j).newPixel = (uchar)floor(v);
	}

	MaxCdF = rowsandcols;
	MaxCdF -= minCdF_B;
	for (j = 0; j < CDF_B.size(); j++) {
		int CdF = CDF_B.at(j).CdF;
		CdF -= minCdF_B;
		double v = ((double)CdF / (double)MaxCdF)*255.0;
		CDF_B.at(j).newPixel = (uchar)floor(v);
	}

	int tmp = 0;
	double v1, v2, v3;

	for (i = 0; i < nRows; ++i)
	{
		p = frame.ptr<uchar>(i);
		q = out.ptr<uchar>(i);

		for (j = 0; j < nCols; j += 3)
		{
			CDFHistograma cdfR;
			CDFHistograma cdfG;
			CDFHistograma cdfB;

			for (int k = 0; k < CDF_R.size(); k++) {
				if (p[j + 2] == CDF_R.at(k).pixel)
					cdfR = CDF_R.at(k);
			}
			for (int k = 0; k < CDF_G.size(); k++) {
				if (p[j + 1] == CDF_G.at(k).pixel)
					cdfG = CDF_G.at(k);
			}
			for (int k = 0; k < CDF_B.size(); k++) {
				if (p[j] == CDF_B.at(k).pixel)
					cdfB = CDF_B.at(k);
			}

			q[j + 0] = cdfB.newPixel;
			q[j + 1] = cdfG.newPixel;
			q[j + 2] = cdfR.newPixel;
		}
	}

	delete[] histoR;
	delete[] histoG;
	delete[] histoB;
}

void histogramaSimpleShit(Mat frame, Mat &out) {
	out = frame.clone();

	int channels = frame.channels();

	int nRows = frame.rows;
	//las columnas efectivas de la imagen
	int nCols = frame.cols * channels;

	int i, j, k = 0;
	int histoSize = 0;
	unsigned char *histoR, *histoG, *histoB;
	int rowsandcols = frame.rows*frame.cols;
	histoR = new unsigned char[rowsandcols];
	histoG = new unsigned char[rowsandcols];
	histoB = new unsigned char[rowsandcols];

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

	heapSort(histoR, rowsandcols);
	heapSort(histoG, rowsandcols);
	heapSort(histoB, rowsandcols);

	vector<CDFHistograma> CDF_R;
	vector<CDFHistograma> CDF_G;
	vector<CDFHistograma> CDF_B;
	double minPr = 1.0 / (double)rowsandcols;

	//OBTIENE TODOS LOS CDF
	for (i = 0; i < rowsandcols; i++) {
		CDFHistograma cdfActual;
		for (j = 0; j < CDF_R.size(); j++) {
			if (histoR[i] == CDF_R.at(j).pixel) {
				CDF_R.at(j).CdF++;
				CDF_R.at(j).Pr += minPr;
				cdfActual = CDF_R.at(j);
				break;
			}
		}
		if (cdfActual.CdF == 0) {
			cdfActual.CdF = 1;
			cdfActual.pixel = histoR[i];
			cdfActual.Pr = minPr;
			CDF_R.push_back(cdfActual);
		}
		//Blue
		cdfActual.CdF = 0;
		for (j = 0; j < CDF_B.size(); j++) {
			if (histoB[i] == CDF_B.at(j).pixel) {
				CDF_B.at(j).CdF++;
				CDF_B.at(j).Pr += minPr;
				cdfActual = CDF_B.at(j);
				break;
			}
		}
		if (cdfActual.CdF == 0) {
			cdfActual.CdF = 1;
			cdfActual.pixel = histoB[i];
			cdfActual.Pr = minPr;
			CDF_B.push_back(cdfActual);
		}
		//Green
		cdfActual.CdF = 0;
		for (j = 0; j < CDF_G.size(); j++) {
			if (histoG[i] == CDF_G.at(j).pixel) {
				CDF_G.at(j).CdF++;
				CDF_G.at(j).Pr += minPr;
				cdfActual = CDF_G.at(j);
				break;
			}
		}
		if (cdfActual.CdF == 0)
		{
			cdfActual.CdF = 1;
			cdfActual.pixel = histoG[i];
			cdfActual.Pr = minPr;
			CDF_G.push_back(cdfActual);
		}
	}

	double PrRj = 0;
	for (j = 0; j < CDF_R.size(); j++) {
		PrRj += CDF_R.at(j).Pr;
		double v = PrRj * 255.0;
		CDF_R.at(j).newPixel = (uchar)floor(v);
	}

	PrRj = 0;
	for (j = 0; j < CDF_G.size(); j++) {
		PrRj += CDF_G.at(j).Pr;
		double v = PrRj * 255.0;
		CDF_G.at(j).newPixel = (uchar)floor(v);
	}

	PrRj = 0;
	for (j = 0; j < CDF_B.size(); j++) {
		PrRj += CDF_B.at(j).Pr;
		double v = PrRj * 255.0;
		CDF_B.at(j).newPixel = (uchar)floor(v);
	}

	int tmp = 0;
	double v1, v2, v3;

	for (i = 0; i < nRows; ++i)
	{
		p = frame.ptr<uchar>(i);
		q = out.ptr<uchar>(i);

		for (j = 0; j < nCols; j += 3)
		{
			CDFHistograma cdfR;
			CDFHistograma cdfG;
			CDFHistograma cdfB;

			for (int k = 0; k < CDF_R.size(); k++) {
				if (p[j + 2] == CDF_R.at(k).pixel)
					cdfR = CDF_R.at(k);
			}
			for (int k = 0; k < CDF_G.size(); k++) {
				if (p[j + 1] == CDF_G.at(k).pixel)
					cdfG = CDF_G.at(k);
			}
			for (int k = 0; k < CDF_B.size(); k++) {
				if (p[j] == CDF_B.at(k).pixel)
					cdfB = CDF_B.at(k);
			}

			q[j + 0] = cdfB.newPixel;
			q[j + 1] = cdfG.newPixel;
			q[j + 2] = cdfR.newPixel;
		}
	}

	delete[] histoR;
	delete[] histoG;
	delete[] histoB;
}

void histogramaUniformeShit(Mat frame, Mat &out) {
	out = frame.clone();

	int channels = frame.channels();

	int nRows = frame.rows;
	//las columnas efectivas de la imagen
	int nCols = frame.cols * channels;

	int i, j, k = 0;
	int histoSize = 0;
	unsigned char *histoR, *histoG, *histoB;
	int rowsandcols = frame.rows*frame.cols;
	histoR = new unsigned char[rowsandcols];
	histoG = new unsigned char[rowsandcols];
	histoB = new unsigned char[rowsandcols];

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

	heapSort(histoR, rowsandcols);
	heapSort(histoG, rowsandcols);
	heapSort(histoB, rowsandcols);

	vector<CDFHistograma> CDF_R;
	vector<CDFHistograma> CDF_G;
	vector<CDFHistograma> CDF_B;
	double minPr = 1.0 / (double)rowsandcols;

	//OBTIENE TODOS LOS CDF
	for (i = 0; i < rowsandcols; i++) {
		CDFHistograma cdfActual;
		for (j = 0; j < CDF_R.size(); j++) {
			if (histoR[i] == CDF_R.at(j).pixel) {
				CDF_R.at(j).CdF++;
				CDF_R.at(j).Pr += minPr;
				cdfActual = CDF_R.at(j);
				break;
			}
		}
		if (cdfActual.CdF == 0) {
			cdfActual.CdF = 1;
			cdfActual.pixel = histoR[i];
			cdfActual.Pr = minPr;
			CDF_R.push_back(cdfActual);
		}
		//Blue
		cdfActual.CdF = 0;
		for (j = 0; j < CDF_B.size(); j++) {
			if (histoB[i] == CDF_B.at(j).pixel) {
				CDF_B.at(j).CdF++;
				CDF_B.at(j).Pr += minPr;
				cdfActual = CDF_B.at(j);
				break;
			}
		}
		if (cdfActual.CdF == 0) {
			cdfActual.CdF = 1;
			cdfActual.pixel = histoB[i];
			cdfActual.Pr = minPr;
			CDF_B.push_back(cdfActual);
		}
		//Green
		cdfActual.CdF = 0;
		for (j = 0; j < CDF_G.size(); j++) {
			if (histoG[i] == CDF_G.at(j).pixel) {
				CDF_G.at(j).CdF++;
				CDF_G.at(j).Pr += minPr;
				cdfActual = CDF_G.at(j);
				break;
			}
		}
		if (cdfActual.CdF == 0)
		{
			cdfActual.CdF = 1;
			cdfActual.pixel = histoG[i];
			cdfActual.Pr = minPr;
			CDF_G.push_back(cdfActual);
		}
	}

	double PrRj = 0;
	for (j = 0; j < CDF_R.size(); j++) {
		int CdF = CDF_R.at(j).CdF;
		PrRj += CDF_R.at(j).Pr;
		double v = (double)((histoR[rowsandcols - 1] - histoR[0]) * PrRj + histoR[0]);
		CDF_R.at(j).newPixel = (uchar)floor(v);
	}

	PrRj = 0;
	for (j = 0; j < CDF_G.size(); j++) {
		int CdF = CDF_G.at(j).CdF;
		PrRj += CDF_G.at(j).Pr;
		double v = (double)((histoG[rowsandcols - 1] - histoG[0]) * PrRj + histoG[0]);
		CDF_G.at(j).newPixel = (uchar)floor(v);
	}

	PrRj = 0;
	for (j = 0; j < CDF_B.size(); j++) {
		int CdF = CDF_B.at(j).CdF;
		PrRj += CDF_B.at(j).Pr;
		double v = (double)((histoB[rowsandcols - 1] - histoB[0]) * PrRj + histoB[0]);
		CDF_B.at(j).newPixel = (uchar)floor(v);
	}

	int tmp = 0;
	double v1, v2, v3;

	for (i = 0; i < nRows; ++i)
	{
		p = frame.ptr<uchar>(i);
		q = out.ptr<uchar>(i);

		for (j = 0; j < nCols; j += 3)
		{
			CDFHistograma cdfR;
			CDFHistograma cdfG;
			CDFHistograma cdfB;

			for (int k = 0; k < CDF_R.size(); k++) {
				if (p[j + 2] == CDF_R.at(k).pixel)
					cdfR = CDF_R.at(k);
			}
			for (int k = 0; k < CDF_G.size(); k++) {
				if (p[j + 1] == CDF_G.at(k).pixel)
					cdfG = CDF_G.at(k);
			}
			for (int k = 0; k < CDF_B.size(); k++) {
				if (p[j] == CDF_B.at(k).pixel)
					cdfB = CDF_B.at(k);
			}

			q[j + 0] = cdfB.newPixel;
			q[j + 1] = cdfG.newPixel;
			q[j + 2] = cdfR.newPixel;
		}
	}

	delete[] histoR;
	delete[] histoG;
	delete[] histoB;
}

void histogramaExponencialShit(Mat frame, Mat &out) {
	out = frame.clone();

	int channels = frame.channels();

	int nRows = frame.rows;
	//las columnas efectivas de la imagen
	int nCols = frame.cols * channels;

	int i, j, k = 0;
	int histoSize = 0;
	unsigned char *histoR, *histoG, *histoB;
	int rowsandcols = frame.rows*frame.cols;
	histoR = new unsigned char[rowsandcols];
	histoG = new unsigned char[rowsandcols];
	histoB = new unsigned char[rowsandcols];

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

	heapSort(histoR, rowsandcols);
	heapSort(histoG, rowsandcols);
	heapSort(histoB, rowsandcols);

	vector<CDFHistograma> CDF_R;
	vector<CDFHistograma> CDF_G;
	vector<CDFHistograma> CDF_B;
	double minPr = 1.0 / (double)rowsandcols;

	//OBTIENE TODOS LOS CDF
	for (i = 0; i < rowsandcols; i++) {
		CDFHistograma cdfActual;
		for (j = 0; j < CDF_R.size(); j++) {
			if (histoR[i] == CDF_R.at(j).pixel) {
				CDF_R.at(j).CdF++;
				CDF_R.at(j).Pr += minPr;
				cdfActual = CDF_R.at(j);
				break;
			}
		}
		if (cdfActual.CdF == 0) {
			cdfActual.CdF = 1;
			cdfActual.pixel = histoR[i];
			cdfActual.Pr = minPr;
			CDF_R.push_back(cdfActual);
		}
		//Blue
		cdfActual.CdF = 0;
		for (j = 0; j < CDF_B.size(); j++) {
			if (histoB[i] == CDF_B.at(j).pixel) {
				CDF_B.at(j).CdF++;
				CDF_B.at(j).Pr += minPr;
				cdfActual = CDF_B.at(j);
				break;
			}
		}
		if (cdfActual.CdF == 0) {
			cdfActual.CdF = 1;
			cdfActual.pixel = histoB[i];
			cdfActual.Pr = minPr;
			CDF_B.push_back(cdfActual);
		}
		//Green
		cdfActual.CdF = 0;
		for (j = 0; j < CDF_G.size(); j++) {
			if (histoG[i] == CDF_G.at(j).pixel) {
				CDF_G.at(j).CdF++;
				CDF_G.at(j).Pr += minPr;
				cdfActual = CDF_G.at(j);
				break;
			}
		}
		if (cdfActual.CdF == 0)
		{
			cdfActual.CdF = 1;
			cdfActual.pixel = histoG[i];
			cdfActual.Pr = minPr;
			CDF_G.push_back(cdfActual);
		}
	}

	int minCdF_R = CDF_R.at(0).CdF;
	int minCdF_G = CDF_G.at(0).CdF;
	int minCdF_B = CDF_B.at(0).CdF;

	for (j = 0; j < CDF_R.size(); j++) {
		if (j == 0)
			continue;
		CDF_R.at(j).CdF += CDF_R.at(j - 1).CdF;
	}
	for (j = 0; j < CDF_G.size(); j++) {
		if (j == 0)
			continue;
		CDF_G.at(j).CdF += CDF_G.at(j - 1).CdF;
	}
	for (j = 0; j < CDF_B.size(); j++) {
		if (j == 0)
			continue;
		CDF_B.at(j).CdF += CDF_B.at(j - 1).CdF;
	}

	double PrRj = 0;
	for (j = 0; j < CDF_R.size(); j++) {
		int CdF = CDF_R.at(j).CdF;
		PrRj += CDF_R.at(j).Pr;
		double lel = log(1.0 - PrRj);
		double lell = -(1.0 / 0.01) * lel;
		double v = (double)minCdF_R + lell;
		CDF_R.at(j).newPixel = (uchar)floor(v);
	}

	PrRj = 0;
	for (j = 0; j < CDF_G.size(); j++) {
		int CdF = CDF_G.at(j).CdF;
		PrRj += CDF_G.at(j).Pr;
		double lel = log(1.0 - PrRj);
		double lell = -(1.0 / 0.01) * lel;
		double v = (double)minCdF_G + lell;
		CDF_G.at(j).newPixel = (uchar)floor(v);
	}


	PrRj = 0;
	for (j = 0; j < CDF_B.size(); j++) {
		int CdF = CDF_B.at(j).CdF;
		PrRj += CDF_B.at(j).Pr;
		double lel = log(1.0 - PrRj);
		double lell = -(1.0/0.01) * lel;
		double v = (double)minCdF_B + lell;
		CDF_B.at(j).newPixel = (uchar)floor(v);
	}

	int tmp = 0;
	double v1, v2, v3;

	for (i = 0; i < nRows; ++i)
	{
		p = frame.ptr<uchar>(i);
		q = out.ptr<uchar>(i);

		for (j = 0; j < nCols; j += 3)
		{
			CDFHistograma cdfR;
			CDFHistograma cdfG;
			CDFHistograma cdfB;

			for (int k = 0; k < CDF_R.size(); k++) {
				if (p[j + 2] == CDF_R.at(k).pixel)
					cdfR = CDF_R.at(k);
			}
			for (int k = 0; k < CDF_G.size(); k++) {
				if (p[j + 1] == CDF_G.at(k).pixel)
					cdfG = CDF_G.at(k);
			}
			for (int k = 0; k < CDF_B.size(); k++) {
				if (p[j] == CDF_B.at(k).pixel)
					cdfB = CDF_B.at(k);
			}

			q[j + 0] = cdfB.newPixel;
			q[j + 1] = cdfG.newPixel;
			q[j + 2] = cdfR.newPixel;
		}
	}

	delete[] histoR;
	delete[] histoG;
	delete[] histoB;
}

// main function to do heap sort 
void heapSort(unsigned char arr[], int n)
{
	// Build heap (rearrange array) 
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	// One by one extract an element from heap 
	for (int i = n - 1; i >= 0; i--)
	{
		// Move current root to end 
		swap(arr[0], arr[i]);

		// call max heapify on the reduced heap 
		heapify(arr, i, 0);
	}
}

void heapify(unsigned char arr[], int n, int i)
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