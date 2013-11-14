#include <iostream>
#include <string>
#include <utility>

struct Foo
{
    int i;
    std::string s;
};

void print(const Foo& f, const std::string& message = std::string()) {
    std::cout << message << std::endl;
    std::cout << "i: " << f.i << ", s: " << f.s << std::endl;
}

struct Bar
{
    explicit Bar(Foo f) : foo(std::move(f)) {
        print(f, "f: Bar()");
    }
    Foo foo;
};

int main()
{
    Foo f{42, "42"};
    Bar b(f);
    print(f, "f: main()");
    print(b.foo, "b: main()");
}
