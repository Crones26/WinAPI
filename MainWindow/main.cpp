#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include "resource.h"

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "My Window";    // Имя класса окна

// Путь к курсорам
const char* cursorPathArrow = "C:\\Users\\Cympak\\source\\repos\\WinAPI\\MainWindow\\Starcraft 3D\\Arrow.ani";
const char* cursorPathSizeNS = "C:\\Users\\Cympak\\source\\repos\\WinAPI\\MainWindow\\Starcraft 3D\\SizeNS.ani";
const char* cursorPathSizeWE = "C:\\Users\\Cympak\\source\\repos\\WinAPI\\MainWindow\\Starcraft 3D\\SizeWE.ani";
const char* cursorPathSizeNWSE = "C:\\Users\\Cympak\\source\\repos\\WinAPI\\MainWindow\\Starcraft 3D\\SizeNWSE.ani";
const char* cursorPathSizeNESW = "C:\\Users\\Cympak\\source\\repos\\WinAPI\\MainWindow\\Starcraft 3D\\SizeNESW.ani";

HINSTANCE hInstanceGlobal;
HCURSOR hCursorArrow, hCursorSizeNS, hCursorSizeWE, hCursorSizeNWSE, hCursorSizeNESW;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    hInstanceGlobal = hInstance;

    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    wc.hCursor = (HCURSOR)LoadImage(NULL, cursorPathArrow, IMAGE_CURSOR, 64, 64, LR_LOADFROMFILE); // Основной курсор увеличенного размера
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hInstance = hInstance;
    wc.lpszClassName = g_sz_MY_WINDOW_CLASS;
    wc.lpfnWndProc = WndProc;

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Ошибка регистрации класса", NULL, MB_OK | MB_ICONERROR);
        return 0;
    }

    // Загрузка курсоров
    hCursorArrow = (HCURSOR)LoadImage(NULL, cursorPathArrow, IMAGE_CURSOR, 64, 64, LR_LOADFROMFILE);
    hCursorSizeNS = (HCURSOR)LoadImage(NULL, cursorPathSizeNS, IMAGE_CURSOR, 64, 64, LR_LOADFROMFILE);
    hCursorSizeWE = (HCURSOR)LoadImage(NULL, cursorPathSizeWE, IMAGE_CURSOR, 64, 64, LR_LOADFROMFILE);
    hCursorSizeNWSE = (HCURSOR)LoadImage(NULL, cursorPathSizeNWSE, IMAGE_CURSOR, 64, 64, LR_LOADFROMFILE);
    hCursorSizeNESW = (HCURSOR)LoadImage(NULL, cursorPathSizeNESW, IMAGE_CURSOR, 64, 64, LR_LOADFROMFILE);

    INT screen_width = GetSystemMetrics(SM_CXSCREEN);
    INT screen_height = GetSystemMetrics(SM_CYSCREEN);
    INT window_width = screen_width * 3 / 4;
    INT window_height = screen_height * .75;
    INT window_start_x = screen_width / 8;
    INT window_start_y = screen_height / 8;

    HWND hwnd = CreateWindowEx(
        NULL,
        g_sz_MY_WINDOW_CLASS,
        g_sz_MY_WINDOW_CLASS,
        WS_OVERLAPPEDWINDOW,
        window_start_x, window_start_y,
        window_width, window_height,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.message;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SETCURSOR:
        // Установка специальных курсоров в зависимости от стороны, за которую пользователь изменяет размер окна
        switch (LOWORD(lParam))
        {
        case HTTOP:
        case HTBOTTOM:
            SetCursor(hCursorSizeNS);
            return TRUE;
        case HTRIGHT:
        case HTLEFT:
            SetCursor(hCursorSizeWE);
            return TRUE;
        case HTTOPLEFT:
        case HTBOTTOMRIGHT:
            SetCursor(hCursorSizeNWSE);
            return TRUE;
        case HTTOPRIGHT:
        case HTBOTTOMLEFT:
            SetCursor(hCursorSizeNESW);
            return TRUE;
        default:
            SetCursor(hCursorArrow);
            return TRUE;
        }
        break;

    case WM_MOVE:
    case WM_SIZE:
    {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        INT window_width = rect.right - rect.left;
        INT window_height = rect.bottom - rect.top;

        CONST INT SIZE = 256;
        CHAR sz_title[SIZE]{};
        sprintf(
            sz_title,
            "%s - Позиция: %ix%i; Размер: %ix%i",
            g_sz_MY_WINDOW_CLASS,
            rect.left, rect.top,
            window_width, window_height
        );

        SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}