#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <limits>
#include <map>
#include <set>
#include <vector>

using point = std::pair<int, int>;

class game {
public:
  game(std::vector<std::string> const& lines)
  {
    auto row = 0;
    for (auto const& line : lines) {
      auto col = 0;

      for (auto val : line) {
        if (val == 'S') {
          start_ = {col, row};
          map_.push_back('a');
        } else if (val == 'E') {
          end_ = {col, row};
          map_.push_back('z');
        } else {
          map_.push_back(val);
        }

        col++;
      }

      row++;
      width_ = col;
    }
    height_ = row;
  }

  char at(int x, int y) const { return map_.at(y * width_ + x); }
  char at(point p) const { return at(p.first, p.second); }

  bool steppable(point start, point end) const
  {
    assert(std::abs(start.first - end.first) <= 1 && "X too far");
    assert(std::abs(start.second - end.second) <= 1 && "Y too far");

    return at(end) - at(start) <= 1;
  }

  std::set<point> low_points() const
  {
    auto ret = std::set<point> {};

    for (auto y = 0; y < height_; ++y) {
      for (auto x = 0; x < width_; ++x) {
        if (at(x, y) == 'a') {
          ret.insert({x, y});
        }
      }
    }

    return ret;
  }

  int distance(point start, point end) const
  {
    auto visited   = std::set<point> {};
    auto tentative = std::map<point, int> {{start, 0}};

    auto tentative_distance = [&](auto p) {
      if (tentative.find(p) == tentative.end()) {
        return std::numeric_limits<int>::max() - 1;
      } else {
        return tentative.at(p);
      }
    };

    auto is_unvisited = [&](auto p) {
      if (p.first < 0 || p.first >= width_ || p.second < 0
          || p.second >= height_ || visited.find(p) != visited.end()) {
        return false;
      } else {
        return true;
      }
    };

    auto next = [&] {
      return std::min_element(
                 tentative.begin(), tentative.end(),
                 [](auto a, auto b) { return a.second < b.second; })
          ->first;
    };

    auto current = start;

    while (true) {
      for (auto dx = -1; dx <= 1; ++dx) {
        for (auto dy = -1; dy <= 1; ++dy) {
          if (dx != 0 && dy != 0) {
            continue;
          }

          auto neighbour = point {current.first + dx, current.second + dy};
          if (is_unvisited(neighbour)) {
            if (steppable(current, neighbour)) {
              auto best = std::min(
                  tentative_distance(current) + 1,
                  tentative_distance(neighbour));

              tentative[neighbour] = best;
            }
          }
        }
      }

      visited.insert(current);
      if (current == end) {
        return tentative.at(current);
      }

      tentative.erase(current);
      current = next();
    }

    __builtin_unreachable();
  }

  point start() const { return start_; }
  point end() const { return end_; }

private:
  point             start_;
  point             end_;
  int               width_;
  int               height_;
  std::vector<char> map_;
};

int main()
{
  auto m = game(utils::get_lines());
  fmt::print("{}\n", m.distance(m.start(), m.end()));

  auto low_points = m.low_points();
  auto best       = std::numeric_limits<int>::max();
  for (auto const& low : low_points) {
    auto dist = m.distance(low, m.end());
    if (dist < best) {
      best = dist;
    }
  }
  fmt::print("{}\n", best);
}
