#include <memory>
#include <iostream>
#include <utility>

struct Foo
{
    unsigned the_number;

    Foo(const Foo& other)
    {
        the_number = other.the_number;
    }

    Foo(Foo&& other)
    {
        the_number = other.the_number;
        other.the_number = 0;
    }

    explicit Foo(unsigned n) : the_number(n) {}
};

int main()
{
    Foo f{42};

    std::cout << "copy" << std::endl;
    auto ptr_cp = std::make_shared<Foo>(f);
    std::cout << "\tptr_cp->the_number = " << ptr_cp->the_number << std::endl;
    std::cout << "\tf.the_number = " << f.the_number << std::endl;
    
    std::cout << "move" << std::endl;
    auto ptr_mv = std::make_shared<Foo>(std::move(f));
    std::cout << "\tptr_mv->the_number = " << ptr_mv->the_number << std::endl;
    std::cout << "\tf.the_number = " << f.the_number << std::endl;
}
