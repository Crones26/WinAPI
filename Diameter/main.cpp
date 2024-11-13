#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cmath>
#include <cstdio>
#include "resource.h"

// Определяем идентификаторы для полей ввода и кнопок
#define IDC_EDIT_OUTER_DIAMETER 1001
#define IDC_EDIT_INNER_DIAMETER 1002
#define IDC_EDIT_WALL_THICKNESS 1003
#define IDC_EDIT_LENGTH 1004
#define IDC_EDIT_WEIGHT 1005
#define IDC_STATIC_RESULT 1006
#define IDC_STATIC_ONE_METER_RESULT 1007
#define IDC_CLEAR_BUTTON 1008
#define IDC_CALCULATE_BUTTON 1009

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "CalculatorWindow";
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

const double M_PI = 3.14159265358979323846;
const double density = 7850;  // Плотность стали (кг/м³)

double outerDiameter = 0;
double innerDiameter = 0;
double wallThickness = 0;
double length = 0;
double weight = 0;

bool updating = false;  // Глобальная переменная для предотвращения зацикливания

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    WNDCLASSEX wc = { sizeof(wc) };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = g_sz_MY_WINDOW_CLASS;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));  // Установка основной иконки
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));  // Установка маленькой иконки
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

    // Регистрируем класс окна и проверяем, успешна ли регистрация
    if (!RegisterClassExA(&wc))
    {
        MessageBox(NULL, "Не удалось зарегистрировать класс окна.", "Ошибка", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Определите размеры окна
    int windowWidth = 500;
    int windowHeight = 300;

    // Получите размер экрана
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Вычислите координаты для центрирования окна
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

    HWND hwnd = CreateWindowExA(
        0, g_sz_MY_WINDOW_CLASS, "Калькулятор", WS_OVERLAPPEDWINDOW,
        x, y, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

    // Проверяем, успешно ли создано окно
    if (!hwnd)
    {
        MessageBox(NULL, "Не удалось создать окно.", "Ошибка", MB_OK | MB_ICONERROR);
        return 1;
    }

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

void AddTextBox(HWND hwnd, int id, int x, int y, int width, int height)
{
    CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT,
        x, y, width, height, hwnd, (HMENU)id, NULL, NULL);
}

void AddLabel(HWND hwnd, const char* text, int x, int y, int width, int height)
{
    CreateWindowA("STATIC", text, WS_VISIBLE | WS_CHILD,
        x, y, width, height, hwnd, NULL, NULL, NULL);
}

void AddButton(HWND hwnd, const char* text, int x, int y, int width, int height, int id)
{
    CreateWindowA("BUTTON", text, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        x, y, width, height, hwnd, (HMENU)id, NULL, NULL);
}

void AddControls(HWND hwnd)
{
    // Размеры полей ввода и меток
    int editWidth = 100;
    int editHeight = 20;
    int labelWidth = 100;
    int labelHeight = 20;
    int spacing = 20;  // Отступы между блоками
    int rowSpacing = 50; // Вертикальный отступ между строками

    // Первый ряд - Диаметры и толщина стенки
    AddLabel(hwnd, "Наружный(D):", 10, 10, labelWidth, labelHeight);
    AddTextBox(hwnd, IDC_EDIT_OUTER_DIAMETER, 10, 30, editWidth, editHeight);

    AddLabel(hwnd, "Внутренний(D):", 10 + editWidth + spacing, 10, labelWidth, labelHeight);
    AddTextBox(hwnd, IDC_EDIT_INNER_DIAMETER, 10 + editWidth + spacing, 30, editWidth, editHeight);

    AddLabel(hwnd, "Стенка(t):", 10 + 2 * (editWidth + spacing), 10, labelWidth, labelHeight);
    AddTextBox(hwnd, IDC_EDIT_WALL_THICKNESS, 10 + 2 * (editWidth + spacing), 30, editWidth, editHeight);

    // Второй ряд - Длина и вес
    AddLabel(hwnd, "Длина (м):", 10, 10 + rowSpacing, labelWidth, labelHeight);
    AddTextBox(hwnd, IDC_EDIT_LENGTH, 10, 30 + rowSpacing, editWidth, editHeight);

    AddLabel(hwnd, "Вес (кг):", 10 + editWidth + spacing, 10 + rowSpacing, labelWidth, labelHeight);
    AddTextBox(hwnd, IDC_EDIT_WEIGHT, 10 + editWidth + spacing, 30 + rowSpacing, editWidth, editHeight);

    // Поле для отображения результата
    AddLabel(hwnd, "ИТОГО:", 130, 30 + 2 * rowSpacing, labelWidth, labelHeight);
    CreateWindowA("STATIC", "", WS_VISIBLE | WS_CHILD | SS_RIGHT, 200, 30 + 2 * rowSpacing, editWidth, editHeight, hwnd, (HMENU)IDC_STATIC_RESULT, NULL, NULL);

    // Поле для отображения результата одного погонного метра
    AddLabel(hwnd, "Погонный(М):", 10, 30 + 3 * rowSpacing, labelWidth, labelHeight);
    CreateWindowA("STATIC", "", WS_VISIBLE | WS_CHILD | SS_RIGHT, 130, 30 + 3 * rowSpacing, editWidth, editHeight, hwnd, (HMENU)IDC_STATIC_ONE_METER_RESULT, NULL, NULL);

    // Кнопка расчета
    AddButton(hwnd, "Рассчитать", 10 + 3 * (editWidth + spacing), 10 + 3 * rowSpacing, 100, 30, IDC_CALCULATE_BUTTON);

    // Кнопка очистки
    AddButton(hwnd, "Очистить", 10 + 3 * (editWidth + spacing), 10 + 4 * rowSpacing, 100, 30, IDC_CLEAR_BUTTON);
}

void ClearAllFields(HWND hwnd)
{
    // Очистить все текстовые поля
    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_OUTER_DIAMETER), "");
    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_INNER_DIAMETER), "");
    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_WALL_THICKNESS), "");
    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_LENGTH), "");
    SetWindowText(GetDlgItem(hwnd, IDC_EDIT_WEIGHT), "");
    SetWindowText(GetDlgItem(hwnd, IDC_STATIC_RESULT), "");
    SetWindowText(GetDlgItem(hwnd, IDC_STATIC_ONE_METER_RESULT), "");
}

void UpdateResults(HWND hwnd)
{
    char text[256];

    // Получаем значения наружного диаметра, внутреннего диаметра и толщины стенки
    GetWindowText(GetDlgItem(hwnd, IDC_EDIT_OUTER_DIAMETER), text, sizeof(text));
    outerDiameter = atof(text);

    GetWindowText(GetDlgItem(hwnd, IDC_EDIT_INNER_DIAMETER), text, sizeof(text));
    innerDiameter = atof(text);

    GetWindowText(GetDlgItem(hwnd, IDC_EDIT_WALL_THICKNESS), text, sizeof(text));
    wallThickness = atof(text);

    // Если хотя бы одно из полей пустое, вычисляем его значение
    if (outerDiameter != 0 && innerDiameter != 0 && wallThickness == 0)
    {
        wallThickness = (outerDiameter - innerDiameter) / 2;
        char thicknessText[256];
        sprintf(thicknessText, "%.2f", wallThickness);
        SetWindowText(GetDlgItem(hwnd, IDC_EDIT_WALL_THICKNESS), thicknessText);
    }

    if (outerDiameter != 0 && innerDiameter == 0 && wallThickness != 0)
    {
        innerDiameter = outerDiameter - 2 * wallThickness;
        char innerText[256];
        sprintf(innerText, "%.2f", innerDiameter);
        SetWindowText(GetDlgItem(hwnd, IDC_EDIT_INNER_DIAMETER), innerText);
    }

    if (outerDiameter == 0 && innerDiameter != 0 && wallThickness != 0)
    {
        outerDiameter = innerDiameter + 2 * wallThickness;
        char outerText[256];
        sprintf(outerText, "%.2f", outerDiameter);
        SetWindowText(GetDlgItem(hwnd, IDC_EDIT_OUTER_DIAMETER), outerText);
    }

    // Расчет одного погонного метра
    double innerDiameterCalculated = outerDiameter - 2 * wallThickness;
    double outerRadius = outerDiameter / 2 / 1000;
    double innerRadius = innerDiameterCalculated / 2 / 1000;

    double wallVolumePerMeter = M_PI * (pow(outerRadius, 2) - pow(innerRadius, 2));
    double weightPerMeter = wallVolumePerMeter * density;

    // Отображение результата одного погонного метра
    sprintf(text, "%.2f кг", weightPerMeter);
    SetWindowText(GetDlgItem(hwnd, IDC_STATIC_ONE_METER_RESULT), text);

    // Проверка поля веса
    GetWindowText(GetDlgItem(hwnd, IDC_EDIT_WEIGHT), text, sizeof(text));
    if (strlen(text) > 0)
    {
        weight = atof(text);

        // Расчет длины
        double length = weight / weightPerMeter;

        // Расчет общего веса
        double totalWeight = weightPerMeter * length;

        // Отображение только длины в поле "ИТОГО"
        sprintf(text, "%.2f м", length);
        SetWindowText(GetDlgItem(hwnd, IDC_STATIC_RESULT), text);
    }

    // Проверка поля длины
    GetWindowText(GetDlgItem(hwnd, IDC_EDIT_LENGTH), text, sizeof(text));
    if (strlen(text) > 0)
    {
        length = atof(text);

        // Расчет веса
        double totalWeight = weightPerMeter * length;

        // Отображение только веса в поле "ИТОГО"
        sprintf(text, "%.2f кг", totalWeight);
        SetWindowText(GetDlgItem(hwnd, IDC_STATIC_RESULT), text);
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
        if (LOWORD(wParam) == IDC_CALCULATE_BUTTON)
        {
            UpdateResults(hwnd);  // Выполняем расчет при нажатии на кнопку "Рассчитать"
        }

        if (LOWORD(wParam) == IDC_CLEAR_BUTTON)
        {
            ClearAllFields(hwnd);  // Очистка всех полей
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}