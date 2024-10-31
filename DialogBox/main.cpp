#include<Windows.h>
#include"resource.h"

//#define LOGIN_PASS_1

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CONST CHAR* szDefaultTextLogin = "Введите имя пользователя";
CONST CHAR* szDefaultTextPassword = "Введите пароль";

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
    return 0;
}

// Функция для обработки фокуса на полях ввода
void HandleFocus(HWND hEdit, const CHAR* szDefaultText, bool isSetFocus)
{
    CHAR szText[256] = {};
    SendMessage(hEdit, WM_GETTEXT, 256, (LPARAM)szText);

    if (isSetFocus)
    {
        // Очищаем поле только если оно содержит текст-приглашение
        if (strcmp(szText, szDefaultText) == 0)
        {
            SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
        }
    }
    else
    {
        // Устанавливаем текст-приглашение, если поле пустое
        if (strlen(szText) == 0)
        {
            SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)szDefaultText);
        }
    }
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hEditLogin;    // Дескриптор поля 'Login'
    static HWND hEditPassword; // Дескриптор поля 'Password'

    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
        SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

        // Получаем дескрипторы для полей 'Login' и 'Password' и устанавливаем текст-приглашение
        hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
        SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)szDefaultTextLogin);

        hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
        SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)szDefaultTextPassword);
    }
    break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON_COPY:
        {
            HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
            // Функция GetDlgItem() возвращает HWND дочернего элемента окна по его Resource ID
            // HWND - Handle to Window (Дескриптор окна) - это число, при помощи которого можно обратиться к окну.
            CONST INT SIZE = 256;
            CHAR sz_buffer[SIZE] = {};
            SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
            SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
        }
        break;

        case IDOK:
            MessageBox(hwnd, "Была нажата кнопка ОК!", "Info", MB_OK | MB_ICONINFORMATION);
            EndDialog(hwnd, 0);
            break;

        case IDCANCEL:
            EndDialog(hwnd, 0);
            break;
        }

#ifdef LOGIN_PASS_1
        // Закомментированная логика обработки получения и потери фокуса для полей 'Login' и 'Password'
        if (HIWORD(wParam) == EN_SETFOCUS)
        {
            if ((HWND)lParam == hEditLogin)
            {
                // Очищаем поле 'Login' при фокусировке
                SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)"");
            }
            if ((HWND)lParam == hEditPassword)
            {
                // Очищаем поле 'Password' при фокусировке
                SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)"");
            }
        }
        if (HIWORD(wParam) == EN_KILLFOCUS)
        {
            if ((HWND)lParam == hEditLogin)
            {
                // Если поле 'Login' пустое при потере фокуса, устанавливаем текст-приглашение
                CHAR szText[256] = {};
                SendMessage(hEditLogin, WM_GETTEXT, 256, (LPARAM)szText);
                if (strlen(szText) == 0)
                {
                    SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)szDefaultTextLogin);
                }
            }
            if ((HWND)lParam == hEditPassword)
            {
                // Если поле 'Password' пустое при потере фокуса, устанавливаем текст-приглашение
                CHAR szText[256] = {};
                SendMessage(hEditPassword, WM_GETTEXT, 256, (LPARAM)szText);
                if (strlen(szText) == 0)
                {
                    SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)szDefaultTextPassword);
                }
            }
        }
        break;
#endif // LOGIN_PASS_1

        // Для обработки фокуса 'Login' и 'Password'
        if (HIWORD(wParam) == EN_SETFOCUS || HIWORD(wParam) == EN_KILLFOCUS)
        {
            HWND hEdit = (HWND)lParam;
            const CHAR* szDefaultText = (hEdit == hEditLogin) ? szDefaultTextLogin :
                (hEdit == hEditPassword) ? szDefaultTextPassword : nullptr;

            if (szDefaultText)
            {
                // Вызов функции для обработки фокуса
                HandleFocus(hEdit, szDefaultText, HIWORD(wParam) == EN_SETFOCUS);
            }
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        break;
    }
    return FALSE;
}