#include <iostream>
#include <cmath>

#define M_PI 3.14159265358979323846

using namespace std;

void calculateWallThickness() {
    double outerDiameter = 0.0, innerDiameter = 0.0;

    cout << "������� ������� ������� ����� (��): ";
    cin >> outerDiameter;

    cout << "������� ���������� ������� ����� (��): ";
    cin >> innerDiameter;

    double wallThickness = (outerDiameter - innerDiameter) / 2;
    cout << "������� ������ �����: " << wallThickness << " ��" << endl;
}

void calculatePipeWeight() {
    double outerDiameter = 0.0, innerDiameter = 0.0, density = 0.0;

    cout << "������� ������� ������� ����� (��): ";
    cin >> outerDiameter;

    cout << "������� ���������� ������� ����� (��): ";
    cin >> innerDiameter;

    cout << "������� ��������� ��������� ����� (��/�?): ";
    cin >> density;

    double outerRadius = outerDiameter / 2 / 1000; // ������� � �����
    double innerRadius = innerDiameter / 2 / 1000; // ������� � �����
    double length = 1; // ����� ����� � ������ (�������� ����)

    double outerVolume = M_PI * pow(outerRadius, 2) * length;
    double innerVolume = M_PI * pow(innerRadius, 2) * length;
    double wallVolume = outerVolume - innerVolume;

    double weight = wallVolume * density;
    cout << "��� ��������� ����� �����: " << weight << " ��" << endl;
}

int main() {
    int choice = 0;

    cout << "�������� ������:" << endl;
    cout << "1 - ����� ������� ������" << endl;
    cout << "2 - ����� ��� ��������� ����� �����" << endl;
    cout << "������� ����� ������: ";
    cin >> choice;

    switch (choice) {
    case 1:
        calculateWallThickness();
        break;
    case 2:
        calculatePipeWeight();
        break;
    default:
        cout << "�������� �����." << endl;
        break;
    }

    return 0;
}