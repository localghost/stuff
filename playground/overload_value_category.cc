#include <iostream>

struct Foo
{
    void bar() &
    {
        std::cout << "void bar() &" << std::endl;
    }

    void bar() &&
    {
        std::cout << "void bar() &&" << std::endl;
    }
};

int main()
{
    Foo f;
    f.bar();

    Foo{}.bar();
}
