#include <iostream>
#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
using namespace std;

extern void start();
extern void stop();
boost::mutex ioMutex;

void foo()
{
    unsigned i = 15;

    while (i--)
    {
//        {
//            boost::lock_guard<boost::mutex> lock(ioMutex);
//            std::cout << "hello from " << boost::this_thread::get_id() << std::endl;
//        }
        sleep(1);
    }
}

void* bar(void *) {}

int main()
{
    cout << "main" << endl;

    pthread_t thread;
    pthread_create(&thread, NULL, &bar, NULL);

    typedef std::vector<boost::thread*> Threads;
    Threads threads;
    size_t const numThreads = 10;
    for (size_t i = 0; i < numThreads; ++i)
    {
        threads.push_back(new boost::thread(&foo));
    }

    start();

    sleep(10);

    stop();

    for (Threads::iterator it = threads.begin(); it != threads.end(); ++it)
    {
        (*it)->join();
        delete *it;
    }
}
