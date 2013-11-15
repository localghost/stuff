#include <iostream>
#include <string>
#include <utility>
using namespace std;

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

struct Baz
{
    Baz() { cout << "ctor" << endl; }
    Baz(const Baz&) { cout << "copy" << endl; }
    Baz(Baz&&) { cout << "move" << endl; }
};

void baz(Baz b)
{
    cout << "baz" << endl;
}

int main()
{
    Foo f{42, "42"};
    Bar b(f);
    print(f, "f: main()");
    print(b.foo, "b: main()");

    cout << "<<<<<<>>>>>>" << endl;
    Baz b;
    cout << "-----" << endl;
    baz(move(b));
    cout << "-----" << endl;
    baz(Baz());
}
