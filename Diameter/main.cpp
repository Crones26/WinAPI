#include <iostream>
#include <cmath>
#include <locale>

#define M_PI 3.14159265358979323846

using namespace std;

void calculatePipeWeightByDiameters()
{
    double outerDiameter = 0.0, innerDiameter = 0.0;
    const double density = 7850; // Плотность стали по умолчанию в кг/м³

    cout << "Введите внешний диаметр трубы (мм): ";
    cin >> outerDiameter;

    cout << "Введите внутренний диаметр трубы (мм): ";
    cin >> innerDiameter;

    double outerRadius = outerDiameter / 2 / 1000; // перевод в метры
    double innerRadius = innerDiameter / 2 / 1000; // перевод в метры
    double length = 1; // длина трубы в метрах (погонный метр)

    double outerVolume = M_PI * pow(outerRadius, 2) * length;
    double innerVolume = M_PI * pow(innerRadius, 2) * length;
    double wallVolume = outerVolume - innerVolume;

    double weight = wallVolume * density;
    cout << "Вес погонного метра трубы: " << weight << " кг" << endl;
}

void calculatePipeWeightByInnerDiameterAndThickness()
{
    double innerDiameter = 0.0, wallThickness = 0.0;
    const double density = 7850; // Плотность стали по умолчанию в кг/м³

    cout << "Введите внутренний диаметр трубы (мм): ";
    cin >> innerDiameter;

    cout << "Введите толщину стенки трубы (мм): ";
    cin >> wallThickness;

    double outerDiameter = innerDiameter + 2 * wallThickness;
    double outerRadius = outerDiameter / 2 / 1000; // перевод в метры
    double innerRadius = innerDiameter / 2 / 1000; // перевод в метры
    double length = 1; // длина трубы в метрах (погонный метр)

    double outerVolume = M_PI * pow(outerRadius, 2) * length;
    double innerVolume = M_PI * pow(innerRadius, 2) * length;
    double wallVolume = outerVolume - innerVolume;

    double weight = wallVolume * density;
    cout << "Вес погонного метра трубы: " << weight << " кг" << endl;
}

void calculatePipeWeightWithThickness()
{
    double outerDiameter = 0.0, wallThickness = 0.0;
    const double density = 7850; // Плотность стали по умолчанию в кг/м³

    cout << "Введите внешний диаметр трубы (мм): ";
    cin >> outerDiameter;

    cout << "Введите толщину стенки трубы (мм): ";
    cin >> wallThickness;

    double innerDiameter = outerDiameter - 2 * wallThickness;
    double outerRadius = outerDiameter / 2 / 1000; // перевод в метры
    double innerRadius = innerDiameter / 2 / 1000; // перевод в метры
    double length = 1; // длина трубы в метрах (погонный метр)

    double outerVolume = M_PI * pow(outerRadius, 2) * length;
    double innerVolume = M_PI * pow(innerRadius, 2) * length;
    double wallVolume = outerVolume - innerVolume;

    double weight = wallVolume * density;
    cout << "Вес погонного метра трубы: " << weight << " кг" << endl;
}

void calculateWallThickness()
{
    double outerDiameter = 0.0, innerDiameter = 0.0;

    cout << "Введите внешний диаметр трубы (мм): ";
    cin >> outerDiameter;

    cout << "Введите внутренний диаметр трубы (мм): ";
    cin >> innerDiameter;

    double wallThickness = (outerDiameter - innerDiameter) / 2;
    cout << "Толщина стенки трубы: " << wallThickness << " мм" << endl;
}

void calculateOuterDiameter()
{
    double innerDiameter = 0.0, wallThickness = 0.0;

    cout << "Введите внутренний диаметр трубы (мм): ";
    cin >> innerDiameter;

    cout << "Введите толщину стенки трубы (мм): ";
    cin >> wallThickness;

    double outerDiameter = innerDiameter + 2 * wallThickness;
    cout << "Внешний диаметр трубы: " << outerDiameter << " мм" << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian"); // Устанавливаем локализацию для поддержки русского языка

    while (true)
    {
        int choice = 0;

        cout << "\nВыберите расчет:" << endl;
        cout << "1 - Найти вес погонного метра трубы (по внешнему и внутреннему диаметру)" << endl;
        cout << "2 - Найти вес погонного метра трубы (по внешнему диаметру и толщине стенки)" << endl;
        cout << "3 - Найти вес погонного метра трубы (по внутреннему диаметру и толщине стенки)" << endl;
        cout << "4 - Найти толщину стенки (по внешнему и внутреннему диаметру)" << endl;
        cout << "5 - Найти внешний диаметр (по внутреннему диаметру и толщине стенки)" << endl;
        cout << "0 - Выход из программы" << endl;
        cout << "Введите номер выбора: ";
        cin >> choice;

        if (choice == 0)
        {
            cout << "Выход из программы." << endl;
            break; // Выход из цикла и завершение программы
        }

        switch (choice)
        {
        case 1:
            calculatePipeWeightByDiameters();
            break;
        case 2:
            calculatePipeWeightWithThickness();
            break;
        case 3:
            calculatePipeWeightByInnerDiameterAndThickness();
            break;
        case 4:
            calculateWallThickness();
            break;
        case 5:
            calculateOuterDiameter();
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
            break;
        }
    }

    return 0;
}