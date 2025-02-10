//Lecture 6 Operator Overloading, Generic Functions, and Propagating Values to Objects [2025-2-10 Mon]
#include <iostream>

using namespace std;

template <typename T>
T myMax(T x, T y) {
    return x > y ? x : y;
}

int main(void) {
    int a = 1, b = 2;
    double d = 1.2, e = 3.4;

    cout << myMax(a, b) << endl;
    cout << myMax(d, e) << endl;

    return 0;
}