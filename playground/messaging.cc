// core
#include <utility>
#include <tuple>
#include <cassert>

// test - main()
#include <iostream>

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
  void handle(MessageBase* message) 
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

template<>
class Message<void> : public MessageBase
{
public:
  void process(HandlerBase* handler)
  {
    auto casted = dynamic_cast<Handler<Message<void>>*>(handler);
    assert(casted);
    casted->handle(this);
  }
};

struct Foo : Handler<Message<int>>,
             Handler<Message<float>>,
             Handler<Message<int, float>>,
             Handler<Message<void>>
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

  void handle(Message<void>* m)
  {
    std::cout << "Message<void>" << std::endl;
  }
};

int main()
{
  Message<int> im{42};
  Message<float> fm{1.23};
  Message<int, float> ifm{42, 1.23};
  Message<void> vm;

  Foo foo_handler;
  HandlerBase* handler = &foo_handler;
  handler->handle(&im);
  handler->handle(&fm);
  handler->handle(&ifm);
  handler->handle(&vm);
}
