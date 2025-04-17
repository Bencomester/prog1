#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

int main()
{
	unordered_map<string, int> map;
	int day = 1;
	ifstream file("list.txt");
	string line;
	while (getline(file, line)) {
		if (line == "") day++;
		else {
			map[line]++;
		}
	}
	file.close();

	ofstream output("occurrence.txt");
	for (auto item : map) output << item.first << " " << item.second * 100 / day  << "%" << endl;
	output.close();
}
