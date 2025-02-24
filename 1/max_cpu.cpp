#include <vector>
#include <thread>
using namespace std;

void nothing() {
	while (true) {}
}

int main()
{
	int threadCount = thread::hardware_concurrency();
	vector<thread> v;
	for (int i = 0; i < threadCount; i++) {
		v.emplace_back(nothing);
	}

	for (auto& t : v) {
		t.join();
	}
}

