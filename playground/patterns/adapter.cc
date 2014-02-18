#include <ctime>
#include <chrono>
#include <iostream>
#include <string>
#include <stdexcept>

class DatePicker
{
public:
    std::time_t getDate() const
    {
        return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }
};

class ObjectAdaptor
{
public:
    std::string getDate() const
    {
        auto seconds = picker.getDate();
        auto time = *localtime(&seconds);
        char result[32] = {};
        if (0 == std::strftime(result, 32, "%Y-%m-%d %H:%M", &time))
            throw std::runtime_error{"time exceeded the buffer"};
        return result;
    }

private:
    DatePicker picker;
};


class ClassAdaptor : private DatePicker
{
public:
    std::string getDate() const
    {
        auto seconds = DatePicker::getDate();
        auto time = *localtime(&seconds);
        char result[32] = {};
        if (0 == std::strftime(result, 32, "%Y-%m-%d %H:%M", &time))
            throw std::runtime_error{"time exceeded the buffer"};
        return result;
    }
};

int main()
{
    ObjectAdaptor obj_adaptor;
    std::cout << obj_adaptor.getDate() << std::endl;

    ClassAdaptor class_adaptor;
    std::cout << class_adaptor.getDate() << std::endl;
}
