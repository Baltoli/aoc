#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <map>
#include <optional>

using point = std::pair<int, int>;

class cave {
public:
  cave(bool wall)
      : wall_(wall)
      , data_ {}
      , max_y_(std::nullopt)
  {
  }

  void dump()
  {
    auto x_cmp = [](auto const& a, auto const& b) {
      return a.first.first < b.first.first;
    };
    auto y_cmp = [](auto const& a, auto const& b) {
      return a.first.second < b.first.second;
    };

    auto min_x
        = std::min_element(data_.begin(), data_.end(), x_cmp)->first.first;
    auto max_x
        = std::max_element(data_.begin(), data_.end(), x_cmp)->first.first;

    auto min_y
        = std::min_element(data_.begin(), data_.end(), y_cmp)->first.second;
    auto max_y
        = std::max_element(data_.begin(), data_.end(), y_cmp)->first.second;

    for (auto y = min_y; y <= max_y; ++y) {
      for (auto x = min_x; x <= max_x; ++x) {
        fmt::print("{}", at({x, y}));
      }
      fmt::print("\n");
    }
  }

  int max_y()
  {
    if (!max_y_) {
      auto y_cmp = [](auto const& a, auto const& b) {
        return a.first.second < b.first.second;
      };
      max_y_
          = std::max_element(data_.begin(), data_.end(), y_cmp)->first.second;
    }

    return *max_y_;
  }

  int run()
  {
    int count = 0;

    while (true) {
      bool moved   = false;
      auto start   = point {500, 0};
      auto current = start;

      do {
        moved       = false;
        at(current) = 'o';

        auto left   = point {current.first - 1, current.second + 1};
        auto center = point {current.first, current.second + 1};
        auto right  = point {current.first + 1, current.second + 1};

        for (auto next : {center, left, right}) {
          if (at(next) == '.') {
            at(next)    = 'o';
            at(current) = '.';
            current     = next;
            moved       = true;
            break;
          }
        }
      } while (moved && current.second <= max_y() + 1);

      if (moved) {
        return count;
      } else if (current == start) {
        return count + 1;
      } else {
        count++;
      }
    }
  }

  char& at(point p, bool draw = false)
  {
    if (data_.find(p) == data_.end()) {
      if (wall_ && !draw && p.second == max_y() + 2) {
        data_[p] = '#';
      } else {
        data_[p] = '.';
      }
    }

    return data_.at(p);
  }

  void draw(std::string const& line)
  {
    auto elts   = utils::split(line, " -> ");
    auto coords = utils::map(elts, [](auto const& e) {
      auto cs = utils::split(e, ",");
      return std::pair {utils::stoi(cs[0]), utils::stoi(cs[1])};
    });

    for (auto i = 0; i < coords.size() - 1; ++i) {
      auto start = coords[i];
      auto end   = coords[i + 1];

      if (start.first == end.first) {
        auto s_y = std::min(start.second, end.second);
        auto e_y = std::max(start.second, end.second);

        for (auto y = s_y; y <= e_y; ++y) {
          at(point {start.first, y}, true) = '#';
        }
      } else if (start.second == end.second) {
        auto s_x = std::min(start.first, end.first);
        auto e_x = std::max(start.first, end.first);

        for (auto x = s_x; x <= e_x; ++x) {
          at(point {x, start.second}, true) = '#';
        }
      } else {
        assert(false);
      }
    }
  }

private:
  bool                  wall_;
  std::optional<int>    max_y_;
  std::map<point, char> data_;
};

int main()
{
  auto c  = cave(false);
  auto c2 = cave(true);

  utils::for_each_line([&](auto const& l) {
    c.draw(l);
    c2.draw(l);
  });

  fmt::print("{}\n", c.run());
  fmt::print("{}\n", c2.run());
}
