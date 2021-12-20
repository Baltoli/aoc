#include <utils/utils.h>

#include <iostream>
#include <limits>
#include <unordered_set>

struct point {
  int x;
  int y;

  point operator+(point const& other) const
  {
    return {x + other.x, y + other.y};
  }
};

namespace std {
template <>
struct hash<point> {
  size_t operator()(point const& p) const
  {
    auto seed = size_t {0};
    utils::hash_combine(seed, p.x, p.y);
    return seed;
  }
};
} // namespace std

bool operator==(point const& a, point const& b)
{
  return a.x == b.x && a.y == b.y;
}

struct image {
  std::unordered_set<point> data;

  bool oob_on = false;

  point min
      = {std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
  point max
      = {std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};

  void add(point p)
  {
    min = {std::min(min.x, p.x), std::min(min.y, p.y)};
    max = {std::max(max.x, p.x), std::max(max.y, p.y)};
    data.insert(p);
  }

  bool at(point p) const
  {
    if (data.contains(p)) {
      return true;
    } else if (p.x < min.x || p.x > max.x || p.y < min.y || p.y > max.y) {
      return oob_on;
    } else {
      return false;
    }
  }

  int total_lit() const { return data.size(); }

  int index(point p) const
  {
    auto idx = 0;
    for (auto dy = -1; dy <= 1; ++dy) {
      for (auto dx = -1; dx <= 1; ++dx) {
        idx <<= 1;
        if (at({p.x + dx, p.y + dy})) {
          idx = idx | 1;
        }
      }
    }
    return idx;
  }

  image enhance(std::string_view alg)
  {
    auto new_img = image {};

    for (auto y = min.y - 1; y <= max.y + 1; ++y) {
      for (auto x = min.x - 1; x <= max.x + 1; ++x) {
        if (alg[index({x, y})] == '#') {
          new_img.add({x, y});
        }
      }
    }

    new_img.oob_on = !oob_on;
    return new_img;
  }
};

std::ostream& operator<<(std::ostream& os, image const& i)
{
  for (auto y = i.min.y; y <= i.max.y; ++y) {
    for (auto x = i.min.x; x <= i.max.x; ++x) {
      if (i.at({x, y})) {
        os << '#';
      } else {
        os << '.';
      }
    }
    os << '\n';
  }
  return os;
}

std::pair<std::string, image> load()
{
  auto lines = utils::get_lines();
  auto img   = image {};

  for (auto i = 2; i < lines.size(); ++i) {
    auto x = 0;
    auto y = i - 2;
    for (auto c : lines[i]) {
      if (c == '#') {
        img.add({x, y});
      }
      ++x;
    }
  }

  return {lines[0], img};
}

int compute(std::string_view alg, image img, int n)
{
  for (auto i = 0; i < n; ++i) {
    img = img.enhance(alg);
  }

  return img.total_lit();
}

int main()
{
  auto [alg, in_image] = load();

  std::cout << compute(alg, in_image, 2) << '\n';
  std::cout << compute(alg, in_image, 50) << '\n';

  return 0;
}
