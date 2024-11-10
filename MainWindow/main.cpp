#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include "resource.h"

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "My Window"; // Имя класса окна

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HCURSOR hCursors[4]; // Массив для хранения курсоров
int currentCursor = 0; // Текущий индекс курсора

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    // 1) Регистрация класса окна:
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.style = 0;
    wc.cbSize = sizeof(wc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0; // cb... - Count Bytes

    // wc.hIcon = (HICON)LoadImage(hInstance, "bitcoin.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    // wc.hIconSm = (HICON)LoadImage(hInstance, "litecoin.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);

    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    // wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
    //wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR5));
    wc.hCursor = (HCURSOR)LoadImage(NULL, "C:\\Users\\Cympak\\source\\repos\\WinAPI\\MainWindow\\curanistarcraft\\Starcraft 02.ani", IMAGE_CURSOR, 32, 32, LR_LOADFROMFILE);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

    wc.hInstance = hInstance;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_sz_MY_WINDOW_CLASS;
    wc.lpfnWndProc = (WNDPROC)WndProc;

    // Загрузка курсоров
    hCursors[0] = (HCURSOR)LoadImage(NULL, "C:\\Users\\Cympak\\source\\repos\\WinAPI\\MainWindow\\curanistarcraft\\Starcraft 02.ani", IMAGE_CURSOR, 32, 32, LR_LOADFROMFILE);
    hCursors[1] = (HCURSOR)LoadImage(NULL, "C:\\Users\\Cympak\\source\\repos\\WinAPI\\MainWindow\\curanistarcraft\\Starcraft 01.ani", IMAGE_CURSOR, 32, 32, LR_LOADFROMFILE);
    hCursors[2] = (HCURSOR)LoadImage(NULL, "C:\\Users\\Cympak\\source\\repos\\WinAPI\\MainWindow\\curanistarcraft\\Starcraft 03.ani", IMAGE_CURSOR, 32, 32, LR_LOADFROMFILE);
    hCursors[3] = (HCURSOR)LoadImage(NULL, "C:\\Users\\Cympak\\source\\repos\\WinAPI\\MainWindow\\curanistarcraft\\Starcraft 04.ani", IMAGE_CURSOR, 32, 32, LR_LOADFROMFILE);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Class registration failed", NULL, MB_OK | MB_ICONERROR);
        return 0;
    }

    // 2) Создание окна:
    INT screen_width = GetSystemMetrics(SM_CXSCREEN);
    INT screen_height = GetSystemMetrics(SM_CYSCREEN);
    INT window_width = screen_width * 3 / 4;
    INT window_height = screen_height * .75;
    INT window_start_x = screen_width / 8;
    INT window_start_y = screen_height / 8;

    HWND hwnd = CreateWindowEx(
        NULL,                           // ExStyles
        g_sz_MY_WINDOW_CLASS,           // Class name
        g_sz_MY_WINDOW_CLASS,           // Window title
        WS_OVERLAPPEDWINDOW,            // Window style
        window_start_x, window_start_y, // Window position
        window_width, window_height,    // Window size
        NULL,    // Parent Window
        NULL,    // Main menu ResourceID for MainWindow or ResourceID for ChildWindow
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow); // Задает режим отображения окна
    UpdateWindow(hwnd);         // Прорисовывает окно.

    // 3) Запуск цикла сообщений:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.message;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
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
            "%s - Position: %ix%i;\tSize: %ix%i",
            g_sz_MY_WINDOW_CLASS,
            rect.left, rect.top,
            window_width, window_height
        );

        SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
    }
    break;
    case WM_RBUTTONDOWN:                          // Событие правой кнопки мыши
        currentCursor = (currentCursor + 1) % 4;  // Меняем индекс курсора
        SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)hCursors[currentCursor]);  // Устанавливаем новый курсор
        SetCursor(hCursors[currentCursor]);       // Устанавливаем новый курсор для текущего окна
        break;
    case WM_COMMAND:
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
    return FALSE;
}