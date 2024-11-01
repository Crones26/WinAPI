#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_LOGIN_INVITATION[] = "Введите имя пользователя";
CONST CHAR g_sz_PASSWORD_INVITATION[] = "Введите пароль";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void HandleEditFocus(HWND hwnd, int controlId, const CHAR* invitationText, BOOL isPasswordField);
void TogglePasswordVisibility(HWND hwnd, BOOL showPassword);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
    return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static BOOL showPassword = FALSE; // Переменная для отслеживания состояния видимости пароля

    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
        SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

        HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
        HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);

        // Устанавливаем текст приглашения
        SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_LOGIN_INVITATION);
        SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)g_sz_PASSWORD_INVITATION);
    }
    break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_EDIT_LOGIN:
            if (HIWORD(wParam) == EN_SETFOCUS || HIWORD(wParam) == EN_KILLFOCUS)
                HandleEditFocus(hwnd, IDC_EDIT_LOGIN, g_sz_LOGIN_INVITATION, FALSE);
            break;
        case IDC_EDIT_PASSWORD:
            if (HIWORD(wParam) == EN_SETFOCUS || HIWORD(wParam) == EN_KILLFOCUS)
                HandleEditFocus(hwnd, IDC_EDIT_PASSWORD, g_sz_PASSWORD_INVITATION, TRUE);
            break;
        case IDC_BUTTON_COPY:
        {
            HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
            HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
            CONST INT SIZE = 256;
            CHAR sz_buffer[SIZE] = {};

            SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

            // Проверяем, чтобы не копировать текст приглашения
            if (strcmp(sz_buffer, g_sz_LOGIN_INVITATION) != 0)
            {
                SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);

                // Проверяем состояние флажка "Показать пароль"
                showPassword = IsDlgButtonChecked(hwnd, IDC_CHEK_SHOW_PASSWORD);
                TogglePasswordVisibility(hwnd, showPassword); // Обновляем видимость пароля
            }
        }
        break;
        case IDC_CHEK_SHOW_PASSWORD:
            // Переключаем видимость пароля при нажатии на флажок
            showPassword = !showPassword;
            TogglePasswordVisibility(hwnd, showPassword);
            break;
        case IDOK:
            MessageBox(hwnd, "Была нажата кнопка OK!", "Info", MB_OK | MB_ICONINFORMATION);
            EndDialog(hwnd, 0);
            break;
        case IDCANCEL:
            EndDialog(hwnd, 0);
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog(hwnd, 0);
        break;
    }
    return FALSE;
}

void HandleEditFocus(HWND hwnd, int controlId, const CHAR* invitationText, BOOL isPasswordField)
{
    CONST INT SIZE = 256;
    CHAR sz_buffer[SIZE] = {};
    HWND hEdit = GetDlgItem(hwnd, controlId);
    SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

    // Проверяем, если поле в фокусе и содержит ли текст приглашения
    if (GetFocus() == hEdit && strcmp(sz_buffer, invitationText) == 0)
    {
        SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");

        // Если это поле пароля и флажок "Показать пароль" не установлен, применяем маскирование
        if (isPasswordField)
        {
            BOOL showPassword = IsDlgButtonChecked(hwnd, IDC_CHEK_SHOW_PASSWORD);
            SendMessage(hEdit, EM_SETPASSWORDCHAR, showPassword ? 0 : '*', 0);
        }
    }
    // Если поле потеряло фокус и текст пустой, устанавливаем текст приглашения
    else if (GetFocus() != hEdit && strcmp(sz_buffer, "") == 0)
    {
        SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)invitationText);

        // Снимаем маскирование, если в поле пароля снова появляется текст приглашения
        if (isPasswordField)
        {
            SendMessage(hEdit, EM_SETPASSWORDCHAR, 0, 0);
        }
    }

    InvalidateRect(hEdit, NULL, TRUE);
}

void TogglePasswordVisibility(HWND hwnd, BOOL showPassword)
{
    HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
    CONST INT SIZE = 256;
    CHAR sz_buffer[SIZE] = {};

    // Получаем текст из поля пароля
    SendMessage(hEditPassword, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);

    // Проверяем, чтобы не изменять видимость, если в поле находится текст приглашения
    if (strcmp(sz_buffer, g_sz_PASSWORD_INVITATION) != 0)
    {
        // Если это не текст приглашения, переключаем видимость пароля
        SendMessage(hEditPassword, EM_SETPASSWORDCHAR, showPassword ? 0 : '*', 0);
    }
    else
    {
        // Если это текст приглашения, снимаем символ маскирования
        SendMessage(hEditPassword, EM_SETPASSWORDCHAR, 0, 0);
    }
    InvalidateRect(hEditPassword, NULL, TRUE);
}