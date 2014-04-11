#ifndef OP_COUNTER_H
#define OP_COUNTER_H

#include <map>
#include <string>
#include <utility>
#include <iostream>

enum operation {
  construction,
  copy,
  move,
  destruction,
  assignment,
  move_assignment,
  equality,
  comparison
};

std::string operation_to_string(operation op)
{
  switch (op)
  {
    case construction: return "construction";
    case copy: return "copy";
    case move: return "move";
    case destruction: return "destruction";
    case assignment: return "assignment";
    case move_assignment: return "move_assignment";
    case equality: return "equality";
    case comparison: return "comparison";
  }
}

template<typename T, typename tag = T>
void print_results();

template<typename T, typename tag = T>
class op_counter
{
public:
  op_counter() : op_counter(0) { }

  explicit op_counter(T value) : value(std::move(value))
  {
    ++counts[construction];
  }

  op_counter(const op_counter& other) : value(other.value)
  {
    ++counts[copy];
  }

  op_counter(op_counter&& other) : value(std::move(other.value))
  {
    ++counts[move];
  }

  op_counter& operator=(const op_counter& other)
  {
    ++counts[assignment];
    value = other.value;
    return *this;
  }

  op_counter& operator=(op_counter&& other)
  {
    ++counts[move_assignment];
    value = std::move(other.value);
    return *this;
  }

  ~op_counter()
  {
    ++counts[destruction];
  }

  friend bool operator==(const op_counter& x, const op_counter& y)
  {
    ++counts[equality];
    return (x.value == y.value);
  }

  friend bool operator!=(const op_counter& x, const op_counter& y)
  {
    return !(x == y);
  }

  friend bool operator<(const op_counter& x, const op_counter& y)
  {
    ++counts[comparison];
    return (x.value < y.value);
  }

  friend bool operator>(const op_counter& x, const op_counter& y)
  {
    return (y < x);
  }

  friend bool operator<=(const op_counter& x, const op_counter& y)
  {
    return !(y < x);
  }

  friend bool operator>=(const op_counter& x, const op_counter& y)
  {
    return !(x < y);
  }

  friend std::ostream& operator<<(std::ostream& os, const op_counter& x)
  {
    os << x.value;
    return os;
  }

  friend void print_results<T, tag>();

private:
  static std::map<operation, unsigned> counts;

  T value;
};

template<typename T, typename tag = T>
void print_results()
{
  for (auto it = op_counter<T, tag>::counts.begin(); it != op_counter<T, tag>::counts.end(); ++it)
    std::cout << operation_to_string(it->first) << ": " << it->second << std::endl;
}

template<typename T, typename tag>
std::map<operation, unsigned> op_counter<T, tag>::counts;

#endif
