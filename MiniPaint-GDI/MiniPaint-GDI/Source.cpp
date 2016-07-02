/*
	использование GDI (Graphics Device Interface) интерфейса
	на примере не сложного графического редактора
*/

#include <Windows.h>
#include <wingdi.h>
#include "resource.h"

void ShapeFill(HDC &hDC, int, int);

HINSTANCE hInst;
HRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);
VOID ButttonState(HWND lParam); //состояние кнопок


int nWidthWindow = 800,				//ширина окна
	nHeightWindow = 600;			// высота окна
int nWidthArea = nWidthWindow-38,	//ширина участка для рисования
	nHeightArea = nHeightWindow-20;	//высота участка для рисования

HWND hWorkArea = NULL;		//хэндл рабочего окна
HWND hPrevButton = NULL;	//кнопка была нажата

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmd, int nCmdShow)
{
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(WNDCLASSEX));
	hInst = hInstance;
	wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"class";
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if(!::RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Call to RegisterClassEx failed class!", L"Win32 Guided Tour", NULL);
		return 1;
	}

	HWND hWnd = ::CreateWindowEx (NULL, L"class", L"- MY PAINT -", WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		200, 100, nWidthWindow+30, nHeightWindow+30, NULL, NULL, hInstance, NULL);
	if(!hWnd)
	{
		::MessageBox(NULL, L"Error create handle...", L"ERROR", MB_OK);
		return 1;
	}
	::ShowWindow(hWnd, nCmdShow);
	::UpdateWindow(hWnd);

	MSG msg;
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
}

COLORREF OldColor = RGB(0, 0, 0);
//ОБРАБОТКА СООБЩЕНИЙ ГЛАВНОГО ОКНА
HRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x = 0, y = 0;
	HBITMAP hbImgButton = NULL,	// изображение на кнопках
	hbImgPalitra = NULL;		// изображенеи палитры

	PAINTSTRUCT ps;
	HDC hDC = NULL;
	HWND hButtons = NULL,	//декриптор кнопок инструментов
		hPalitra = NULL;	//дескриптор статика выбора палитры

	int i = 0; //счетчик кнопок

	//переменные для окна выбора цвета
	CHOOSECOLOR cc;                 // структура содержащая информацию о диологовом окне выбора цвета
	static COLORREF acrCustClr[16]; // структура содержит информацию о цвете RGB 
	static DWORD rgbCurrent;        // первоначально выбранный цвет

	switch (message)
	{
	case WM_CREATE: //при создании главного окна

		WNDCLASSEX wc; //клас дочернего окна
		memset(&wc, 0, sizeof(WNDCLASSEX));
		wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
		wc.hCursor = ::LoadCursor(NULL, IDC_CROSS);
		wc.hInstance = hInst;
		wc.lpszClassName = L"WorkArea";
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = ChildProc;
		wc.cbSize = sizeof(WNDCLASSEX);

		if(!::RegisterClassEx(&wc))
		{
			MessageBox(NULL, L"Call to RegisterClassEx failed WorkArea!", L"Win32 Guided Tour", NULL);
			return 1;
		}

		hWorkArea = ::CreateWindowEx (NULL, L"WorkArea", NULL, WS_CHILD|WS_VISIBLE|WS_BORDER,
			38, 1, nWidthArea, nHeightArea, hWnd, NULL, hInst, NULL);
		if(!hWnd)
		{
			::MessageBox(NULL, L"Error create handle hWorkArea...", L"ERROR", MB_OK);
			return 1;
		}
		
		//окно статика выбора палитры
		hPalitra = ::CreateWindowEx (NULL, L"STATIC", NULL, WS_CHILD|SS_BITMAP|SS_NOTIFY|WS_VISIBLE,
			2, 250, 36, 230, hWnd, (HMENU) IDB_PALITRA, hInst, NULL);
		hbImgPalitra = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		::SendMessage(hPalitra, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) hbImgPalitra);

		//кнопки выбора инструмента
		while (i < 7)
		{
			hButtons = ::CreateWindowEx (NULL, L"BUTTON", NULL, WS_CHILD|WS_VISIBLE|BS_BITMAP,
				2, 32*i+2, 32, 32, hWnd, (HMENU) (201+i), hInst, NULL);
			hbImgButton = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1 + i));
			::SendMessage(hButtons, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM) hbImgButton);
			::DeleteObject(hButtons);
			i++;
		}
		break;

	case WM_PAINT: //перерисовка главного окна
		hDC = ::BeginPaint(hWnd, &ps);
		::EndPaint(hWnd, &ps);
        break;

	case WM_COMMAND:
		//вызов окна выбора цвета
		if(HIWORD(wParam) == STN_CLICKED && LOWORD(wParam) == IDB_PALITRA)
		{
			ZeroMemory(&cc, sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hWnd;
			cc.lpCustColors = (LPDWORD) acrCustClr;
			cc.rgbResult = rgbCurrent;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;
			if (ChooseColor(&cc))
				OldColor = cc.rgbResult;
		}
		SendMessage(hWorkArea, message, wParam, lParam);
		break;

	case WM_DESTROY:
		::DeleteObject(hbImgButton);
		::DeleteObject(hbImgPalitra);

		::PostQuitMessage(0);
		break;

	default:
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


BOOL fDraw = FALSE;		//нажатие ЛКМ
POINT ptPrevious= {0};	//координаты начала рисования

HDC hMemDC = NULL;		//дескриптор контекста в памяти
HBITMAP hBitmap;
HBITMAP hOldBitmap;
PAINTSTRUCT ps;

HPEN hPenNew = NULL,	//выбраная кисть для рисования
	hPenOld = NULL,		//старая кисть
	hPenForm = NULL,	//кисть для квадратов/кругов
	hPenBrush = NULL;
	WORD wChoise=NULL;	// выбор инструмента

HRESULT CALLBACK ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		hdc = ::GetDC(hWnd);
		hMemDC = ::CreateCompatibleDC(hdc); //совместимый контекст в памяти
		hBitmap = ::CreateCompatibleBitmap(hdc, nWidthArea, nHeightArea); //совместимый битмап
		hOldBitmap = (HBITMAP) ::SelectObject(hMemDC, hBitmap); //загружаем в контекст битмап
		::PatBlt(hMemDC, 0, 0, nWidthArea, nHeightArea, WHITENESS); //заливаем белым цветом
		hPenNew = ::CreatePen(PS_SOLID, 3, OldColor); //выбираем кисть по умолчанию

		ReleaseDC(hWnd, hdc);
		break;

	case WM_LBUTTONDOWN:

		fDraw = TRUE; //ЛКМ нажата
		
		ptPrevious.x = LOWORD(lParam);
		ptPrevious.y = HIWORD(lParam);		
		break;

	case WM_PAINT:
		hdc = ::BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, nWidthArea, nHeightArea, hMemDC, 0, 0, SRCCOPY);

		::EndPaint(hWnd, &ps);
		break;
 
	case WM_LBUTTONUP:
		if (fDraw)
		{
			hdc = ::GetDC(hWnd);
			hPenOld = (HPEN)::SelectObject(hMemDC, hPenNew); //выбираем кисть в hMemDC

			if(wChoise == IDB_PEN || wChoise == IDB_BRUSH || wChoise == IDB_ERASE || wChoise == IDB_LINE)
			{ 
				//рисуем тонкой кистью, широкой кистью, стираем или проводим линию
				::MoveToEx(hMemDC, ptPrevious.x, ptPrevious.y, NULL);
				::LineTo(hMemDC, LOWORD(lParam), HIWORD(lParam));

				//загружаем в ДС экрана
				::BitBlt(hdc, 0, 0, nWidthArea, nHeightArea, hMemDC, 0, 0, SRCCOPY);
			}
			else if(wChoise == IDB_RECTANGLE)
			{ //рисуем квадрат
				::Rectangle(hMemDC, ptPrevious.x, ptPrevious.y, LOWORD(lParam), HIWORD(lParam));
				::BitBlt(hdc, 0, 0, nWidthArea, nHeightArea, hMemDC, 0, 0, SRCCOPY);
			}
			else if(wChoise == IDB_ELLIPSE)
			{
				::Ellipse(hMemDC, ptPrevious.x, ptPrevious.y, LOWORD(lParam), HIWORD(lParam));
				::BitBlt(hdc, 0, 0, nWidthArea, nHeightArea, hMemDC, 0, 0, SRCCOPY);
			}
			else if(wChoise == IDB_FILL)
			{ //заливка (есть возможность реализовать через станартную функцию)
				ShapeFill(hMemDC, LOWORD(lParam), HIWORD(lParam));
				
				//::FloodFill(hMemDC, ptPrevious.x, ptPrevious.y, OldColor);
				::BitBlt(hdc, 0, 0, nWidthArea, nHeightArea, hMemDC, 0, 0, SRCCOPY);
			}
			fDraw = FALSE; //сброс флага

			::SelectObject(hMemDC, hPenOld);
			::ReleaseDC(hWnd, hdc);
		} 
		break;

	case WM_MOUSEMOVE:
		if (fDraw && (wChoise == IDB_PEN || wChoise == IDB_BRUSH || wChoise == IDB_ERASE))
		{ //рисуем тонкой кистью, широкой кистью или стираем
			hdc = GetDC(hWnd);
			hPenOld = (HPEN)::SelectObject(hMemDC, hPenNew); //выбираем кисть в hMemDC

			::MoveToEx(hMemDC, ptPrevious.x, ptPrevious.y, NULL); 
			::LineTo(hMemDC, ptPrevious.x = LOWORD(lParam), ptPrevious.y = HIWORD(lParam));
			BitBlt(hdc, 0, 0, nWidthArea, nHeightArea, hMemDC, 0, 0, SRCCOPY);

			::ReleaseDC(hWnd, hdc);
		} 
		break; 

		

	case WM_COMMAND:
		wChoise = LOWORD(wParam);
		switch(wChoise) //выбор инструмента
		{
		case IDB_PEN:
			ButttonState((HWND) lParam);
			hPenNew = ::CreatePen(PS_SOLID, 3, OldColor);
			break;

		case IDB_BRUSH:
			ButttonState((HWND) lParam);
			hPenNew = ::CreatePen(PS_SOLID, 15, OldColor);
			break;

		case IDB_LINE:
			ButttonState((HWND) lParam);
			break;

		case IDB_RECTANGLE:
			hPenForm = ::CreatePen(PS_SOLID, 3, OldColor);
			ButttonState((HWND) lParam);
			break;

		case IDB_ELLIPSE:
			hPenForm = ::CreatePen(PS_SOLID, 3, OldColor);
			ButttonState((HWND) lParam);
			break;

		case IDB_FILL:
			hPenBrush = ::CreatePen(PS_SOLID, 1, OldColor);
			ButttonState((HWND) lParam);
			break;

		case IDB_ERASE:
			ButttonState((HWND) lParam);
			hPenNew = ::CreatePen(PS_SOLID, 25, RGB(255,255,255));
			break;
		}
		break;
	case WM_DESTROY:
		::SelectObject(hMemDC, hOldBitmap);
		::SelectObject(hMemDC, hPenOld);
		::DeleteObject(hOldBitmap);
		::DeleteObject(hPenBrush);
		::DeleteObject(hPenNew);
		::DeleteObject(hPenForm);
		::DeleteObject(hPenOld);
		::DeleteDC(hMemDC);
		return 0;		
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

//состояние кнопок выбора инструмента
VOID ButttonState(HWND lParam)
{
	if(hPrevButton)
		::SendMessage(hPrevButton, BM_SETSTATE, FALSE, NULL);
	hPrevButton = (HWND) lParam;
	::SendMessage((HWND)  lParam, BM_SETSTATE, TRUE, NULL);
}

//функция заполнения (сырой вариант, заполняет только правельные формы)
void ShapeFill(HDC &hDC, int x, int y)
{
	int iLeftCoord = NULL,
		iRightCoord = NULL;
	COLORREF oldColor = ::GetPixel(hDC, x, y);
	::SelectObject(hMemDC, hPenBrush);

	int i=x;
	int j=y;

	while(j < nHeightWindow-20 && oldColor == ::GetPixel(hDC, i, j))
	{
		while (i >= 20 && oldColor == ::GetPixel(hDC, i, j))
			i--;
		iLeftCoord = i;

		i = x + 1;
		while (i < nWidthWindow-38 && oldColor == ::GetPixel(hDC, i, j))
			i++;
		iRightCoord = i;

		::MoveToEx(hDC, iLeftCoord, j, NULL); 
		::LineTo(hDC, iRightCoord, j);

		i = iLeftCoord + (iRightCoord-iLeftCoord)/2;
		j++;
	}
	
	i = x;
	j = y - 1;
	while(j >= 1 && oldColor == ::GetPixel(hDC, i, j))
	{
		while (i >= 20 && oldColor == ::GetPixel(hDC, i, j))
			i--;
		iLeftCoord = i;

		i = x + 1;
		while (i < nWidthWindow-38 && oldColor == ::GetPixel(hDC, i, j))
			i++;
		iRightCoord = i;

		::MoveToEx(hDC, iLeftCoord, j, NULL); 
		::LineTo(hDC, iRightCoord, j);

		i = iLeftCoord + (iRightCoord - iLeftCoord) / 2;
		j--;
	}
}