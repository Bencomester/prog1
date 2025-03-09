#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct record {
    string rendsz;
    string marka;
    string tipus;
    short int ev;
};

vector<record> getRecords();
string findCar(string l, vector<record>& data);

int main()
{
    setlocale(LC_ALL, "");
    vector<record> data = getRecords();
    string license;
    cout << "Rendszám: ";
    cin >> license;
    cout << endl;
    string car = findCar(license, data);
    cout << car << endl;
}

string findCar(string l, vector<record>& data) {
    for (record car : data) {
        if (car.rendsz == l) return ("Rendszám: " + l) + ("\tMárka: " + car.marka) + ("\tTípus: " + car.tipus) + ("\tÉvjárat: " + to_string(car.ev));
    }
    return "Nincs ilyen autó az adatbázisban";
}

vector<record> getRecords() {
    return {
        {"ABC-123", "Toyota", "Corolla", 2015},
        {"DEF-456", "Honda", "Civic", 2018},
        {"GHI-789", "Ford", "Focus", 2016},
        {"JKL-012", "BMW", "320i", 2020},
        {"MNO-345", "Audi", "A4", 2019},
        {"PQR-678", "Mercedes", "C200", 2017},
        {"STU-901", "Volkswagen", "Golf", 2021},
        {"VWX-234", "Hyundai", "Elantra", 2014},
        {"YZA-567", "Nissan", "Altima", 2013},
        {"BCD-890", "Kia", "Sorento", 2022},
        {"EFG-123", "Mazda", "CX-5", 2017},
        {"HIJ-456", "Chevrolet", "Malibu", 2015},
        {"KLM-789", "Subaru", "Impreza", 2016},
        {"NOP-012", "Tesla", "Model 3", 2023},
        {"QRS-345", "Volvo", "S60", 2020},
        {"TUV-678", "Peugeot", "308", 2018},
        {"WXY-901", "Renault", "Megane", 2019},
        {"ZAB-234", "Fiat", "Punto", 2014},
        {"CDE-567", "Opel", "Astra", 2013},
        {"FGH-890", "Seat", "Leon", 2021},
        {"IJK-123", "Skoda", "Octavia", 2017},
        {"LMN-456", "Mitsubishi", "Lancer", 2016},
        {"OPQ-789", "Lexus", "IS250", 2015},
        {"RST-012", "Jeep", "Wrangler", 2020},
        {"UVW-345", "Land Rover", "Defender", 2021},
        {"XYZ-678", "Suzuki", "Swift", 2011},
        {"BCD-901", "Chrysler", "300", 2013},
        {"EFG-234", "Cadillac", "CTS", 2014},
        {"HIJ-567", "Alfa Romeo", "Giulia", 2018},
        {"KLM-890", "Jaguar", "XE", 2019}
    };
}