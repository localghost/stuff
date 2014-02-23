#include <iostream>
#include <utility>
#include <fstream>
#include <string>

class File
{
public:
    explicit File(std::string name) : name_(std::move(name)), is_open(false)
    {
    }

    std::string readLine()
    {
        if (!is_open)
        {
            handle_.open(name_);
            is_open = true;
        }
        std::string result;
        std::getline(handle_, result);
        return result;
    }

    ~File()
    {
        if (is_open)
            handle_.close();
    }

private:
    std::ifstream handle_;
    std::string name_;
    bool is_open;
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "usage: " << argv[0] << " [FILE_NAME]" << std::endl;
        return -1;
    }

    File f{argv[1]};
    std::cout << f.readLine() << std::endl;
}
