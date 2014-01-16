// core
#include <utility>
#include <tuple>
#include <cassert>

// test - main()
#include <iostream>
#include <memory>

class HandlerBase;

class MessageBase
{
public:
    virtual ~MessageBase() {}
    virtual void process(HandlerBase*) =0;
};

class HandlerBase
{
public:
    virtual ~HandlerBase() {}
    virtual void handle(MessageBase* message)
    {
        message->process(this);
    }
};

template<typename MessageType>
class Handler : public virtual HandlerBase
{
public:
    virtual ~Handler() {}
    virtual void handle(MessageType* message) =0;
};

template<typename ...Args>
class Message : public MessageBase
{
private:
    typedef std::tuple<Args...> ValueType;

public:
    explicit Message(Args... args) : t_(std::move(args)...) {}

    void process(HandlerBase* handler)
    {
        auto casted = dynamic_cast<Handler<Message<Args...>>*>(handler);
        assert(casted);
        casted->handle(this);
    }

    template<std::size_t I>
    typename std::tuple_element<I, ValueType>::type get() const
    {
        return std::get<I>(t_);
    }

private:
    ValueType t_;
};

struct Foo : Handler<Message<int>>,
             Handler<Message<float>>,
             Handler<Message<int, float>>
{
    void handle(Message<int>* m)
    {
        std::cout << "Message<int>: " << m->get<0>() << std::endl;
    }

    void handle(Message<float>* m)
    {
        std::cout << "Message<float>: " << m->get<0>() << std::endl;
    }

    void handle(Message<int, float>* m)
    {
        std::cout << "Message<int, float>: " << m->get<0>() << ", " << m->get<1>() << std::endl;
    }
};

int main()
{
    std::unique_ptr<Message<int>> mi{new Message<int>{42}};
    std::unique_ptr<Message<float>> mf{new Message<float>{1.23}};
    std::unique_ptr<Message<int, float>> mif{new Message<int, float>{42, 1.23}};
    std::unique_ptr<HandlerBase> handler{new Foo};
    handler->handle(mi.get());
    handler->handle(mf.get());
    handler->handle(mif.get());
}
