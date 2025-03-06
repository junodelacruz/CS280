// Lecture 11 and 12 Variables, Bindings, and Scopes [2025-2-27 Thu]
#include <iostream>
#include <string>

using namespace std;

int i = 1;
void foo(void) {}

int main(int argc, char *argv[])
{
    int a = 1;
    // ^ a is an L-value and 1 is an R-value (left and right values)
    if (a = 1)
    {
        cout << "OOPS!" << endl;
    }
    // ----------------
    int b = 2;
    int *ip = new int(3);
    cout << &a << ' ' << &b << ' ' << ip << endl;
    // ----------------
    string s;                  // actual string object, in c++ string declaraions are the object itself, not a reference
    string *sp = new string(); // a reference to string object, similar to how strings in java are defined as references
    cout << s.size();
    // ----------------
    cout << &i << ' ' << foo << ' ' << main << endl;
    // ----------------
    // 3.2 % 1.6; cannot do this in c++ because modulus only works with integers
    // ----------------
    int c = 1;

    c = c + 1; // c on left is L value and a on right is R value
    // ----------------
    // const int *const ip; // read from right to left to determine what type the variable is,
    // pointers need to always be initialized
    // ----------------
    auto z = 1;
    cout << sizeof(z) << endl;
    // ----------------
    // we allocate when the function starts, at { and we deallocate the memory at the end of a function }
    // lifetime - execution time of variables

    return 0;
}