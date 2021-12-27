#include <utils/utils.h>

#include <fmt/format.h>

#include <vector>

struct board {
  std::vector<char> data;
  int               width;
  int               height;

  static board load()
  {
    auto ret = board();
    utils::for_each_line([&](auto const& line) {
      std::copy(line.begin(), line.end(), std::back_inserter(ret.data));
      ret.height++;
      ret.width = line.size();
    });
    return ret;
  }

  bool step()
  {
    bool any = false;

    auto next_x = [this](auto x) { return (x + 1) % width; };
    auto next_y = [this](auto y) { return (y + 1) % height; };

    auto x_blit = *this;
    for (auto y = 0; y < height; ++y) {
      for (auto x = 0; x < width; ++x) {
        if (at(x, y) == '>' && at(next_x(x), y) == '.') {
          x_blit.at(x, y)         = '.';
          x_blit.at(next_x(x), y) = '>';
          any                     = true;
        }
      }
    }

    auto y_blit = x_blit;
    for (auto y = 0; y < height; ++y) {
      for (auto x = 0; x < width; ++x) {
        if (x_blit.at(x, y) == 'v' && x_blit.at(x, next_y(y)) == '.') {
          y_blit.at(x, y)         = '.';
          y_blit.at(x, next_y(y)) = 'v';
          any                     = true;
        }
      }
    }

    *this = y_blit;
    return any;
  }

  char& at(int x, int y) { return data.at(x + y * width); }
};

int main()
{
  auto in = board::load();

  auto step = 0;

  while (true) {
    step++;
    if (!in.step()) {
      fmt::print("{}\n", step);
      break;
    }
  }
}
