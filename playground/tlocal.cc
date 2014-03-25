#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

std::mutex iom;
std::atomic<unsigned> cter{0};

template<typename T>
void print(const T& msg)
{
    std::lock_guard<std::mutex> lock(iom);
    std::cout << "[" << std::this_thread::get_id() << "] " << msg << std::endl;
}

struct Foo
{
    Foo() { print("Foo ctor"); ++cter; }
    int i;
};

thread_local Foo f;

void thread_loop_no_f()
{
    print(">> thread_loop_no_f");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    print("thread_loop_no_f >>");
}

void thread_loop_with_f()
{
    print(">> thread_loop_with_f");
    f;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    print("thread_loop_with_f >>");
}

int main()
{
    print(">> main");
    f;
    std::thread t1(thread_loop_no_f);
    std::thread t2(thread_loop_with_f);
    t1.join();
    print("main >>");
    t2.join();
    print(cter);
}
