#include <utils/utils.h>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

struct seats {
  seats()
      : rows(utils::get_lines())
  {
  }

  int width() const { return rows[0].size(); }
  int height() const { return rows.size(); }

  char&       at(int x, int y) { return rows[y][x]; }
  char const& at(int x, int y) const { return rows[y][x]; }
  char&       b_at(int x, int y) { return blit[y][x]; }
  char const& b_at(int x, int y) const { return blit[y][x]; }

  int occupied_near(int x, int y) const
  {
    auto sum = 0;
    for (auto dx = -1; dx <= 1; ++dx) {
      for (auto dy = -1; dy <= 1; ++dy) {
        if ((dx != 0 || dy != 0) && (x + dx >= 0) && (x + dx < width())
            && (y + dy >= 0) && (y + dy < height())) {
          sum += (at(x + dx, y + dy) == '#' ? 1 : 0);
        }
      }
    }
    return sum;
  }

  int visible(int x, int y) const
  {
    auto sum = 0;
    for (auto dx = -1; dx <= 1; ++dx) {
      for (auto dy = -1; dy <= 1; ++dy) {
        if (dx != 0 || dy != 0) {
          for (auto nx = x + dx, ny = y + dy;
               nx >= 0 && ny >= 0 && nx < width() && ny < height();
               nx += dx, ny += dy) {
            if (at(nx, ny) == '#') {
              ++sum;
              break;
            } else if (at(nx, ny) == 'L') {
              break;
            }
          }
        }
      }
    }
    return sum;
  }

  bool step(bool sight)
  {
    using std::swap;

    bool changed = false;
    blit         = rows;

    for (auto x = 0; x < width(); ++x) {
      for (auto y = 0; y < height(); ++y) {
        int near = sight ? visible(x, y) : occupied_near(x, y);
        if (at(x, y) == 'L' && near == 0) {
          b_at(x, y) = '#';
          changed    = true;
        } else if (at(x, y) == '#' && near >= (sight ? 5 : 4)) {
          b_at(x, y) = 'L';
          changed    = true;
        }
      }
    }

    swap(blit, rows);
    return changed;
  }

  int occupied() const
  {
    return std::accumulate(
        rows.begin(), rows.end(), 0, [](auto acc, auto const& row) {
          return acc + std::count(row.begin(), row.end(), '#');
        });
  }

  std::vector<std::string> rows;
  std::vector<std::string> blit;
};

int iterate(seats s, bool sight)
{
  while (s.step(sight)) {
  }

  return s.occupied();
}

int main()
{
  auto in = seats();
  std::cout << iterate(in, false) << '\n';
  std::cout << iterate(in, true) << '\n';
}
