#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include "resource.h"

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "CalculatorWindow";
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

char buffer[256];
double value = 0;
double currentValue = 0;
char operation = 0;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    WNDCLASSEX wc = { sizeof(wc) };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = g_sz_MY_WINDOW_CLASS;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

    RegisterClassEx(&wc);

    // Определите размеры окна
    int windowWidth = 300;
    int windowHeight = 400;

    // Получите размер экрана
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Вычислите координаты для центрирования окна
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    HWND hwnd = CreateWindowEx(
        0, g_sz_MY_WINDOW_CLASS, "Калькулятор", WS_OVERLAPPEDWINDOW,
        x, y, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

void AddButton(HWND hwnd, int id, const char* text, int x, int y, int width, int height)
{
    CreateWindow("BUTTON", text, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        x, y, width, height, hwnd, (HMENU)id, NULL, NULL);
}

void AddControls(HWND hwnd)
{
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT,
        10, 10, 260, 40, hwnd, (HMENU)IDC_EDIT, NULL, NULL);

    // Первая строка кнопок
    AddButton(hwnd, IDC_BTN_7, "7", 10, 100, 60, 40);
    AddButton(hwnd, IDC_BTN_8, "8", 80, 100, 60, 40);
    AddButton(hwnd, IDC_BTN_9, "9", 150, 100, 60, 40);

    // Вторая строка кнопок
    AddButton(hwnd, IDC_BTN_4, "4", 10, 150, 60, 40);
    AddButton(hwnd, IDC_BTN_5, "5", 80, 150, 60, 40);
    AddButton(hwnd, IDC_BTN_6, "6", 150, 150, 60, 40);

    // Третья строка кнопок
    AddButton(hwnd, IDC_BTN_1, "1", 10, 200, 60, 40);
    AddButton(hwnd, IDC_BTN_2, "2", 80, 200, 60, 40);
    AddButton(hwnd, IDC_BTN_3, "3", 150, 200, 60, 40);

    // Последняя строка кнопок
    AddButton(hwnd, IDC_BTN_0, "0", 10, 250, 60, 40);
    AddButton(hwnd, IDC_BTN_PLUS, "+", 150, 250, 60, 40);
    AddButton(hwnd, IDC_BTN_EQUALS, "=", 220, 250, 60, 90);
}

void ProcessButton(HWND hwnd, WPARAM wParam)
{
    char text[256];
    HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);

    switch (LOWORD(wParam))
    {
    case IDC_BTN_0: case IDC_BTN_1: case IDC_BTN_2: case IDC_BTN_3:
    case IDC_BTN_4: case IDC_BTN_5: case IDC_BTN_6: case IDC_BTN_7:
    case IDC_BTN_8: case IDC_BTN_9:
        sprintf(text, "%s%c", buffer, '0' + LOWORD(wParam) - IDC_BTN_0);
        strcpy(buffer, text);
        SetWindowText(hEdit, buffer);
        break;

    case IDC_BTN_PLUS:
        value = atof(buffer);
        buffer[0] = '\0';
        operation = '+';
        break;

    case IDC_BTN_EQUALS:
        currentValue = atof(buffer);
        switch (operation)
        {
        case '+': value += currentValue; break;
        }
        sprintf(buffer, "%g", value);
        SetWindowText(hEdit, buffer);
        buffer[0] = '\0';
        operation = 0;
        break;
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        AddControls(hwnd);
        break;
    case WM_COMMAND:
        ProcessButton(hwnd, wParam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}