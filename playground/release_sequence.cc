#include <atomic>
#include <thread>
#include <iostream>
#include <chrono>
#include <sstream>
#include <vector>

std::atomic<int> counter;
std::mutex m;
const unsigned counter_max = 20;

void setup()
{
    //counter.store(counter_max, std::memory_order_release);
    counter.store(counter_max, std::memory_order_relaxed);
}

void consume()
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    int value;
    //while ((value = counter.fetch_sub(1, std::memory_order_acquire)) <= 0)
    while ((value = counter.fetch_sub(1, std::memory_order_relaxed)) <= 0)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        continue;
    }

    std::stringstream ss;
    ss << std::this_thread::get_id() << " " << value << "\n";

    std::lock_guard<std::mutex> lock(m);
    std::cout << ss.str();
}

int main()
{
    std::thread init(setup);
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < counter_max; ++i)
        threads.push_back(std::thread{consume});
    init.join();
    for (auto& th : threads)
        th.join();
}
