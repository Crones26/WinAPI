#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<iostream>
#include"resource.h"

HBRUSH hbrMainBackground = NULL; // Кисть для фона окна

CONST CHAR g_sz_CLASS_NAME[] = "Calc PV_319";

CONST INT g_i_INTERVAL = 5;
CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_BUTTON_DOUBLE_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;

CONST INT g_i_DISPLAY_WIDTH = g_i_BUTTON_SIZE * 5 + g_i_INTERVAL * 4;
CONST INT g_i_DISPLAY_HEIGHT = 48;

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

CONST CHAR* g_szCurrentSkin = "square_blue";

void HandleButtonPress(HWND hwnd, WPARAM wParam, BOOL isKeyDown)
{
    HWND hButton = NULL;
    BOOL shiftPressed = GetKeyState(VK_SHIFT) < 0;

    if (shiftPressed && wParam == 0x38)
    {
        hButton = GetDlgItem(hwnd, IDC_BUTTON_ASTER);
    }
    else if (wParam >= '0' && wParam <= '9')
    {
        hButton = GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0);
    }
    else if (wParam >= 0x60 && wParam <= 0x69)
    {
        hButton = GetDlgItem(hwnd, wParam - 0x60 + IDC_BUTTON_0);
    }
    else
    {
        switch (wParam)
        {
        case VK_ADD:
        case VK_OEM_PLUS:
            hButton = GetDlgItem(hwnd, IDC_BUTTON_PLUS);
            break;
        case VK_SUBTRACT:
        case VK_OEM_MINUS:
            hButton = GetDlgItem(hwnd, IDC_BUTTON_MINUS);
            break;
        case VK_MULTIPLY:
            hButton = GetDlgItem(hwnd, IDC_BUTTON_ASTER);
            break;
        case VK_DIVIDE:
        case VK_OEM_2:
            hButton = GetDlgItem(hwnd, IDC_BUTTON_SLASH);
            break;
        case VK_DECIMAL:
        case VK_OEM_PERIOD:
            hButton = GetDlgItem(hwnd, IDC_BUTTON_POINT);
            break;
        case VK_BACK:
            hButton = GetDlgItem(hwnd, IDC_BUTTON_BSP);
            break;
        case VK_ESCAPE:
            hButton = GetDlgItem(hwnd, IDC_BUTTON_CLR);
            break;
        case VK_RETURN:
            hButton = GetDlgItem(hwnd, IDC_BUTTON_EQUAL);
            break;
        }
    }

    if (hButton)
    {
        SendMessage(hButton, BM_SETSTATE, isKeyDown ? TRUE : FALSE, 0);
        if (!isKeyDown)
        {
            SendMessage(hwnd, WM_COMMAND, LOWORD(GetDlgCtrlID(hButton)), 0);
        }
    }
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

VOID SetSkin(HWND hwnd, CONST CHAR* skin);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    //1) Регистраци класса окна:

    WNDCLASSEX wClass;
    ZeroMemory(&wClass, sizeof(wClass));

    wClass.style = 0;
    wClass.cbSize = sizeof(wClass);
    wClass.cbClsExtra = 0;
    wClass.cbWndExtra = 0;

    wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    //wClass.hbrBackground = CreateSolidBrush(RGB(0,0,200));

    wClass.hInstance = hInstance;
    wClass.lpszClassName = g_sz_CLASS_NAME;
    wClass.lpfnWndProc = (WNDPROC)WndProc;
    wClass.lpszMenuName = NULL;

    if (!RegisterClassEx(&wClass))
    {
        MessageBox(NULL, "Class registration failed!", NULL, MB_OK | MB_ICONERROR);
        return 0;
    }

    //2) Создание окна:
    int xPos = (GetSystemMetrics(SM_CXSCREEN) - g_i_WINDOW_WIDTH) / 2; // X-координата центра экрана
    int yPos = (GetSystemMetrics(SM_CYSCREEN) - g_i_WINDOW_HEIGHT) / 2; // Y-координата центра экрана

    HWND hwnd = CreateWindowEx
    (
        NULL,
        g_sz_CLASS_NAME,
        g_sz_CLASS_NAME,
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
        xPos, yPos,                             // Установка позиции: X, Y
        g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGHT,    // Установка размеров: Width, Height
        NULL,
        NULL,
        hInstance,
        NULL
    );
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    //3) Запуск цикла сообщений:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static DOUBLE a = DBL_MIN;
    static DOUBLE b = DBL_MIN;
    static INT operation = 0;
    static BOOL input = FALSE;
    static BOOL input_operation = FALSE;
    static BOOL point_entered = FALSE; // Флаг для отслеживания ввода точки
    switch (uMsg)
    {
    case WM_CREATE:
    {
        //AllocConsole();
        //freopen("CONOUT$", "w", stdout);

        HWND hEdit = CreateWindowEx
        (
            NULL, "Edit", "0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            10, 10,
            g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
            hwnd,
            (HMENU)IDC_EDIT_DISPLAY,
            GetModuleHandle(NULL),
            NULL
        );

        {
            g_szCurrentSkin = "square_blue";
            hbrMainBackground = CreateSolidBrush(RGB(0, 102, 204)); // Синий фон
            SetSkin(hwnd, g_szCurrentSkin);
            InvalidateRect(hwnd, NULL, TRUE); // Перерисовка окна
        }

        // Подключение пользовательского шрифта
        AddFontResourceEx("Fonts\\digital-7.ttf", FR_PRIVATE, 0);
        HFONT hFont = CreateFont
        (
            g_i_DISPLAY_HEIGHT - 2, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, "digital-7"
        );
        SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

        //TODO:    Button Icons.
        CHAR sz_digit[2] = "0";
        for (int i = 6; i >= 0; i -= 3)
        {
            for (int j = 0; j < 3; j++)
            {
                sz_digit[0] = 49 + i + j;    //49 - ASCII-код единицы.
                CreateWindowEx
                (
                    NULL, "Button", sz_digit,
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
                    g_i_BUTTON_START_X + j * (g_i_BUTTON_SIZE + g_i_INTERVAL),
                    g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (2 - i / 3),
                    g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
                    hwnd,
                    (HMENU)(IDC_BUTTON_1 + i + j),
                    GetModuleHandle(NULL),
                    NULL
                );
            }
        }

        HWND hButton0 = CreateWindowEx
        (
            NULL, "Button", "0",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
            g_i_BUTTON_START_X,
            g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
            g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
            hwnd,
            (HMENU)IDC_BUTTON_0,
            GetModuleHandle(NULL),
            NULL
        );

        HBITMAP hBitmap0 = (HBITMAP)LoadImage
        (
            GetModuleHandle(NULL),
            "ButtonsBMP\\square_blue\\button_0.bmp",
            IMAGE_BITMAP,
            g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
            LR_LOADFROMFILE
        );
        SendMessage(hButton0, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap0);

        CreateWindowEx
        (
            NULL, "Button", ".",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
            g_i_BUTTON_START_X + g_i_BUTTON_DOUBLE_SIZE + g_i_INTERVAL,
            g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
            g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
            hwnd,
            (HMENU)IDC_BUTTON_POINT,
            GetModuleHandle(NULL),
            NULL
        );

        for (int i = 0; i < 4; i++)
        {
            CreateWindowEx
            (
                NULL, "Button", g_OPERATIONS[i],
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
                g_i_OPERATION_BUTTON_START_X,
                g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (3 - i),
                g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
                hwnd,
                (HMENU)(IDC_BUTTON_PLUS + i),
                GetModuleHandle(NULL),
                NULL
            );
        }

        CreateWindowEx
        (
            NULL, "Button", "<-",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
            g_i_CONTROL_BUTTON_START_X,
            g_i_CONTROL_BUTTON_START_Y,
            g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
            hwnd,
            (HMENU)IDC_BUTTON_BSP,
            GetModuleHandle(NULL),
            NULL
        );
        CreateWindowEx
        (
            NULL, "Button", "C",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
            g_i_CONTROL_BUTTON_START_X,
            g_i_CONTROL_BUTTON_START_Y + g_i_BUTTON_SIZE + g_i_INTERVAL,
            g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
            hwnd,
            (HMENU)IDC_BUTTON_CLR,
            GetModuleHandle(NULL),
            NULL
        );
        CreateWindowEx
        (
            NULL, "Button", "=",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
            g_i_CONTROL_BUTTON_START_X,
            g_i_CONTROL_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 2,
            g_i_BUTTON_SIZE, g_i_BUTTON_DOUBLE_SIZE,
            hwnd,
            (HMENU)IDC_BUTTON_EQUAL,
            GetModuleHandle(NULL),
            NULL
        );
        SetSkin(hwnd, "square_blue");
    }
    break;

    case WM_ERASEBKGND:
    {
        if (hbrMainBackground)
        {
            HDC hdc = (HDC)wParam;
            RECT rc;
            GetClientRect(hwnd, &rc);
            FillRect(hdc, &rc, hbrMainBackground); // Заливаем фон цветом
            return 0;
        }
        break;
    }

    case WM_CTLCOLOREDIT:
    {
        HDC hdcEdit = (HDC)wParam;
        HWND hwndEdit = (HWND)lParam;

        if (GetDlgCtrlID(hwndEdit) == IDC_EDIT_DISPLAY)
        {
            if (strcmp(g_szCurrentSkin, "metal_mistral") == 0)
            {
                SetTextColor(hdcEdit, RGB(255, 255, 0)); // Желтый текст
                SetBkColor(hdcEdit, RGB(30, 30, 30));    // Серый фон
                hbrMainBackground = CreateSolidBrush(RGB(50, 50, 50));
            }
            else if (strcmp(g_szCurrentSkin, "square_blue") == 0)
            {
                SetTextColor(hdcEdit, RGB(255, 0, 0));   // Красный текст
                SetBkColor(hdcEdit, RGB(0, 30, 62));     // Синий фон
                hbrMainBackground = CreateSolidBrush(RGB(0, 102, 204));
            }
            return (INT_PTR)hbrMainBackground;
        }
        break;
    }

    case WM_CONTEXTMENU:
    {
        // Создание подменю для выбора скинов
        HMENU hSubmenuSkins = CreatePopupMenu();
        InsertMenu(hSubmenuSkins, 0, MF_BYPOSITION | MF_STRING, IDM_SKIN_METAL_MISTRAL, "Metal Mistral");
        InsertMenu(hSubmenuSkins, 1, MF_BYPOSITION | MF_STRING, IDM_SKIN_SQUARE_BLUE, "Square Blue");

        // Создание основного меню
        HMENU hMenu = CreatePopupMenu();
        InsertMenu(hMenu, 0, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hSubmenuSkins, "Skins");
        InsertMenu(hMenu, 1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
        InsertMenu(hMenu, 2, MF_BYPOSITION | MF_STRING, IDR_EXIT, "Exit");

        // Отображение контекстного меню
        int cmd = TrackPopupMenu
        (
            hMenu,
            TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD,
            LOWORD(lParam), HIWORD(lParam),
            0,
            hwnd,
            NULL
        );

        // Обработка выбора пользователя
        switch (cmd)
        {
        case IDM_SKIN_METAL_MISTRAL:
            g_szCurrentSkin = "metal_mistral";
            hbrMainBackground = CreateSolidBrush(RGB(70, 70, 70)); // Серый фон
            SetSkin(hwnd, g_szCurrentSkin);
            InvalidateRect(hwnd, NULL, TRUE); // Перерисовка окна
            break;

        case IDM_SKIN_SQUARE_BLUE:
            g_szCurrentSkin = "square_blue";
            hbrMainBackground = CreateSolidBrush(RGB(0, 102, 204)); // Синий фон
            SetSkin(hwnd, g_szCurrentSkin);
            InvalidateRect(hwnd, NULL, TRUE); // Перерисовка окна
            break;

        case IDR_EXIT:
            DestroyWindow(hwnd); // Завершение работы программы
            break;
        }

        // Удаление меню из памяти
        DestroyMenu(hSubmenuSkins);
        DestroyMenu(hMenu);
    }
    break;

    case WM_COMMAND:
    {
        CONST INT SIZE = 256;
        CHAR sz_display[SIZE] = {};
        CHAR sz_digit[2] = {};
        HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
        SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);

        // Обработка кнопок с цифрами
        if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
        {
            if (!input && !input_operation)
            {
                SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLR), 0);
                //ZeroMemory(sz_display, SIZE);
                sz_display[0] = 0;
            }
            if (!input && input_operation) sz_display[0] = 0;

            sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
            if (strlen(sz_display) == 1 && sz_display[0] == '0' && !point_entered)
                sz_display[0] = sz_digit[0];
            else
                strcat(sz_display, sz_digit);
            SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);

            input = TRUE;
            //input_operation = FALSE;
        }

        // Точка
        if (LOWORD(wParam) == IDC_BUTTON_POINT)
        {
            if (!point_entered)
            {
                strcat(sz_display, strlen(sz_display) == 0 ? "0." : ".");
                SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
                point_entered = TRUE;
                input = TRUE;
            }
        }

        // Backspace
        //if (LOWORD(wParam) == IDC_EDIT_DISPLAY && HIWORD(wParam) == EN_SETFOCUS)SetFocus(hwnd);
        if (LOWORD(wParam) == IDC_BUTTON_BSP)
        {
            if (strlen(sz_display) == 1)sz_display[0] = '0';
            else sz_display[strlen(sz_display) - 1] = 0;
            SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
        }

        // Очистка дисплея
        if (LOWORD(wParam) == IDC_BUTTON_CLR)
        {
            a = b = DBL_MIN;
            operation = 0;
            input = FALSE;
            input_operation = FALSE;
            //strcpy(sz_display, "0");
            //sz_display[0] = 0;
            ZeroMemory(sz_display, SIZE);
            SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"0");
        }

        // Операции (+, -, *, /)
        if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
        {
            //SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
            if (a == DBL_MIN)a = atof(sz_display);
            //input = FALSE;
            if (input_operation)SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
            operation = LOWORD(wParam);
            input = FALSE;
            input_operation = TRUE;
        }

        // Равно
        if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
        {
            //if(b==DBL_MIN && !input)
            if (input || b == DBL_MIN && !input)b = atof(sz_display);
            input = FALSE;
            switch (operation)
            {
            case IDC_BUTTON_PLUS:    a += b;    break;
            case IDC_BUTTON_MINUS:    a -= b;    break;
            case IDC_BUTTON_ASTER:    a *= b;    break;
            case IDC_BUTTON_SLASH:    a /= b;    break;
            }
            //input = FALSE;
            input_operation = FALSE;
            if (a == DBL_MIN)strcpy(sz_display, "0");
            else sprintf(sz_display, "%g", a);
            SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
        }
        SetFocus(hwnd);
    }
    break;

    case WM_KEYDOWN:
    {
        HandleButtonPress(hwnd, wParam, TRUE);
    }
    break;

    case WM_KEYUP:
    {
        HandleButtonPress(hwnd, wParam, FALSE);
    }
    break;

    case WM_DESTROY:
    {
        if (hbrMainBackground)
        {
            DeleteObject(hbrMainBackground);
        }
        PostQuitMessage(0);
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    default:    return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

CONST CHAR* g_BUTTON_FILENAME[] =
{
    "button_0",
    "button_1",
    "button_2",
    "button_3",
    "button_4",
    "button_5",
    "button_6",
    "button_7",
    "button_8",
    "button_9",
    "button_point",
    "button_plus"    ,
    "button_minus",
    "button_aster",
    "button_slash",
    "button_bsp"    ,
    "button_clr",
    "button_equal"
};

VOID SetSkin(HWND hwnd, CONST CHAR* skin)
{
    CHAR sz_filename[FILENAME_MAX]{};
    for (INT i = 0; i < 18; i++)
    {
        HWND hButton = GetDlgItem(hwnd, IDC_BUTTON_0 + i);
        sprintf(sz_filename, "ButtonsBMP\\%s\\%s.bmp", skin, g_BUTTON_FILENAME[i]);
        HBITMAP bmpButton = (HBITMAP)LoadImage
        (
            GetModuleHandle(NULL),
            sz_filename,
            IMAGE_BITMAP,
            i + IDC_BUTTON_0 == IDC_BUTTON_0 ? g_i_BUTTON_DOUBLE_SIZE : g_i_BUTTON_SIZE,
            i + IDC_BUTTON_0 == IDC_BUTTON_EQUAL ? g_i_BUTTON_DOUBLE_SIZE : g_i_BUTTON_SIZE,
            LR_LOADFROMFILE
        );
        std::cout << sz_filename << std::endl;
        SendMessage(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmpButton);
    }
}