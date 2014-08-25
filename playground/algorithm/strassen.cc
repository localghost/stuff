#include <memory>
#include <utility>
#include <initializer_list>
#include <iostream>

template<typename T>
class matrix
{
public:
  matrix() = default;
  
  explicit matrix(size_t size) : size_{size}
  {
    allocate();
  }

  matrix(std::initializer_list<T> data, size_t size) : size_{size}
  {
    allocate();
    size_t i = 0;
    for (auto it = data.begin(); it != data.end(); ++it, ++i)
      data_[i / size_][i % size_] = *it;
  }

  matrix(const matrix&) = delete;

  matrix(matrix&& other)
  {
    size_ = other.size_;
    data_ = other.data_;

    other.size_ = 0;
    other.data_ = nullptr;
  }

  ~matrix()
  {
    deallocate();
  }

  matrix& operator=(const matrix&) = delete;

  matrix& operator=(matrix&& other)
  {
    deallocate();

    size_ = other.size_;
    data_ = other.data_;

    other.size_ = 0;
    other.data_ = nullptr;

    return *this;
  }

  T* operator[](size_t i)
  {
    return data_[i];
  }

  const T* operator[](size_t i) const
  {
    return data_[i];
  }

  size_t size() const { return size_; }

private:
  void allocate()
  {
    data_ = new T*[size_];
    for (size_t i = 0; i != size_; ++i)
      data_[i] = new T[size_];
  }

  void deallocate()
  {
    for (size_t i = 0; i != size_; ++i)
      delete [] data_[i];
    delete [] data_;
  }

  T** data_ = nullptr;
  size_t size_ = 0;
};

template<typename T>
matrix<T> operator+(const matrix<T>& a, const matrix<T>& b)
{
  matrix<T> result{a.size()};
  for (size_t i = 0; i != a.size(); ++i)
    for (size_t j = 0; j != a.size(); ++j)
      result[i][j] = a[i][j] + b[i][j];
  return result;
}

template<typename T>
matrix<T> operator-(const matrix<T>& a, const matrix<T>& b)
{
  matrix<T> result{a.size()};
  for (size_t i = 0; i != result.size(); ++i)
    for (size_t j = 0; j != result.size(); ++j)
      result[i][j] = a[i][j] - b[i][j];
  return result;
}

template<typename T>
matrix<T> strassen(const matrix<T>& a, const matrix<T>& b)
{
  matrix<T> c{a.size()};

  if (a.size() == 1)
  {
    c[0][0] = a[0][0] * b[0][0];
    return c;
  }
  else
  {
    size_t size = a.size() / 2;
    
    matrix<T> a11{size};
    matrix<T> a12{size};
    matrix<T> a21{size};
    matrix<T> a22{size};

    matrix<T> b11{size};
    matrix<T> b12{size};
    matrix<T> b21{size};
    matrix<T> b22{size};

    for (size_t i = 0; i != size; ++i)
    {
      for (size_t j = 0; j != size; ++j)
      {
        a11[i][j] = a[i][j];
        a12[i][j] = a[i][j+size];
        a21[i][j] = a[i+size][j];
        a22[i][j] = a[i+size][j+size];

        b11[i][j] = b[i][j];
        b12[i][j] = b[i][j+size];
        b21[i][j] = b[i+size][j];
        b22[i][j] = b[i+size][j+size];
      }
    }

    matrix<T> m1 = strassen(a11 + a22, b11 + b22);
    matrix<T> m2 = strassen(a21 + a22, b11);
    matrix<T> m3 = strassen(a11, b12 - b22);
    matrix<T> m4 = strassen(a22, b21 - b11);
    matrix<T> m5 = strassen(a11 + a12, b22);
    matrix<T> m6 = strassen(a21 - a11, b11 + b12);
    matrix<T> m7 = strassen(a12 - a22, b21 + b22);

    matrix<T> c11{size};
    matrix<T> c12{size};
    matrix<T> c21{size};
    matrix<T> c22{size};

    c11 = m1 + m4 - m5 + m7;
    c12 = m3 + m5;
    c21 = m2 + m4;
    c22 = m1 - m2 + m3 + m6;

    for (size_t i = 0; i != size; ++i)
    {
      for (size_t j = 0; j != size; ++j)
      {
        c[i][j] = c11[i][j];
        c[i+size][j] = c21[i][j];
        c[i][j+size] = c12[i][j];
        c[i+size][j+size] = c22[i][j];
      }
    }

    return c;
  }
}

template<typename T>
void print_matrix(const matrix<T>& m)
{
  for (size_t i = 0; i != m.size(); ++i)
  {
    for (size_t j = 0; j != m.size(); ++j)
    {
      std::cout << m[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

int main()
{
  matrix<int> a{{1, 2, 3, 4, 5, 6, 2, 9, 2, 8, 5, 3, 1, 4, 6, 7}, 4};
  print_matrix(a);

  matrix<int> b{{9, 5, 8, 3, 6, 3, 90, 7, 1, 23, 4, 2, 7, 6, 3, 7}, 4};
  print_matrix(b);

  matrix<int> c = strassen(a, b);
  print_matrix(c);

  // should be:
  //  52 104 212  51 
  // 146 143 615 124
  //  92 167 765  93 
  //  88 197 413  92 
}
