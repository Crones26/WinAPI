#include <iostream>
#include <cmath>

#define M_PI 3.14159265358979323846

using namespace std;

void calculateWallThickness() {
    double outerDiameter = 0.0, innerDiameter = 0.0;

    cout << "Введите внешний диаметр трубы (мм): ";
    cin >> outerDiameter;

    cout << "Введите внутренний диаметр трубы (мм): ";
    cin >> innerDiameter;

    double wallThickness = (outerDiameter - innerDiameter) / 2;
    cout << "Толщина стенки трубы: " << wallThickness << " мм" << endl;
}

void calculatePipeWeight() {
    double outerDiameter = 0.0, innerDiameter = 0.0, density = 0.0;

    cout << "Введите внешний диаметр трубы (мм): ";
    cin >> outerDiameter;

    cout << "Введите внутренний диаметр трубы (мм): ";
    cin >> innerDiameter;

    cout << "Введите плотность материала трубы (кг/м?): ";
    cin >> density;

    double outerRadius = outerDiameter / 2 / 1000; // перевод в метры
    double innerRadius = innerDiameter / 2 / 1000; // перевод в метры
    double length = 1; // длина трубы в метрах (погонный метр)

    double outerVolume = M_PI * pow(outerRadius, 2) * length;
    double innerVolume = M_PI * pow(innerRadius, 2) * length;
    double wallVolume = outerVolume - innerVolume;

    double weight = wallVolume * density;
    cout << "Вес погонного метра трубы: " << weight << " кг" << endl;
}

int main() {
    int choice = 0;

    cout << "Выберите расчет:" << endl;
    cout << "1 - Найти толщину стенки" << endl;
    cout << "2 - Найти вес погонного метра трубы" << endl;
    cout << "Введите номер выбора: ";
    cin >> choice;

    switch (choice) {
    case 1:
        calculateWallThickness();
        break;
    case 2:
        calculatePipeWeight();
        break;
    default:
        cout << "Неверный выбор." << endl;
        break;
    }

    return 0;
}