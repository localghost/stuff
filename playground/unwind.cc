#include <iostream>
#include <exception>

struct Foo { ~Foo() { throw std::exception(); }};

struct ex : std::exception
{
    ex() { std::cout << "ex()" << std::endl; }
    ex(const ex&) { std::cout << "copy()" << std::endl; }
    ex(ex&&) { std::cout << "move()" << std::endl; }
    ~ex() { std::cout << "~ex()" << std::endl; }
};

void handler()
{ 
    try 
    { 
        std::cout << "handler()" << std::endl; 
        throw;
    }
    catch (const ex& e)
    {
        std::cout << "ex" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    abort(); 
}


int main()
{
    std::set_terminate(&handler);
    Foo f;
    throw ex{};
}
