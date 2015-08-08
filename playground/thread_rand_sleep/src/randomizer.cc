#include <pthread.h>
#include <dlfcn.h>
#include <signal.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/optional.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/scoped_ptr.hpp>

#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>

typedef int (*PthreadCreate)(pthread_t*, const pthread_attr_t*, void* (*)(void*), void*);
PthreadCreate pthread_create_real = reinterpret_cast<PthreadCreate>(dlsym(RTLD_NEXT, "pthread_create"));;

__thread bool sleeping = false;

#define LOG(msg) \
    do { \
        std::ostringstream oss; oss << msg; printf("%s\n", oss.str().c_str()); \
    } while (false)

class Randomizer
{
public:
    static Randomizer& instance();

    void start();
    void stop();

    void registerThread(pthread_t const & threadId);

private:
    Randomizer();
    Randomizer(Randomizer const &);
    Randomizer& operator=(Randomizer const &);

    static void* startThreadTrampoline(void*);
    void startThread();

    static boost::scoped_ptr<Randomizer> inst;

    typedef std::vector<pthread_t> Threads;
    Threads threads;
    boost::mutex threadsMutex;

    boost::random::mt19937 randomGenerator;

    pthread_t workerThread;
    bool workerRunning; // should be atomic on x86 though in production it should be atomic explicitly
};

boost::scoped_ptr<Randomizer> Randomizer::inst;

Randomizer& Randomizer::instance()
{
    if (!Randomizer::inst)
    {
        Randomizer::inst.reset(new Randomizer);
    }
    return *Randomizer::inst;
}

Randomizer::Randomizer() : workerRunning(false)
{
}

void Randomizer::start()
{
    assert(pthread_create_real);
    assert(!this->workerRunning);

    this->workerRunning = true;
    if (pthread_create_real(&this->workerThread, NULL, &Randomizer::startThreadTrampoline, this) != 0)
    {
        LOG("Could not create randomizer worker thread");
        ::abort();
    }
}

void* Randomizer::startThreadTrampoline(void* data)
{
    assert(data);

    static_cast<Randomizer*>(data)->startThread();

    return NULL;
}

void Randomizer::startThread()
{
    while (this->workerRunning)
    {
        Threads ids;
        {
            boost::lock_guard<boost::mutex> lock(this->threadsMutex);
            ids = this->threads;
        }
        if (!ids.empty())
        {
            boost::random::uniform_int_distribution<> dist(0, ids.size() - 1);
            int const index = dist(this->randomGenerator);
            LOG("Sending signal to: " << std::hex << ids[index] << ", index: " << index);
            pthread_kill(ids[index], SIGUSR1);
        }
        sleep(1);
    }
}

void Randomizer::stop()
{
    assert(this->workerRunning);
    this->workerRunning = false;
    if (pthread_join(this->workerThread, NULL) != 0)
    {
        LOG("Failed while joining worker thread");
    }
}

void Randomizer::registerThread(pthread_t const & threadId)
{
    boost::lock_guard<boost::mutex> lock(this->threadsMutex);
    this->threads.push_back(threadId);
}

int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start)(void*), void* data)
{
    assert(pthread_create_real);

    int const result = pthread_create_real(thread, attr, start, data);
    if (result == 0)
    {
        LOG("Registering thread: " << std::hex << *thread);
        Randomizer::instance().registerThread(*thread);
    }
    return result;
}

void suspend_handler(int)
{
    if (sleeping) return;
    sleeping = true;
    LOG(std::hex << pthread_self() << " is sleeping for a sec");
    sleep(1);
    sleeping = false;
}

void install_signal_handlers()
{
    struct sigaction sa;

    sigfillset(&sa.sa_mask);
    sa.sa_flags = 0;
//    sa.sa_handler = resume_handler;
//    sigaction(RESUME_SIG, &sa, NULL);

    sa.sa_handler = suspend_handler;
    sigaction(SIGUSR1, &sa, NULL);
}

__attribute__((constructor))
static void ctor()
{
    install_signal_handlers();
}

void start()
{
    LOG("Loading randomizer library");
    Randomizer::instance().start();
}

void stop()
{
    LOG("Unloading randomizer library");
    Randomizer::instance().stop();
}
