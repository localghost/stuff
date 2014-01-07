#include <iostream>
#include <string>
#include <utility>
using namespace std;

struct Parent
{
    explicit Parent(std::string name) : name(move(name)) {}

    virtual void whoami() = 0;

    std::string name;
};

struct Child1 : Parent {
    Child1() : Parent("Child1") {}
    void whoami() { cout << "Child1::foo(): " << name << endl; }
};

struct Child2 : Parent {
    Child2() : Parent("Child2") {}
    void whoami() { cout << "Child2::foo(): " << name << endl; }
};

int main()
{
    Child1 c1;
    Child2 c2;

    Parent* p = &c1;

    p->whoami();

    // jam vptr
    *reinterpret_cast<void**>(&c1) = *reinterpret_cast<void**>(&c2);
    
    p->whoami();
}
