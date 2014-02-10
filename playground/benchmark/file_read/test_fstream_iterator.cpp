#include <fstream>
#include <iterator>
#include <chrono>
#include <iostream>
#include <cstddef>

namespace {
const char* const TEST_FILE_PATH = "/tmp/file_read_test";
}

int main(int argc, char* argv[])
{
    std::ifstream ifs{TEST_FILE_PATH};
    if (!ifs)
    {
        std::cerr << "Could not open file '" << TEST_FILE_PATH
                  << "' for reading" << std::endl;
        return -1;
    }
    ifs >> std::noskipws;
    
    ifs.seekg(0, ifs.end);
    std::size_t size = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    std::string content;
    content.reserve(size+1);

//    auto before = std::chrono::steady_clock::now();
    content.insert(content.end(),
                   std::istream_iterator<char>{ifs},
                   std::istream_iterator<char>{});
//    auto after = std::chrono::steady_clock::now();
    ifs.close();
    std::cout << content;
}
