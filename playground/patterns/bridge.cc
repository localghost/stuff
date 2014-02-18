#include <iostream>
#include <memory>
#include <vector>

class UIElement
{
public:
    virtual ~UIElement() {}
    virtual void draw() =0;
};

class DrawingAPI
{
public:
    virtual ~DrawingAPI() {}
    virtual void draw(size_t width, size_t height) =0;
};

class Window : public UIElement
{
public:
    Window(size_t width, size_t height, DrawingAPI* drawing) 
        : width(width), height(height), drawing(drawing) {}

    void draw()
    {
        std::cout << "drawing window" << std::endl;
        drawing->draw(width, height);
    }
private:
    size_t width;
    size_t height;
    DrawingAPI* drawing;
};

class Button : public UIElement
{
public:
    Button(size_t width, size_t height, DrawingAPI* drawing) 
        : width(width), height(height), drawing(drawing) {}

    void draw()
    {
        std::cout << "drawind button" << std::endl;
        drawing->draw(width, height);
    }
private:
    size_t width;
    size_t height;
    DrawingAPI* drawing;
};

class ClassicDrawingAPI : public DrawingAPI
{
public:
    void draw(size_t width, size_t height)
    {
        std::cout << "drawing hard corner area" << std::endl;
    }
};

class RoundedDrawingAPI : public DrawingAPI
{
public:
    void draw(size_t width, size_t height)
    {
        std::cout << "drawing rounded corner area" << std::endl;
    }
};

int main()
{
    std::unique_ptr<DrawingAPI> classic{new ClassicDrawingAPI};
    std::unique_ptr<DrawingAPI> rounded{new RoundedDrawingAPI};

    std::vector<std::unique_ptr<UIElement>> elem;
    elem.emplace_back(new Window{10, 10, classic.get()});
    elem.emplace_back(new Window{11, 11, rounded.get()});
    elem.emplace_back(new Button{12, 12, classic.get()});
    elem.emplace_back(new Button{13, 13, rounded.get()});
    for (auto it = elem.begin(); it != elem.end(); ++it)
        (*it)->draw();
}
