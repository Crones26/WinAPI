#include <iostream>
#include <cmath>
#include <locale>
#include <string>

#define M_PI 3.14159265358979323846

using namespace std;

// Функция для ввода числа с заменой запятой на точку
double inputDouble(const string& prompt)
{
    string input;
    cout << prompt;
    getline(cin, input);

    // Заменяем запятые на точки
    for (char& c : input)
    {
        if (c == ',') c = '.';
    }

    // Преобразуем строку в число
    return stod(input);
}

// Расчет веса трубы по внешнему диаметру и толщине стенки
void calculatePipeWeightWithThickness()
{
    const double density = 7850;
    double outerDiameter = inputDouble("Введите внешний диаметр трубы (мм): ");
    double wallThickness = inputDouble("Введите толщину стенки трубы (мм): ");
    double length = inputDouble("Введите длину труб (м): ");

    double innerDiameter = outerDiameter - 2 * wallThickness;
    double outerRadius = outerDiameter / 2 / 1000;
    double innerRadius = innerDiameter / 2 / 1000;

    double outerVolume = M_PI * pow(outerRadius, 2) * length;
    double innerVolume = M_PI * pow(innerRadius, 2) * length;
    double wallVolume = outerVolume - innerVolume;
    
    double weight = wallVolume * density;
    cout << "Вес труб длиной " << length << " м: " << weight << " кг" << endl;
}

// Расчет веса трубы по внешнему и внутреннему диаметру
void calculatePipeWeightByDiameters()
{
    const double density = 7850;
    double outerDiameter = inputDouble("Введите внешний диаметр трубы (мм): ");
    double innerDiameter = inputDouble("Введите внутренний диаметр трубы (мм): ");
    double length = inputDouble("Введите длину труб (м): ");

    double outerRadius = outerDiameter / 2 / 1000;
    double innerRadius = innerDiameter / 2 / 1000;

    double outerVolume = M_PI * pow(outerRadius, 2) * length;
    double innerVolume = M_PI * pow(innerRadius, 2) * length;
    double wallVolume = outerVolume - innerVolume;

    double weight = wallVolume * density;
    cout << "Вес труб длиной " << length << " м: " << weight << " кг" << endl;
}

// Расчет веса трубы по внутреннему диаметру и толщине стенки
void calculatePipeWeightByInnerDiameterAndThickness()
{
    const double density = 7850;
    double innerDiameter = inputDouble("Введите внутренний диаметр трубы (мм): ");
    double wallThickness = inputDouble("Введите толщину стенки трубы (мм): ");
    double length = inputDouble("Введите длину труб (м): ");

    double outerDiameter = innerDiameter + 2 * wallThickness;
    double outerRadius = outerDiameter / 2 / 1000;
    double innerRadius = innerDiameter / 2 / 1000;

    double outerVolume = M_PI * pow(outerRadius, 2) * length;
    double innerVolume = M_PI * pow(innerRadius, 2) * length;
    double wallVolume = outerVolume - innerVolume;

    double weight = wallVolume * density;
    cout << "Вес труб длиной " << length << " м: " << weight << " кг" << endl;
}

// Расчет длины трубы по внешнему диаметру и толщине стенки
void calculateLengthByOuterDiameterAndThickness()
{
    const double density = 7850;
    double outerDiameter = inputDouble("Введите внешний диаметр трубы (мм): ");
    double wallThickness = inputDouble("Введите толщину стенки трубы (мм): ");
    double weight = inputDouble("Введите вес труб (кг): ");

    double innerDiameter = outerDiameter - 2 * wallThickness;
    double outerRadius = outerDiameter / 2 / 1000;
    double innerRadius = innerDiameter / 2 / 1000;

    double wallVolumePerMeter = M_PI * (pow(outerRadius, 2) - pow(innerRadius, 2));
    double length = weight / (density * wallVolumePerMeter);

    cout << "Длина труб: " << length << " м" << endl;
}

// Расчет длины трубы по внешнему и внутреннему диаметру
void calculateLengthByOuterAndInnerDiameter()
{
    const double density = 7850;
    double outerDiameter = inputDouble("Введите внешний диаметр трубы (мм): ");
    double innerDiameter = inputDouble("Введите внутренний диаметр трубы (мм): ");
    double weight = inputDouble("Введите вес труб (кг): ");

    double outerRadius = outerDiameter / 2 / 1000;
    double innerRadius = innerDiameter / 2 / 1000;

    double wallVolumePerMeter = M_PI * (pow(outerRadius, 2) - pow(innerRadius, 2));
    double length = weight / (density * wallVolumePerMeter);

    cout << "Длина труб: " << length << " м" << endl;
}

// Расчет длины трубы по внутреннему диаметру и толщине стенки
void calculateLengthByInnerDiameterAndThickness()
{
    const double density = 7850;
    double innerDiameter = inputDouble("Введите внутренний диаметр трубы (мм): ");
    double wallThickness = inputDouble("Введите толщину стенки трубы (мм): ");
    double weight = inputDouble("Введите вес труб (кг): ");

    double innerRadius = innerDiameter / 2 / 1000;
    double outerRadius = innerRadius + wallThickness / 1000;

    double wallVolumePerMeter = M_PI * (pow(outerRadius, 2) - pow(innerRadius, 2));
    double length = weight / (density * wallVolumePerMeter);

    cout << "Длина труб: " << length << " м" << endl;
}

// Расчет толщины стенки по внешнему и внутреннему диаметру
void calculateWallThickness()
{
    double outerDiameter = inputDouble("Введите внешний диаметр трубы (мм): ");
    double innerDiameter = inputDouble("Введите внутренний диаметр трубы (мм): ");

    double wallThickness = (outerDiameter - innerDiameter) / 2;
    cout << "Толщина стенки труб: " << wallThickness << " мм" << endl;
}

// Расчет внешнего диаметра по внутреннему диаметру и толщине стенки
void calculateOuterDiameter()
{
    double innerDiameter = inputDouble("Введите внутренний диаметр трубы (мм): ");
    double wallThickness = inputDouble("Введите толщину стенки трубы (мм): ");

    double outerDiameter = innerDiameter + 2 * wallThickness;
    cout << "Внешний диаметр труб: " << outerDiameter << " мм" << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    while (true)
    {
        int choice = 0;

        cout << "\nВыберите расчет:" << endl;
        cout << "1 - Вес труб (по внешнему диаметру и толщине стенки)" << endl;
        cout << "2 - Вес труб (по внешнему и внутреннему диаметру)" << endl;
        cout << "3 - Вес труб (по внутреннему диаметру и толщине стенки)" << endl;
        cout << "4 - Длина труб (по внешнему диаметру и толщине стенки)" << endl;
        cout << "5 - Длина труб (по внешнему и внутреннему диаметру)" << endl;
        cout << "6 - Длина труб (по внутреннему диаметру и толщине стенки)" << endl;
        cout << "7 - Толщина стенки (по внешнему и внутреннему диаметру)" << endl;
        cout << "8 - Внешний диаметр (по внутреннему диаметру и толщине стенки)" << endl;
        cout << "0 - Выход из программы" << endl;
        cout << "Введите номер выбора: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0)
        {
            cout << "Выход из программы." << endl;
            break;
        }

        switch (choice)
        {
        case 1:
            calculatePipeWeightWithThickness();
            break;
        case 2:
            calculatePipeWeightByDiameters();
            break;
        case 3:
            calculatePipeWeightByInnerDiameterAndThickness();
            break;
        case 4:
            calculateLengthByOuterDiameterAndThickness();
            break;
        case 5:
            calculateLengthByOuterAndInnerDiameter();
            break;
        case 6:
            calculateLengthByInnerDiameterAndThickness();
            break;
        case 7:
            calculateWallThickness();
            break;
        case 8:
            calculateOuterDiameter();
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
            break;
        }
    }

    return 0;
}