#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>

// px = p sorted by x coordinate
// py = p sorted by y coordinate
// qx = left half of px
// qy = left half of py
// rx = right half of px
// ry = right half of py
//
// (p1, q1) = closest_pair(qx, qy)
// (p2, q2) = closest_pair(rx, ry)
// delta = min(d(p1, q1), d(p2, q2))
// (p3, q3) = closest_split_pair(px, py, delta)
//
// base case = for 2,3 points can do brute force
// target = cooridnates of points closest to each other on 2-D plane
//
// algorithm
// 1. sort input array by x and y
// 2. split input array into two halves
// 3. compute closest pair in each half
// 4. compute closest split pair (one point in left, second one in right)
//
//
// closest_split_pair(px, py, delta):
//   1. xt = biggest x-coordinate in left of p
//   2. sy = points of p with x-coordinate in [xt - delta, xt + delta] sorted by y-coordinate
//   3. best = delta, best_pair = nil
//      for i = 1 to size(sy) - 1:
//        for j = 1 to min(7, size(sy)-i):
//          (p,q) = ith, (i+j)th of sy
//          if d(p,q) < best:
//            best_pair = (p,q),
//            best = d(p,q)

struct point
{
  int x;
  int y;
};

unsigned distance(const point& x, const point& y)
{
  auto dx = x.x - y.x;
  auto dy = x.y - y.y;
  return std::sqrt(dx*dx + dy*dy);
}

std::pair<point, point> closest_pair(const std::vector<point>& x, const std::vector<point>& y)
{
  if ((x.size() <= 3) && (y.size() <= 3))
  {
    std::pair<point, point> result;
    unsigned min_distance = -1;
    for (size_t i = 0; i < x.size() - 1; ++i)
    {
      auto d = distance(x[i], x[i+1]);
      if (d < min_distance)
      {
        min_distance = d;
        result = {x[i], x[i+1]};
      }
    }

    for (size_t i = 0; i < y.size() - 1; ++i)
    {
      auto d = distance(y[i], y[i+1]);
      if (d < min_distance)
      {
        min_distance = d;
        result = {y[i], y[i+1]};
      }
    }
    return result;
  }

  std::vector<point> leftx;
  std::vector<point> rightx;
  std::vector<point> lefty;
  std::vector<point> righty;

  auto middlex = x.begin() + x.size() / 2;
  auto middley = y.begin() + y.size() / 2;

  std::copy(x.begin(), middlex, std::back_inserter(leftx));
  std::copy(middlex, x.end(), std::back_inserter(rightx));
  std::copy(y.begin(), middley, std::back_inserter(lefty));
  std::copy(middley, y.end(), std::back_inserter(righty));

  auto result1 = closest_pair(leftx, lefty);
  auto result2 = closest_pair(rightx, righty);

  return (distance(result1.first, result1.second) < distance(result2.first, result2.second) ? result1 : result2);
}

std::pair<point, point> closest_split_pair(const std::vector<point>& x, const std::vector<point>& y, unsigned delta)
{
  point xt = *(x.begin() + x.size() / 2);
  std::vector<point> sy;
  for (const auto& p : y)
    if (p.x <= xt.x + delta || p.x >= xt.x - delta)
      sy.push_back(p);

  std::pair<point, point> result;
  unsigned dist = delta;

  for (size_t i = 0; i < sy.size() - 1; ++i)
  {
    for (size_t j = 1; j < std::min(size_t(7), sy.size() - 1); ++j)
    {
      auto d = distance(sy[i], sy[i+j]);
      if (d < dist)
      {
        result.first = sy[i];
        result.second = sy[i+j];
        dist = d;
      }
    }
  }

  return result;
}

int main()
{
  std::vector<point> data{{1, 7}, {5, 23}, {9, 3}, {-23, 5}, {45, 4}, {14, 12}};
  for (size_t i = 0; i < data.size(); ++i)
  {
    for (size_t j = i+1; j < data.size(); ++j)
    {
      std::cout << "distance([" << data[i].x << ", " << data[i].y << "], [" << data[j].x << ", " << data[j].y << "]): " << distance(data[i], data[j]) << std::endl;
    }
  }

  std::vector<point> px = data;
  std::vector<point> py = data;
  std::sort(px.begin(), px.end(), [](const point& x, const point& y) { return x.x < y.x; });
  std::sort(py.begin(), py.end(), [](const point& x, const point& y) { return x.y < y.y; });
  auto res = closest_pair(px, py);
  auto res2 = closest_split_pair(px, py, distance(res.first, res.second));
  std::cout << "(" << res.first.x << ", " << res.first.y << "), (" << res.second.x << ", " << res.second.y << "), distance: " << distance(res.first, res.second) << std::endl;
  std::cout << "(" << res2.first.x << ", " << res2.first.y << "), (" << res2.second.x << ", " << res2.second.y << "), distance: " << distance(res2.first, res2.second) << std::endl;
}
