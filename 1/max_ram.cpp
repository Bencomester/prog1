#include <iostream>
using namespace std;

int main()
{
    while (true) {
        try {
            char* p = new char[1024 * 1024];
        }
        catch (bad_alloc&) {
            cerr << "Failed";
        }
    }
}

