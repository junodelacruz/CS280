// Lecture 7 STL Containers [2025-2-13 Thu]
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <stack>
#include <queue>
#include <map>
#include <string>

using namespace std;

int main(void)
{
    // lists
    //  vector<int> v = {12, 23, 34, 45, 56, 67, 78};
    vector<int> v(10);

    // to print the size and capacity
    cout << v.size() << ' ' << v.capacity() << endl;
    v.push_back(1); // pushes element to the back of the vector

    cout << v.size() << ' ' << v.capacity() << endl; // after adding another element, the memory size doubles and the size of the vector has 11 elements

    for (int i : v)
    {
        cout << i << ' ';
    }
    cout << endl;
    // another way to iterate
    //  vector<int>::iterator it = v.begin(); //normal iterate
    vector<int>::reverse_iterator it = v.rbegin(); // reverse iterate
    while (it != v.rend())
    {
        cout << *it << endl;
        it++;
    }

    // lists
    list<int> l = {12, 23, 34, 45, 56, 67, 78};
    list<int> t(l); // copy of l, but l doesnt change

    t.push_back(89);

    for (int i : l)
    {
        cout << i << ' ';
    }
    cout << endl;

    for (int i : t)
    {
        cout << i << ' ';
    }
    cout << endl;

    // deque
    deque<int> d = {12, 23, 34, 45, 56, 67, 78};
    d.push_back(89);
    d.push_front(1);
    for (int i : d)
    {
        cout << i << ' ';
    }
    cout << endl;

    // stacks
    //  vector<int> v = {12, 23, 34, 45, 56, 67, 78};
    //  stack<int> s; // cant initialize
    stack<int, vector<int>> s(v); // create stacks from scratch or by copying a vector, list, or deque
    s.push(12);                   // can use for loop to add to stack
    s.pop();

    // cant iterate over using for loop
    //  for (int i : s)
    //  {
    //      cout << i << ' ';
    //  }
    //  cout << endl;

    // queues
    queue<int> q;
    for (int i = 0; i < 10; i++)
    {
        q.push(i);
    }
    while (!q.empty())
    {
        cout << q.front() << endl;
        q.pop();
    }

    // maps (similar to hashmaps or dictionaries)
    map<int, string> m; // takes two parameters
    // m[1] == "one";
    // m[2] == "two";

    // cannot use this for maps
    // for(int i : m) {
    //     cout << m[i] << endl;
    // }

    // iterator for maps
    for (auto it = m.begin(); it != m.end(); it++)
    {
        cout << it->first << ':' << it->second << endl;
    }
    // has to be called first and second for key and value pair

    return 0;
}