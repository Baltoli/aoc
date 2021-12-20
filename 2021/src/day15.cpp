#include <utils/utils.h>

#include <iostream>
#include <limits>
#include <unordered_map>
#include <unordered_set>

struct point {
  int x;
  int y;
  int risk;

  point(int x, int y, int r)
      : x(x)
      , y(y)
      , risk(r)
  {
  }

  point(int x, int y)
      : point(x, y, 0)
  {
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

class map {
public:
  static map load()
  {
    auto ret = map {};
    utils::for_each_line([&ret](auto const& line) {
      ret.width_ = line.size();
      ret.height_ += 1;

      for (auto c : line) {
        ret.data_.push_back(utils::ctoi(c));
      }
    });
    return ret;
  }

  std::vector<int> const& data() const { return data_; }
  virtual int             width() const { return width_; }
  virtual int             height() const { return height_; }

  virtual int risk_at(int x, int y) const { return data_.at(y * width() + x); }
  point       at(int x, int y) const { return point(x, y, risk_at(x, y)); }

  int shortest_path()
  {
    auto x = width() - 1;
    auto y = height() - 1;

    auto visited   = std::unordered_set<point> {};
    auto distances = std::unordered_map<point, int> {};
    auto heap_cmp  = [&distances](auto const& a, auto const& b) {
      return distances.at(a) > distances.at(b);
    };

    auto initial       = point(0, 0, risk_at(0, 0));
    distances[initial] = 0;

    auto queue = std::vector<point> {initial};
    while (true) {
      std::pop_heap(queue.begin(), queue.end(), heap_cmp);
      auto current = queue.back();
      queue.pop_back();

      if (visited.contains(current)) {
        continue;
      }

      for (auto dy = -1; dy <= 1; ++dy) {
        for (auto dx = -1; dx <= 1; ++dx) {
          auto ny = current.y + dy;
          auto nx = current.x + dx;
          if (std::abs(dx) != std::abs(dy) && ny >= 0 && ny < height()
              && nx >= 0 && nx < width()) {
            auto np = at(nx, ny);
            if (!visited.contains(np)) {
              distances.try_emplace(np, std::numeric_limits<int>::max());
              distances[np]
                  = std::min(distances[np], distances[current] + np.risk);

              queue.push_back(np);
              std::push_heap(queue.begin(), queue.end(), heap_cmp);
            }
          }
        }
      }

      visited.insert(current);
      if (current.x == x && current.y == y) {
        return distances[current];
      }
    }

    return 0;
  }

protected:
  std::vector<int> data_   = {};
  int              width_  = 0;
  int              height_ = 0;
};

class big_map : public map {
public:
  big_map(map const& m)
      : map(m)
  {
    data_   = m.data();
    width_  = m.width();
    height_ = m.height();
  }

  virtual int width() const override { return width_ * 5; }
  virtual int height() const override { return height_ * 5; }

  virtual int risk_at(int x, int y) const override
  {
    int x_scale = x / width_;
    int y_scale = y / height_;

    auto real_x = x % width_;
    auto real_y = y % height_;

    auto val = data_.at(real_y * width_ + real_x) + x_scale + y_scale;
    if (val > 9) {
      val = val % 9;
    }
    return val;
  }
};

int main()
{
  auto m  = map::load();
  auto bm = big_map(m);

  std::cout << m.shortest_path() << '\n';
  std::cout << bm.shortest_path() << '\n';
}
