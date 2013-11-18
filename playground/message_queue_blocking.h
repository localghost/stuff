#ifndef MESSAGE_QUEUE_BLOCKING_H
#define MESSAGE_QUEUE_BLOCKING_H

#include <mutex>
#include <queue>
#include <condition_variable>

template <typename T>
class message_queue_blocking
{
public:
    void push(const T& item)
    {
        std::lock_guard<std::mutex> lock{mtx};
        queue.push(item);
        condition.notify_one();
    }

    void pop(T& item)
    {
        std::unique_lock<std::mutex> lock{mtx};
        condition.wait(lock, [this]{ return !queue.empty(); });
        item = queue.front();
        queue.pop();
   }

private:
    std::mutex mtx;
    std::condition_variable condition;
    std::queue<T> queue;
};

#endif
