
#include "stdafx.h"
#include "LaFotoTienda.h"

#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\video.hpp>
#include <opencv2\video\video.hpp>
#include <opencv2\objdetect.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\objdetect\detection_based_tracker.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\videostab.hpp>
#include <opencv2\world.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\video\tracking.hpp>
#include <opencv2\core\ocl.hpp>
#include <opencv2\core\cvstd.hpp>
#include <opencv2\core\utility.hpp>
#include <cassert>
#include <iostream>
#include <CommDlg.h>
#include <fstream>
#include <mmsystem.h>
#include <codecvt>
#include <math.h>
#include <shlwapi.h>
#include <direct.h>

#pragma comment(lib, "Shlwapi.lib")

#pragma comment(lib,"opencv_world310.lib")//d Debug, delete to production (release)
using namespace cv;
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;	//current instance

//Some declarations, needed
void toGray(Mat frame, Mat &out);
void toGray1(Mat frame, Mat &out);
void toGray2(Mat frame, Mat &out);
void toSepia(Mat frame, Mat &out);
void heapSort(unsigned char arr[], int n);
void heapify(unsigned char arr[], int n, int i);
void useThisMat(Mat frame, Mat &out, Mat mat, char matSum, char matX, char matY);
void filtroMediana(Mat frame, Mat &out, char matX, char matY);
void histogramaShit(Mat frame, Mat &out);
void histogramaSimpleShit(Mat frame, Mat &out);
void histogramaUniformeShit(Mat frame, Mat &out);
void histogramaExponencialShit(Mat frame, Mat &out);
Mat makeGaussianoMat(float sigma, int matSize);
void simpleFilterIni();
Mat getListFilterImg(Mat frame);
void cleanFilterList();
void changeTurnedOff();

//Some global variables
Mat frame; //aqui guardaremos el frame
bool exito;
VideoCapture camara; //tipo de dato correspondiente a la camara
bool turnOffAllFilters = false;

//Timer Definer
#define ID_TIMER 6910294
#define ID_RENDERTIMER 6910293

#include "matToBMP.h"
#include "BMPToFile.h"

#include "Filter.h"
#include "Puntuales.h"
#include "Locales.h"
#include "Globales.h"
#include "filtros.h"
#include "useFilters.h"

#include "r_webcam.h"
#include "r_files.h"
#include "r_group.h"

#include "home.h"

// Forward declarations of functions included in this code module:
BOOL                InitInstance(HINSTANCE, int);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAFOTOTIENDA));

    MSG msg;

	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading\n"); exit(0); };
	if (!eyes_cascade.load(eyes_cascade_name)) { printf("--(!)Error loading\n"); exit(0); };

	srand(time(NULL));

	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector()); //Se carga el Support Vector Machine

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateDialog(hInst, MAKEINTRESOURCEW(IDD_HOME), NULL, homeCallback);
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			DestroyWindow(hDlg);
			return (INT_PTR)TRUE;
		}

		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);
		EndPaint(hDlg, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return (INT_PTR)FALSE;
}

