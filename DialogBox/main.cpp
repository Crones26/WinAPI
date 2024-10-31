#include<Windows.h>
#include"resource.h"

//#define LOGIN_PASS_1

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CONST CHAR* szDefaultTextLogin = "������� ��� ������������";
CONST CHAR* szDefaultTextPassword = "������� ������";

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
    return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hEditLogin;    // ���������� ���� 'Login'
    static HWND hEditPassword; // ���������� ���� 'Password'

    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
        SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

        // �������� ����������� ��� ����� 'Login' � 'Password' � ������������� �����-�����������
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
            //������� GetDlgItem() ���������� HWND ��������� �������� ���� �� ��� Resource ID
            //HWND - Handle to Window (���������� ����) - ��� �����, ��� ������ �������� ����� ���������� � ����.
            CONST INT SIZE = 256;
            CHAR sz_buffer[SIZE] = {};
            SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
            SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
        }
        break;

        case IDOK:
            MessageBox(hwnd, "���� ������ ������ ��!", "Info", MB_OK | MB_ICONINFORMATION);
            EndDialog(hwnd, 0);
            break;

        case IDCANCEL:
            EndDialog(hwnd, 0);
            break;
        }
#ifdef LOGIN_PASS_1
        // ��������� ��������� � ������ ������ ��� ����� 'Login' � 'Password'
        if (HIWORD(wParam) == EN_SETFOCUS)
        {
            if ((HWND)lParam == hEditLogin)
            {
                // ������� ���� 'Login' ��� �����������
                SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)"");
            }
            else if ((HWND)lParam == hEditPassword)
            {
                // ������� ���� 'Password' ��� �����������
                SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)"");
            }
        }
        else if (HIWORD(wParam) == EN_KILLFOCUS)
        {
            if ((HWND)lParam == hEditLogin)
            {
                // ���� ���� 'Login' ������ ��� ������ ������, ������������� �����-�����������
                CHAR szText[256] = {};
                SendMessage(hEditLogin, WM_GETTEXT, 256, (LPARAM)szText);
                if (strlen(szText) == 0)
                {
                    SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)szDefaultTextLogin);
                }
            }
            else if ((HWND)lParam == hEditPassword)
            {
                // ���� ���� 'Password' ������ ��� ������ ������, ������������� �����-�����������
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

        if (HIWORD(wParam) == EN_SETFOCUS)
        {
            HWND hEdit = (HWND)lParam;
            const CHAR* szDefaultText = nullptr;

            // ����������, ����� ���� �������� �����, � ������������� ��������������� �����-�����������
            if (hEdit == hEditLogin) szDefaultText = szDefaultTextLogin;
            else if (hEdit == hEditPassword) szDefaultText = szDefaultTextPassword;
            if (szDefaultText)
            {
                CHAR szText[256] = {};
                SendMessage(hEdit, WM_GETTEXT, 256, (LPARAM)szText);

                // ������� ���� ������ ���� ��� �������� �����-�����������
                if (strcmp(szText, szDefaultText) == 0)
                {
                    SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
                }
            }
        }

        else if (HIWORD(wParam) == EN_KILLFOCUS)
        {
            HWND hEdit = (HWND)lParam;
            const CHAR* szDefaultText = nullptr;

            // ����������, ����� ���� ������ ����� � ������������� ��������������� �����-�����������
            if (hEdit == hEditLogin) szDefaultText = szDefaultTextLogin;
            else if (hEdit == hEditPassword) szDefaultText = szDefaultTextPassword;
            if (szDefaultText)
            {
                CHAR szText[256] = {};
                SendMessage(hEdit, WM_GETTEXT, 256, (LPARAM)szText);

                // ������������� �����-�����������, ���� ���� ������
                if (strlen(szText) == 0)
                {
                    SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)szDefaultText);
                }
            }
        }
        break;

    case WM_CLOSE:
        EndDialog(hwnd, 0);
        break;
    }
    return FALSE;
}