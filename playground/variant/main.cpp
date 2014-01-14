/**
 * @file   main.cpp
 * @author Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */
#include <iostream>
#include <string>
#include <map>
#include "Variant.h"
//using namespace std;

class MyVisitor : public Visitor<char, int, double, std::string, std::map<std::string, std::string> >
{
public:
    void visit(char&) { std::cout << "char" << std::endl; }
    void visit(int&) { std::cout << "int" << std::endl; }
    void visit(double&) { std::cout << "double" << std::endl; }
    void visit(std::string&) { std::cout << "std::string" << std::endl; }
    void visit(std::map<std::string, std::string>&) { std::cout << "std::map" << std::endl; }
};

int main()
{
    Variant<int> v1;
    Variant<char, int, double, std::string, std::map<std::string, std::string>> v2(std::string("hello world!"));
    std::cout << "min_size = " << min_size<int, double>::value << std::endl;
    std::cout << "min_size = " << sizeof(min_type<int, double>::type) << std::endl;
    std::cout << "type_index = " << index_of<float, double, char, int, float>::value << std::endl;
    try
    {
        std::cout << v2.get<char>() << std::endl;
    }
    catch (std::bad_cast)
    {
        std::cerr << "bad cast" << std::endl;
    }
    std::cout << v2.get<std::string>() << std::endl;
    auto visitor = MyVisitor();
    v2.apply(visitor);
    v2.set(5);
    v2.apply(visitor);
    v2 = 5.3;
    v2.apply(visitor);
    int i = 4;
    v1 = i;
    std::cout << "v1 = " << v1.get<int>() << std::endl;
    std::cout << typeid(type_at<0, int, float>::type).name() << std::endl;
    return 0;
}
