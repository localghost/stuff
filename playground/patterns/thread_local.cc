#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include <vector>

const unsigned int num_of_threads = 10;
std::mutex io_mutex;

//thread_local unsigned int answer = 42; // C++11
__thread unsigned int answer = 42;

void whats_the_answer()
{
    ++answer;
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    std::lock_guard<std::mutex> lock{io_mutex};
    std::cout << "answer is: " << answer << std::endl;
}

int main()
{
    std::vector<std::thread> threads;
    for (size_t i = 0; i < num_of_threads; ++i)
        threads.push_back(std::thread(&whats_the_answer));
    for (auto& th : threads)
        th.join();
}
