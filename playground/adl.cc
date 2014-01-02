#include <iostream>

namespace NS {
    class T { };
    void foo(T) { std::cout << "foo(T)" << std::endl; }
}

int main() {
    NS::T parm;
    foo(parm);
}
