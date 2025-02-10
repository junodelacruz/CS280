//Lecture 6 Operator Overloading, Generic Functions, and Propagating Values to Objects [2025-2-10 Mon]
#include <iostream>
#include <fstream>

using namespace std;

struct complex {

    complex(double real=0.0, double imag=0.0) : real(real), imag(imag) {} //constructor (initizalize)
    //members
    complex operator*(const complex &rhs) const {
        return complex(real*rhs.real - imag*rhs.imag, real*rhs.imag + rhs.imag + imag);
    }//rhs : right hand side of the equation, not changing the operand on right hand side so declare as const, const at the end is to keep left hand
    // side operand also a constant

    complex operator*(double d) const { //overloading
        //return complex(real*d, imag*d);
        return *this * complex(d); // same thing as above but easier
    } 

    complex operator-(const complex &rhs) const { //overloading subtraction
        return complex(real-rhs.real, imag-rhs.imag);
    }
    complex operator-(double d) const { //overloading subtraction
        // return complex(real-d, imag);
        return *this - complex(d);
    }
    // complex operator~(void) { 
    //     return complex(-real, -imag);
    // }
    // complex operator/(const complex&rhs) const { //overloading division
    //     complex top = *this * ~rhs;
    //     complex bot = rhs * ~rhs;
    //     return complex(top.real / bot.real, top.imag / bot.real);
    // }

    friend ostream &operator<<(ostream &, const complex &);
    //friend is a keyword used to grant a function or another class access to the private and protected members of a class. 
    // It's a way to bypass the usual access restrictions for specific entities

    void print(void) {
        cout << '(' << real << " + " << imag << "i)" << endl;
    }

private: //private area (define)
    double real; 
    double imag;
};

//functions
complex operator*(double d, const complex &c) { 
    return complex(d) * c;
}
ostream &operator<<(ostream &out, const complex &c){
   return cout << '(' << c.real << " + " << c.imag << "i)";
}

int main(void)  {

    complex a(2, 3); //2 + 3i
    complex b(1, 2); //1 + 2i
    complex c = a - b;

    cout << c << endl;
    c.print();

    complex d = c.operator * (5);
    cout << d << endl;
    d.print();

    complex e = 5 * d;
    cout << e << endl;
    e.print();

    operator<<(cout, e) << endl;

    return 0;
}