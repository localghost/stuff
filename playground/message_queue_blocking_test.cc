#include <thread>
#include <chrono>
#include <functional>
#include <sstream>
#include <iostream>
#include "message_queue_blocking.h"

typedef message_queue_blocking<std::string> message_queue;

//std::string operator""s(const char* s, size_t)
//{
//    return std::string(s);
//}

std::string operator+(const std::string& s, unsigned i)
{
    std::stringstream ss;
    ss << s << i;
    return ss.str();
}

void pusher(message_queue& queue)
{
    for (unsigned i = 0; i < 100; ++i)
    {
        queue.push(std::string("i: ") + i);
        std::this_thread::sleep_for(std::chrono::milliseconds{30});
    }
    queue.push("quit");
}

void poper(message_queue& queue)
{
    while (true)
    {
        std::string msg;
        queue.pop(msg);
        std::cout << msg << std::endl;
        if (msg == "quit")
            break;
    } 
}

int main()
{
    message_queue queue;
    std::thread th_pusher(&pusher, std::ref(queue));
    poper(queue);
    th_pusher.join();
}
