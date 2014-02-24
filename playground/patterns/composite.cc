#include <iostream>
#include <memory>
#include <vector>
#include <utility>

class component
{
public:
    virtual ~component() {}
    virtual void draw() =0;
};

class ellipse : public component
{
public:
    void draw()
    {
        std::cout << "drawing leaf: " << this << std::endl;
    }
};

class composite : public component
{
public:
    void draw()
    {
        std::cout << "drawing composite: " << this << std::endl;
        for (auto it = children.begin(); it != children.end(); ++it)
            (*it)->draw();
    }

    void add_child(std::unique_ptr<component> comp)
    {
        children.push_back(std::move(comp));
    }

private:
    std::vector<std::unique_ptr<component>> children;
};

template<typename T>
std::unique_ptr<T> make_unique(T* t)
{
    return std::unique_ptr<T>(t);
}

int main()
{
    auto root = make_unique(new composite);
    auto child1 = make_unique(new composite);
    root->add_child(make_unique(new ellipse));
    child1->add_child(make_unique(new ellipse));
    root->add_child(std::move(child1));
    root->draw();
}
