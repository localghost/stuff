#include <ctime>
#include <chrono>
#include <iostream>
#include <string>
#include <exception>

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
            throw std::runtime_error("time exceeded the buffer");
        return result;
    }

private:
    DatePicker picker;
};

int main()
{
    ObjectAdaptor adaptor;
    std::cout << adaptor.getDate() << std::endl;
}
