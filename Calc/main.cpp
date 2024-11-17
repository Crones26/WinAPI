#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "resource.h"
#include <stdio.h>

CONST CHAR g_sz_CLASS_NAME[] = "Calc PV_319";

CONST INT g_i_INTERVAL = 5;
CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_BUTTON_DOUBLE_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;

CONST INT g_i_DISPLAY_WIDTH = g_i_BUTTON_SIZE * 5 + g_i_INTERVAL * 4;
CONST INT g_i_DISPLAY_HEIGHT = 22;

CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;
CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_Y + g_i_DISPLAY_HEIGHT + g_i_INTERVAL;
CONST INT g_i_OPERATION_BUTTON_START_X = g_i_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3;
CONST INT g_i_OPERATION_BUTTON_START_Y = g_i_BUTTON_START_Y;
CONST INT g_i_CONTROL_BUTTON_START_X = g_i_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4;
CONST INT g_i_CONTROL_BUTTON_START_Y = g_i_BUTTON_START_Y;

CONST INT g_i_WINDOW_WIDTH = g_i_DISPLAY_WIDTH + g_i_START_X * 2 + 16;
CONST INT g_i_WINDOW_HEIGHT = g_i_DISPLAY_HEIGHT + g_i_START_Y * 2 + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4 + 38;

CONST CHAR* g_OPERATIONS[] = { "+", "-", "*", "/" };

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

_Use_decl_annotations_ 
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    //1) Регистрация класса окна:
    WNDCLASSEX wClass = { 0 };
    wClass.cbSize = sizeof(wClass);
    wClass.style = 0;
    wClass.hInstance = hInstance;
    wClass.lpszClassName = g_sz_CLASS_NAME;
    wClass.lpfnWndProc = (WNDPROC)WndProc;
    wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassEx(&wClass))
    {
        MessageBox(NULL, "Class registration failed!", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // Вычисление координат для центрирования окна
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowPosX = (screenWidth - g_i_WINDOW_WIDTH) / 2;
    int windowPosY = (screenHeight - g_i_WINDOW_HEIGHT) / 2;

    //2) Создание окна:
    HWND hwnd = CreateWindowEx
    (
        0, g_sz_CLASS_NAME, "Calculator",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
        windowPosX, windowPosY, // Устанавливаем координаты окна
        g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    //3) Запуск цикла сообшений:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static double operand1 = 0.0;
    static double operand2 = 0.0;
    static char operation = '\0';
    static bool operationSelected = false;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        HWND hEdit = CreateWindowEx(
            0, "Edit", "0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            g_i_START_X, g_i_START_Y,
            g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
            hwnd, (HMENU)IDC_EDIT_DISPLAY,
            GetModuleHandle(NULL), NULL);

        // Устанавливаем фокус на строку ввода
        SetFocus(hEdit);

        CHAR sz_digit[2] = "0";
        for (int i = 6; i >= 0; i -= 3)
        {
            for (int j = 0; j < 3; j++)
            {
                sz_digit[0] = 49 + i + j; // 49 - ASCII-код единицы.
                CreateWindowEx(
                    0, "Button", sz_digit,
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                    g_i_BUTTON_START_X + j * (g_i_BUTTON_SIZE + g_i_INTERVAL),
                    g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (2 - i / 3),
                    g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
                    hwnd, (HMENU)(IDC_BUTTON_1 + i + j),
                    GetModuleHandle(NULL), NULL);
            }
        }

        CreateWindowEx(0, "Button", "0",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            g_i_BUTTON_START_X,
            g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
            g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
            hwnd, (HMENU)IDC_BUTTON_0,
            GetModuleHandle(NULL), NULL);

        CreateWindowEx(0, "Button", ".",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            g_i_BUTTON_START_X + g_i_BUTTON_DOUBLE_SIZE + g_i_INTERVAL,
            g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
            g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
            hwnd, (HMENU)IDC_BUTTON_POINT,
            GetModuleHandle(NULL), NULL);

        for (int i = 0; i < 4; i++)
        {
            CreateWindowEx(0, "Button", g_OPERATIONS[i],
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                g_i_OPERATION_BUTTON_START_X,
                g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (3 - i),
                g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
                hwnd, (HMENU)(IDC_BUTTON_PLUS + i),
                GetModuleHandle(NULL), NULL);
        }

        CreateWindowEx(0, "Button", "<-",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            g_i_CONTROL_BUTTON_START_X,
            g_i_CONTROL_BUTTON_START_Y,
            g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
            hwnd, (HMENU)IDC_BUTTON_BSP,
            GetModuleHandle(NULL), NULL);

        CreateWindowEx(0, "Button", "C",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            g_i_CONTROL_BUTTON_START_X,
            g_i_CONTROL_BUTTON_START_Y + g_i_BUTTON_SIZE + g_i_INTERVAL,
            g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
            hwnd, (HMENU)IDC_BUTTON_CLEAR,
            GetModuleHandle(NULL), NULL);

        CreateWindowEx(0, "Button", "=",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            g_i_CONTROL_BUTTON_START_X,
            g_i_CONTROL_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 2,
            g_i_BUTTON_SIZE, g_i_BUTTON_DOUBLE_SIZE,
            hwnd, (HMENU)IDC_BUTTON_EQUAL,
            GetModuleHandle(NULL), NULL);
    }
    break;

    case WM_COMMAND:
    {
        CONST INT SIZE = 256;
        CHAR sz_display[SIZE] = {};
        CHAR sz_digit[2] = {};
        HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
        SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);

        // Обработка кнопок цифр
        if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
        {
            sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
            if (strlen(sz_display) == 1 && sz_display[0] == '0')
                sz_display[0] = sz_digit[0];
            else
                strcat(sz_display, sz_digit);
            SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
        }

        // Обработка кнопки точки
        if (LOWORD(wParam) == IDC_BUTTON_POINT)
        {
            if (strchr(sz_display, '.')) break;
            strcat(sz_display, ".");
            SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
        }

        // Обработка кнопки операций
        if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
        {
            operand1 = atof(sz_display); // Сохраняем первый операнд
            int operationIndex = LOWORD(wParam) - IDC_BUTTON_PLUS;
            if (operationIndex >= 0 && operationIndex < 4) // Проверка, чтобы индекс был в пределах массива
            {
                operation = g_OPERATIONS[operationIndex][0];
                operationSelected = true;
                SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"0"); // Очистка дисплея для второго операнда
            }
        }

        // Обработка кнопки "="
        if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
        {
            operand2 = atof(sz_display);
            double result = 0.0;

            switch (operation)
            {
            case '+': result = operand1 + operand2; break;
            case '-': result = operand1 - operand2; break;
            case '*': result = operand1 * operand2; break;
            case '/':
                if (operand2 != 0.0)
                    result = operand1 / operand2;
                else
                    MessageBox(hwnd, "На ноль делить нельзя!", "Ошибка", MB_OK | MB_ICONERROR);
                break;
            }

            sprintf(sz_display, "%.2f", result);
            SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
            operationSelected = false;
        }

        // Обработка кнопки "C"
        if (LOWORD(wParam) == IDC_BUTTON_CLEAR)
        {
            operand1 = operand2 = 0.0;
            operation = '\0';
            operationSelected = false;
            SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"0");
        }

        // Обработка кнопки "<-"
        if (LOWORD(wParam) == IDC_BUTTON_BSP)
        {
            size_t len = strlen(sz_display);
            if (len > 0)
            {
                sz_display[len - 1] = '\0';
                if (len == 1)
                    strcpy(sz_display, "0");
                SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
            }
        }
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