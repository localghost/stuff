#include <iostream>
#include <memory>

template<typename T>
std::unique_ptr<T> make_unique(T* t)
{
    return std::unique_ptr<T>(t);
}

enum class FactoryType
{
    D3,
    D2
};

struct Shape {};

struct D3Shape : Shape {};

struct D2Shape : Shape {};

class Factory
{
public:
    virtual ~Factory() {}
    virtual std::unique_ptr<Shape> createShape() =0;
};

class D3Factory : public Factory
{
    std::unique_ptr<Shape> createShape()
    {
        std::cout << "Create 3D Shape" << std::endl;
        return make_unique(new D3Shape);
    }
};

class D2Factory : public Factory
{
    std::unique_ptr<Shape> createShape()
    {
        std::cout << "Create 2D Shape" << std::endl;
        return make_unique(new D2Shape);
    }
};

std::unique_ptr<Factory> createFactory(FactoryType type)
{
    switch (type)
    {
        case FactoryType::D3: return make_unique(new D3Factory);
        case FactoryType::D2: default: return make_unique(new D2Factory);
    }
}

int main(int argc, char* argv[])
{
    FactoryType type = FactoryType::D2;
    if ((argc > 1) and (std::string{"3D"} == argv[1]))
        type = FactoryType::D3;
    auto factory = createFactory(type);
    factory->createShape();
}
