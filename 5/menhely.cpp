#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct dog {
    string type;
	string name;
    int age = 0;
	string color;
};

vector<dog> db;

int read(string fileName) {
	ifstream file(fileName);
	if (!file.is_open()) {
		cerr << "Error opening file" << endl;
		return 1;
	}

	string line;
	while (getline(file, line)) {
		dog d;
		int n = 1;
		string age = "";
		for (char i : line) {
			if (i == ',') n++;
			else {
				switch (n) {
				case 1: d.type += i; break;
				case 2: d.name += i; break;
				case 3: age += i; break;
				case 4: d.color += i; break;
				}
			}
		}
		d.age = stoi(age);
		db.push_back(d);
	}
	file.close();
	return 0;
}

vector<dog> search(string name) {
	vector<dog> dogs;
	for (dog d : db) if (d.name == name) dogs.push_back(d);
	return dogs;
}

int main()
{
	if (read("menhely.txt")) return 1;

	while (true) {
		cout << "Name to search for: ";
		string name;
		cin >> name;
		vector<dog> dogs = search(name);
		cout << endl;
		if (dogs.empty()) cout << "Not found" << endl;
		else {
			for (dog d : dogs) {
				cout << "Name: " << d.name << endl;
				cout << "Type: " << d.type << endl;
				cout << "Age: " << d.age << endl;
				cout << "Color: " << d.color << endl << endl;
			}
		}
		cout << endl << endl;
	}
}
