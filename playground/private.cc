#include <iostream>

class Foo
{
  void foo() { std::cout << "Foo" << std::endl; member = 42;}
  int member;
};

using FooFun = void (Foo::*)();
using FooMember = int Foo::*;

template<FooFun fun_, FooMember member_>
struct Baz
{
  friend FooFun fun(FooFun) { return fun_; }
  friend FooMember member(FooMember) { return member_; }
};

template class Baz<&Foo::foo, &Foo::member>;
FooFun fun(FooFun);
FooMember member(FooMember);

int main()
{
  Foo f;
  (f.*fun(FooFun{}))();
  std::cout << f.*member(FooMember{}) << std::endl;
}
