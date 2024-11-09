#include<Windows.h>
#include <string>
#include <sstream>

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "My Window"; // ��� ������ ����

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    // 1) ����������� ������ ����:
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.style = 0;
    wc.cbSize = sizeof(wc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;

    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    wc.hInstance = hInstance;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_sz_MY_WINDOW_CLASS;
    wc.lpfnWndProc = (WNDPROC)WndProc;

    if (!RegisterClassExA(&wc))
    {
        MessageBoxA(NULL, "Class registration failed", NULL, MB_OK | MB_ICONERROR);
        return 0;
    }

    // �������� ������� ������
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // ��������� ������� ���� ��� 75% ������ � ���������� ���
    int windowWidth = screenWidth * 3 / 4;
    int windowHeight = screenHeight * 3 / 4;
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    // 2) �������� ����:
    HWND hwnd = CreateWindowExA
    (
        0,                      // ExStyles
        g_sz_MY_WINDOW_CLASS,    // Class name
        "My Window",             // Initial window title
        WS_OVERLAPPEDWINDOW,     // Window style
        posX, posY,              // Window position
        windowWidth, windowHeight, // Window size
        NULL,                    // Parent Window
        NULL,                    // Main menu ResourceID
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);	//������ ����� ����������� ���� 
    //(���������� �� ���� �����, �������� � ����, �������� � ������ �����)
    UpdateWindow(hwnd);			//������������� ����.

    //3) ������ ����� ���������:
    MSG msg;	//������� ���������
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
    case WM_CREATE:
        break;

    case WM_MOVE:
    {
        // �������� ���������� ���� ��� �����������
        int xPos = (int)(short)LOWORD(lParam);
        int yPos = (int)(short)HIWORD(lParam);

        // ��������� ��������� ���� � ������ ������������
        std::ostringstream title;
        title << "My Window - Position: (" << xPos << ", " << yPos << ")";
        SetWindowTextA(hwnd, title.str().c_str());
        break;
    }

    case WM_SIZE:
    {
        // �������� ������� ���� ��� ��������� �������
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        // ��������� ��������� ���� � ������ ���������
        std::ostringstream title;
        title << "My Window - Size: (" << width << "x" << height << ")";
        SetWindowTextA(hwnd, title.str().c_str());
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    default:
        return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}