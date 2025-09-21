#include <iostream>
#include <cmath>
#include <unistd.h>
#define RED "\033[31m"
#define WHITE "\033[37m"
using namespace std;

int main(int argc, char **argv) {
    float x, y;
    for (y = 1.3f; y >= -1.2f; y -= 0.08f) {
        for (x = -2.0f; x <= 2.0f; x += 0.05f) {
            // Fórmula mejorada del corazón: (x²+y²-1)³ - x²y³ ≤ 0
            float a = x * x + y * y - 1;
            if ((a * a * a - x * x * y * y * y) <= 0.0f)
                cout << RED << "@" << WHITE;
            else
                cout << " ";
			usleep(50);
        }
        cout << endl;
    }
    return 0;
}
