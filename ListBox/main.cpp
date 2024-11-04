#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include "resource.h"

CONST CHAR* g_VALUES[] = { "This", "is", "my", "first", "List", "Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AddItemProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);
    return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
        SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

        HWND hList = GetDlgItem(hwnd, IDC_LISTBOX);
        for (int i = 0; i < sizeof(g_VALUES) / sizeof(g_VALUES[0]); i++)
        {
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)g_VALUES[i]);
        }
        SendMessage(hList, LB_SETCURSEL, 0, 0); // ���������� ������ ������� ��� ���������
    }
    break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:  // ��������� ������� ������ OK
        {
            CHAR sz_buffer[256] = {};
            HWND hList = GetDlgItem(hwnd, IDC_LISTBOX);
            INT i = SendMessage(hList, LB_GETCURSEL, 0, 0); // ��������� ������� ���������� ��������

            if (SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer) > 0)
            {
                CHAR sz_message[256] = {};
                sprintf(sz_message, "�� ������� ����� �%i �� ��������� \"%s\".", i, sz_buffer);
                MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                MessageBox(hwnd, "��� ���������� ��������.", "������", MB_OK | MB_ICONERROR);
            }
        }
        break;

        case IDC_ADD_BUTTON:  // ��������� ������ "��������"
        {
            CHAR sz_input[256] = {};
            if (DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ADDITEM), hwnd, AddItemProc, (LPARAM)sz_input) == IDOK)
            {
                HWND hList = GetDlgItem(hwnd, IDC_LISTBOX);
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_input); // ���������� ���������� ������ � ListBox
            }
        }
        break;

        case IDC_DELETE_BUTTON:  // ��������� ������ "�������"
        {
            HWND hList = GetDlgItem(hwnd, IDC_LISTBOX);
            INT i = SendMessage(hList, LB_GETCURSEL, 0, 0); // ��������� ������� ���������� ��������
            if (i != LB_ERR)
            {
                SendMessage(hList, LB_DELETESTRING, i, 0); // �������� ���������� �������� �� ListBox
            }
            else
            {
                MessageBox(hwnd, "������� �� ������.", "������", MB_OK | MB_ICONERROR);
            }
        }
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

BOOL CALLBACK AddItemProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static CHAR* sz_input;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        sz_input = (CHAR*)lParam;
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            // �������� ����� �� ���� �����
            GetDlgItemText(hwnd, IDC_EDIT1, sz_input, 256);

            // ���������, ������ �� ������
            if (strlen(sz_input) == 0)
            {
                MessageBox(hwnd, "������� ����� ��� ����������.", "������", MB_OK | MB_ICONERROR);
                return TRUE;  // �������� � ���������� ����
            }

            EndDialog(hwnd, IDOK);  // ��������� ������ ������ ���� ������ �� �����
            return TRUE;
        }
        
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}