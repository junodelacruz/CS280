// Lecture 13 Midterm Review Examples [2025-3-6 Thu]

// follow the slides with these notes:

// imperative is the first programming language paradigm, giving the computer machine language (von neumann)
// !file means error opening file
// pointers in cpp store the memory address of values. arrow operator: pointer -> member (pointer points to an object and member is a member of the object)
// c is the right answer because of conflicting types of parameters for T max (float, double). d is wrong because it forces everything to be a double, so that there will be no syntax error T max(double, double)
// x is an int, so it is not a member function of class One. answer is B
// A -> produces A followed by a b or just a b, and B -> produces a followed by a B or just an a. plug in A and B in S. S is at least one B followed by at least two A's followed by a B.
// # has more precedence because we read from left to right. where E is an expression and T is a term: <E> => <T> + <E> | <T> this is right to left associative. <E> => <E> + <T> | <T> this is left to right associative. just think of if the different term is on the right of left. answer for d is unambiguous.
// answer is c for EBNF. not d because factor and factor does not match term and factor. shows that a term can be a factor or a term followed by a multiplication, division, or module operator and a factor at the end. {} means zero or more.
// ********** regex do research on the patterns (study hard might be emphasis on regex on exam)
// for lexical analyzers: true, true
// what does a lexical analyzer generate? b. tokens. Most existing languages can be tokenized using: b. one character lookahead
// What are the lifetimes of z a and i in the above code: z is global, a is block scope and i is inner block scope. answer: d
// L-value is a value that can be assigned to. pointer contains the address at which we can assign a value of x
// heap segment is the only thing that makes sense, therefore making it the answer. "new" operator puts it on the heap
// in C++ we can reuse variable names
// answer is a because the if function can see those variables, but does not see the original a, only sees the a that is available to it.

#include <iostream>

using namespace std;

int main(void)
{

    return 0;
}