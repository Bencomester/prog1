#include <iostream>

using namespace std;

int randint(int min, int max) { srand(time(0)); return rand() % (max - min) + min; }

int width = randint(5, 100);
int height = randint(5, 30);
int ballx = randint(1, width - 1);
int bally = randint(1, height - 1);
int xdir = 1;
int ydir = 1;

void drawBoard() {
	cout << "Width: " << width << " Height: " << height << endl;
	cout << " ";
	for (int i = 0; i < width + 2; i++) cout << "-";
	cout << endl;
	for (int i = 0; i < height + 2; i++) {
		cout << "|";
		for (int j = 0; j < width + 2; j++) cout << (i == bally && j == ballx ? "O" : " ");
		cout << "|";
		cout << endl;
	}
	cout << " ";
	for (int i = 0; i < width + 2; i++) cout << "-";
	cout << endl;
}



int main()
{
	drawBoard();
	while (true) {
		system("cls");
		if (ballx == 0 || ballx == width) xdir = -xdir;
		if (bally == 0 || bally == height) ydir = -ydir;
		ballx += xdir;
		bally += ydir;
		drawBoard();
		_Thrd_sleep_for(100);
	}

}


