#include <memory>
#include <utility>
#include <initializer_list>
#include <iostream>

class matrix
{
public:
  matrix() = default;
  
  explicit matrix(size_t size) : size_{size}
  {
    allocate();
  }

  matrix(std::initializer_list<int> data, size_t size) : size_{size}
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
    size_ = other.size_;
    data_ = other.data_;

    other.size_ = 0;
    other.data_ = nullptr;

    return *this;
  }

  int* operator[](size_t i)
  {
    return data_[i];
  }

  const int* operator[](size_t i) const
  {
    return data_[i];
  }

  size_t size() const { return size_; }

private:
  void allocate()
  {
    data_ = new int*[size_];
    for (size_t i = 0; i != size_; ++i)
      data_[i] = new int[size_];
  }

  void deallocate()
  {
    for (size_t i = 0; i != size_; ++i)
      delete [] data_[i];
    delete [] data_;
  }

  int** data_ = nullptr;
  size_t size_ = 0;
};

matrix operator+(const matrix& a, const matrix& b)
{
  matrix result{a.size()};
  for (size_t i = 0; i != a.size(); ++i)
    for (size_t j = 0; j != a.size(); ++j)
      result[i][j] = a[i][j] + b[i][j];
  return result;
}

matrix operator-(const matrix& a, const matrix& b)
{
  matrix result{a.size()};
  for (size_t i = 0; i != result.size(); ++i)
    for (size_t j = 0; j != result.size(); ++j)
      result[i][j] = a[i][j] - b[i][j];
  return result;
}

matrix strassen(const matrix& a, const matrix& b)
{
  matrix c{a.size()};

  if (a.size() == 1)
  {
    c[0][0] = a[0][0] * b[0][0];
    return c;
  }
  else
  {
    size_t size = a.size() / 2;
    
    matrix a11{size};
    matrix a12{size};
    matrix a21{size};
    matrix a22{size};

    matrix b11{size};
    matrix b12{size};
    matrix b21{size};
    matrix b22{size};

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

    matrix m1 = strassen(a11 + a22, b11 + b22);
    matrix m2 = strassen(a21 + a22, b11);
    matrix m3 = strassen(a11, b12 - b22);
    matrix m4 = strassen(a22, b21 - b11);
    matrix m5 = strassen(a11 + a12, b22);
    matrix m6 = strassen(a21 - a11, b11 + b12);
    matrix m7 = strassen(a12 - a22, b21 + b22);

    matrix c11{size};
    matrix c12{size};
    matrix c21{size};
    matrix c22{size};

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

void print_matrix(const matrix& m)
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
  matrix a{{1, 2, 2, 2}, 2};
  print_matrix(a);

  matrix b{{2, 2, 2, 2}, 2};
  print_matrix(b);

  matrix c = strassen(a, b);
  print_matrix(c);
}
