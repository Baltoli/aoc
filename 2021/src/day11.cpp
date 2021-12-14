#include <utils/utils.h>

#include <iostream>
#include <set>
#include <sstream>

class map {
public:
  static map load()
  {
    auto ret = map();

    utils::for_each_line([&ret](auto const& line) {
      ret.height_++;
      for (auto c : line) {
        ret.width_ = line.size();
        ret.data_.push_back(utils::ctoi(c));
      }
    });

    return ret;
  }

  int width() const { return width_; }
  int height() const { return height_; }

  int& at(int x, int y) { return data_.at(y * width_ + x); }

  int step()
  {
    auto flashes = std::set<std::pair<int, int>> {};
    for (auto& o : data_) {
      o++;
    }

    bool any;
    do {
      any = false;

      for (auto y = 0; y < height(); ++y) {
        for (auto x = 0; x < width(); ++x) {
          if (at(x, y) > 9 && !flashes.contains({x, y})) {
            flashes.insert({x, y});
            any = true;

            for (auto dy = -1; dy <= 1; ++dy) {
              for (auto dx = -1; dx <= 1; ++dx) {
                if ((dy != 0 || dx != 0) && (y + dy >= 0 && y + dy < height())
                    && (x + dx >= 0 && x + dx < width())) {
                  at(x + dx, y + dy) += 1;
                }
              }
            }
          }
        }
      }
    } while (any);

    for (auto& o : data_) {
      if (o > 9) {
        o = 0;
      }
    }

    return flashes.size();
  }

private:
  map() = default;

  int              width_  = 0;
  int              height_ = 0;
  std::vector<int> data_   = {};
};

int part_1(map m)
{
  auto sum = 0;
  for (auto i = 0; i < 100; ++i) {
    sum += m.step();
  }
  return sum;
}

int part_2(map m)
{
  for (int i = 1;; i++) {
    auto f = m.step();
    if (f == m.width() * m.height()) {
      return i;
    }
  }
}

int main()
{
  auto map = map::load();

  std::cout << part_1(map) << '\n';
  std::cout << part_2(map) << '\n';
}
