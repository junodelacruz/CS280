//Lecture 6 Operator Overloading, Generic Functions, and Propagating Values to Objects [2025-2-10 Mon]
#include <iostream>
#include <fstream>

using namespace std;

// class node {
//     node(void) {}
//     friend struct stack;
// };

struct stack { //outer class
    stack(void){
        head = new node;
    }
private: //inner class
    class node {

    };

    node *head;
};

int main(void)  {
    stack s;
    return 0;
}