// DiscordQuestPlaceholder.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "DiscordQuestPlaceholder.h"
#include <dwmapi.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <stdlib.h>
#pragma comment( lib, "dwmapi" )

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool stop = false;
int timer = 900;

COLORREF COLOR_BG = RGB(41, 43, 47);
COLORREF COLOR_TITLE_BAR = RGB(30, 33, 36);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    LoadStringW(hInstance, IDC_DISCORDQUESTPLACEHOLDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DISCORDQUESTPLACEHOLDER));

    MSG msg;

    // Main message loop:
    while (!stop)
    {
        if (GetMessage(&msg, nullptr, 0, 0)) {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(COLOR_BG);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DISCORDQUESTPLACEHOLDER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm = 0;

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, L"Quest Placeholder", WS_EX_DLGMODALFRAME,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   SetWindowLong(hWnd, GWL_STYLE, WS_CAPTION);

   RECT rc;

   GetWindowRect(hWnd, &rc);

   int xPos = (GetSystemMetrics(SM_CXSCREEN) - 570) / 2;
   int yPos = (GetSystemMetrics(SM_CYSCREEN) - 340) / 2;


   WINDOWPLACEMENT wp = { sizeof WINDOWPLACEMENT };
   ::GetWindowPlacement(hWnd, &wp);
   ::SetMenu(hWnd, NULL);
   ::SetWindowPos(hWnd, 0, xPos, yPos, 570, 340,
        SWP_NOZORDER | SWP_NOOWNERZORDER);


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT  rect;
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
    case WM_ACTIVATE:
        {
            const MARGINS shadow_on = { 0, 0, 1, 0 };

            DwmExtendFrameIntoClientArea(hWnd, &shadow_on);

        }
        break;
    case WM_NCCALCSIZE:
        if (wParam == TRUE)
        {
            SetWindowLong(hWnd, DWLP_MSGRESULT, 0);
            return TRUE;
        }
        return FALSE;
        break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
            HBRUSH brushCap = static_cast<HBRUSH>(SelectObject(hdc, GetStockObject(DC_BRUSH)));
            HPEN penCap = static_cast<HPEN>(SelectObject(hdc, GetStockObject(DC_PEN)));
            HFONT title_font = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE, TEXT("Segoe UI"));
            HFONT font = CreateFont(16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE, TEXT("Consolas"));
            HFONT emoticon_font = CreateFont(30, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
                OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE, TEXT("Segoe UI"));

            // UNIVERSAL TEXT SETTINGS
            SetTextColor(hdc, RGB(200, 200, 200));
            SetBkMode(hdc, TRANSPARENT);

            // DRAW TITLE BAR
            RECT rcCap = { -1, -1, 890, 23 };
            RECT rcTitleText = { 5, 3, 0, 0 };
            SetDCPenColor(hdc, COLOR_TITLE_BAR);   
            SetDCBrushColor(hdc, COLOR_TITLE_BAR);
            SelectObject(hdc, title_font);
            Rectangle(hdc, rcCap.left, rcCap.top, rcCap.right, rcCap.bottom);
            DrawText(hdc, L"> Quest Placeholder", -1, &rcTitleText, DT_SINGLELINE | DT_NOCLIP);


            // DRAW TEXT
            SelectObject(hdc, font);
            GetClientRect(hWnd, &rect);
            rect.left = 5;
            rect.top = 5;
            OffsetRect(&rect, 0, 20);
            DrawText(hdc, L"Hello World! You can change the name of this .exe file to complete", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            OffsetRect(&rect, 0, 1 * 16);
            DrawText(hdc, L"A Discord 'Stream' Quest! Because of how they work which is using", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            OffsetRect(&rect, 0, 1 * 16);
            DrawText(hdc, L"The application filename IF they do not have proper RPC support", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            OffsetRect(&rect, 0, 1 * 16);
            DrawText(hdc, L"To start, you just have to stream this file in Discord for 15 mins", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            OffsetRect(&rect, 0, 2 * 16);
            SetTextColor(hdc, RGB(200, 100, 100));
            DrawText(hdc, L"NOTE: YOU MUST HAVE SOMEONE ELSE IN THE SAME VC AS YOU!", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            OffsetRect(&rect, 0, 2 * 16);
            SetTextColor(hdc, RGB(200, 200, 200));
            DrawText(hdc, L"Currently, you can use the following names for each quests:", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            OffsetRect(&rect, 0, 1 * 16);
            SetTextColor(hdc, RGB(200, 200, 100));
            DrawText(hdc, L": genshinimpact.exe (Due May 22, 2024): Genshin Impact", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            OffsetRect(&rect, 0, 1 * 16);
            DrawText(hdc, L": lostark.exe (Due April 23, 2024): Lost Ark", -1, &rect, DT_SINGLELINE | DT_NOCLIP);

            SelectObject(hdc, emoticon_font);
            SetTextColor(hdc, RGB(200, 200, 200));
            OffsetRect(&rect, 1 * 16, 3 * 16);
            DrawText(hdc, L"  ╱| 、", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            OffsetRect(&rect, 0, 1 * 25);
            DrawText(hdc, L"(`  -  7", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            OffsetRect(&rect, 0, 1 * 25);
            DrawText(hdc, L" | 、˜〵", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            OffsetRect(&rect, 0, 1 * 25);
            DrawText(hdc, L" じしˍ, )ノ", -1, &rect, DT_SINGLELINE | DT_NOCLIP);

            OffsetRect(&rect, 5 * 16, -2 * 16);
            SelectObject(hdc, font);
            DrawText(hdc, L"by specifix", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            OffsetRect(&rect, 0, 1 * 16);
            DrawText(hdc, L"₍^ >ヮ<^₎ .ᐟ.ᐟ", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            EndPaint(hWnd, &ps);
            DeleteObject(font);
            DeleteObject(brushCap);
            DeleteObject(penCap);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        stop = true;
        break;
    case WM_LBUTTONDOWN:
        PostMessage(hWnd, WM_SYSCOMMAND, SC_SIZE + 9, 0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}