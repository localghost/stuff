#include <fstream>
#include <iterator>
#include <chrono>
#include <iostream>
#include <cstddef>

namespace {
const char* const TEST_FILE_PATH = "/tmp/file_read_test";
}

std::chrono::microseconds test_fstream_iterator(const std::string& filename)
{
    std::ifstream ifs{filename};
    if (!ifs)
        throw std::exception{}; // FIXME add some relevant message

    ifs >> std::noskipws;
    
    ifs.seekg(0, ifs.end);
    std::size_t file_size = ifs.tellg();
    ifs.seekg(0, ifs.beg);

    std::string content;
    content.reserve(file_size + 1); // TODO is +1 necessary?

    auto start = std::chrono::steady_clock::now();
    content.insert(content.end(),
                   std::istream_iterator<char>{ifs},
                   std::istream_iterator<char>{});
    auto end = std::chrono::steady_clock::now();

    ifs.close();

    return (end - start);
}

int main(int argc, char* argv[])
{
    test_fstream_iterator(TEST_FILE_PATH);
}
