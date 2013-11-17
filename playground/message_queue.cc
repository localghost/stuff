#include <mutex>
#include <queue>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <sstream>
#include <iostream>

template <typename T>
class message_queue
{
public:
    void push(const T& item)
    {
        std::lock_guard<std::mutex> lock{mtx};
        queue.push(item);
        cond.notify_one();
    }

    void pop(T& item)
    {
        std::unique_lock<std::mutex> lock{mtx};
        cond.wait(lock, [this]{ return !queue.empty(); });
        item = queue.front();
        queue.pop();
   }

private:
    std::mutex mtx;
    std::condition_variable cond;
    std::queue<T> queue;
};

std::string cat(const std::string& s, unsigned i)
{
    std::stringstream ss;
    ss << s << i;
    return ss.str();
}

void pusher(message_queue<std::string>& queue)
{
    for (unsigned i = 0; i < 100; ++i)
    {
        queue.push(cat("i: ", i));
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
}

void poper(message_queue<std::string>& queue)
{
    while (true)
    {
        std::string msg;
        queue.pop(msg);
        std::cout << msg << std::endl;
        if (msg == "i: 99")
            break;
    } 
}

int main()
{
    message_queue<std::string> queue;
    std::thread th_pusher(&pusher, std::ref(queue));
    std::thread th_poper(&poper, std::ref(queue));
    th_pusher.join();
    th_poper.join();
}
