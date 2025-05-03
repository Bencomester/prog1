#include <iostream>
#include <algorithm>

struct Pair {
	int a_;
	int b_;
};

void SortByValueA(Pair* data, int length) {
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length - 1; j++) {
			if (data[j].a_ > data[j + 1].a_) {
				std::swap(data[j], data[j + 1]);
			}
		}
	}
}

void PrintDoubleLine() {
	std::cout << "===" << std::endl;
}
void PrintSingleLine() {
	std::cout << "---" << std::endl;
}
void PrintOutput() {
	std::cout << "Output:" << std::endl;
}

void CalculateValuesAndPrint(const Pair* array, int size) {
	for (int i = 0; i < size; i++) {
		int result = 0;
		if (array[i].a_ % 2 == 0) {
			if (array[i].b_ % 2 == 0) {
				result = array[i].a_ * array[i].b_;
			} else {
				result = array[i].a_ + array[i].b_;
			}
		} else {
			if (array[i].b_ % 2 == 0) {
				result = array[i].a_ - array[i].b_;
			} else {
				result = array[i].a_;
			}
		}

		std::cout << result << std::endl;
	}
}

void ProcessData(Pair* data, int length) {
	SortByValueA(data, length);
	PrintDoubleLine();
	PrintOutput();
	PrintSingleLine();
	CalculateValuesAndPrint(data, length);
}

int main() {
	Pair array[5] = {
		{5, 1},
		{2, 4},
		{3, 7},
		{1, 6},
		{4, 5}
	};

	ProcessData(array, 5);
	return 0;
}
