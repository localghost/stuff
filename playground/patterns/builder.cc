#include <iostream>
#include <memory>
#include <cstddef>
#include <string>
#include <utility>

enum class Color { RED, GREEN, BLUE };

//class WindowBuilder;

class Window
{
public:
    void draw()
    {
        std::cout << "Window: \n"
                  << "\tforeground: " << static_cast<int>(foreground) << "\n"
                  << "\tbackground: " << static_cast<int>(background) << "\n"
                  << "\ttitle: " << title << "\n"
                  << "\twidth: " << width << "\n"
                  << "\theight: " << height << std::endl;
    }

private:
    Window() =default;

    Color foreground;
    Color background;
    std::string title;
    std::size_t width;
    std::size_t height;

    friend class WindowBuilder;
};

class WindowBuilder
{
public:
    void setForegroundColor(Color color) { foreground = color; }

    void setBackgroundColor(Color color) { background = color; }

    void setTitle(std::string title) { this->title = std::move(title); }

    void setWidth(std::size_t width) { this->width = width; }

    void setHeight(std::size_t height) { this->height = height; }

    std::unique_ptr<Window> build()
    {
        std::unique_ptr<Window> result{new Window};
        result->foreground = foreground;
        result->background = background;
        result->title = title;
        result->width = width;
        result->height = height;
        return result;
    }

private:
    Color foreground;
    Color background;
    std::string title;
    std::size_t width;
    std::size_t height;
};

int main()
{
    WindowBuilder builder;
    builder.setForegroundColor(Color::RED);
    builder.setBackgroundColor(Color::GREEN);
    builder.setHeight(100);
    builder.setWidth(200);
    builder.setTitle("Window1");

    auto w1 = builder.build();

    builder.setTitle("Window 2");
    builder.setHeight(200);

    auto w2 = builder.build();

    w1->draw();
    w2->draw();
}
