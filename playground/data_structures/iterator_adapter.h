#ifndef ITERATOR_ADAPTER_
#define ITERATOR_ADAPTER_

#include <iterator>

// I is at most BidirectionalIterator
//
// Adapted iterator should provide at least following methods:
// deref() : should return reference to the iterator's value
// increment() : should move the iterator one step forward
// decrement() : should move the iterator one step backwards
// state() : should return iterator's internal state by which two
//           iterators can be compared
template<typename I>
class iterator_adapter
{
public:
  typedef typename std::iterator_traits<I>::value_type value_type;
  typedef typename std::iterator_traits<I>::reference reference;
  typedef typename std::iterator_traits<I>::pointer pointer;
  typedef typename std::iterator_traits<I>::iterator_category iterator_category;

  iterator_adapter() = default;
  explicit iterator_adapter(I adapted) : adaptee_{std::move(adapted)} {}

  reference operator*() const
  {
    return adaptee_.deref();
  }

  pointer operator->() const
  {
    return &adaptee_.deref();
  }

  iterator_adapter& operator++()
  {
    adaptee_.increment();
    return *this;
  }

  iterator_adapter operator++(int)
  {
    auto result = *this;
    ++*this;
    return result;
  }

  iterator_adapter& operator--()
  {
    adaptee_.decrement();
    return *this;
  }

  iterator_adapter operator--(int)
  {
    auto result = *this;
    --*this;
    return result;
  }

  friend bool operator==(const iterator_adapter& x, const iterator_adapter& y)
  {
    return x.adaptee_.state() == y.adaptee_.state();
  }

  friend bool operator!=(const iterator_adapter& x, const iterator_adapter& y)
  {
    return !(x == y);
  }

private:
  I adaptee_;
};

#endif
