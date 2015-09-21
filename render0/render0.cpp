// render0.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "render0.h"
#include "Device.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);;
void Draw();
// �Զ���ȫ������
static const int left = 50;
static const int top = 50;
static const int width = 800;
static const int height = 600;

static LPVOID memory;
static HDC cdc;
static Device *render0;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RENDER0, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RENDER0));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	delete render0;
	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RENDER0));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RENDER0);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 900, 750, NULL, NULL, hInstance, NULL);
	HDC hdc = GetDC(hWnd);
	cdc = CreateCompatibleDC(hdc);
	
	BITMAPINFO bi = {{ sizeof(BITMAPINFOHEADER), width, -height, 1, 32, BI_RGB, 
		width * height * 4, 0, 0, 0, 0 }};
	HBITMAP hb = CreateDIBSection(cdc, &bi, DIB_RGB_COLORS, &memory, NULL, 0);
	SelectObject(cdc, hb);
	ReleaseDC(hWnd, hdc);
	render0 = new Device(800, 600, (Color *)memory);
	
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	
	int bltRes;
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Draw();
		bltRes = BitBlt(hdc, top, left, width, height, cdc, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void Draw()
{
	// TODO: �ڴ���������ͼ����...
	render0->transform.setPerspective(3.1415927*0.5, (double)width / height, 1.0, 500.0);
	render0->transform.setLookAt(Vector<double>(100.0, 100.0, 100.0),
		Vector<double>(0.0, 0.0, 0.0),
		Vector<double>(0.0, 0.0, 1.0));
	Vertex v1(Vector<double>(-50.0, -50.00), Color(0, 0, 255));
	Vertex v2(Vector<double>(50.0, -50.0), Color(0, 0, 255));
	Vertex v3(Vector<double>(50.0, 50.0), Color(0, 0, 255));
	Vertex v4(Vector<double>(-50.0, 50.0), Color(0, 0, 255));
	render0->drawPane(v1, v2, v3, v4);
	Vertex v5(Vector<double>(-50.0, -50.00, 60.0), Color(255, 0, 0));
	Vertex v6(Vector<double>(50.0, -50.0, 60.0), Color(255, 0, 0));
	Vertex v7(Vector<double>(50.0, 50.0, 60.0), Color(255, 0, 0));
	Vertex v8(Vector<double>(-50.0, 50.0, 60.0), Color(255, 0, 0));
	render0->drawPane(v5, v6, v7, v8);
	render0->drawLine(v1, v5)->drawLine(v2, v6)->drawLine(v3, v7)->drawLine(v4, v8);
	Vertex zero(Vector<double>(0.0, 0.0, 0.0), Color(255, 255, 255));
	Vertex xaxis(Vector<double>(100.0, 0.0, 0.0), Color(255, 0, 0));
	Vertex yaxis(Vector<double>(0.0, 100.0, 0.0), Color(0, 255, 0));
	Vertex zaxis(Vector<double>(0.0, 0.0, 100.0), Color(0, 0, 255));
	render0->drawLine(zero, xaxis)->drawLine(zero, yaxis)->drawLine(zero, zaxis);
}
