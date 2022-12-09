#include <utils/utils.h>

#include <limits>
#include <vector>

class grid {
public:
  static grid load();

  int at(int x, int y) const { return data_.at(y * width_ + x); }

  bool edge(int x, int y) const
  {
    return x == 0 || x == width_ - 1 || y == 0 || y == height_ - 1;
  }

  bool visible(int x, int y) const
  {
    if (edge(x, y)) {
      return true;
    }

    auto vectors
        = std::vector<std::pair<int, int>> {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (auto [dx, dy] : vectors) {
      auto all = true;

      for (auto cx = x + dx, cy = y + dy; !edge(cx - dx, cy - dy);
           cx += dx, cy += dy) {
        all = all && (at(cx, cy) < at(x, y));
      }

      if (all) {
        return true;
      }
    }

    return false;
  }

  int count_visible() const
  {
    auto total = 0;
    for (int y = 0; y < height_; ++y) {
      for (int x = 0; x < width_; ++x) {
        total += visible(x, y);
      }
    }
    return total;
  }

  int score(int x, int y) const
  {
    auto vectors
        = std::vector<std::pair<int, int>> {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    auto score = 1;

    for (auto [dx, dy] : vectors) {
      auto this_score = 0;
      for (auto cx = x + dx, cy = y + dy; !edge(cx - dx, cy - dy);
           cx += dx, cy += dy) {
        this_score++;

        if (at(cx, cy) >= at(x, y)) {
          break;
        }
      }
      score *= this_score;
    }

    return score;
  }

  int max_score() const
  {
    auto max = std::numeric_limits<int>::min();
    for (int y = 0; y < height_; ++y) {
      for (int x = 0; x < width_; ++x) {
        max = std::max(max, score(x, y));
      }
    }
    return max;
  }

private:
  grid()
      : width_(0)
      , height_(0)
      , data_ {}
  {
  }

  int              width_;
  int              height_;
  std::vector<int> data_;
};

grid grid::load()
{
  auto ret = grid();

  utils::for_each_line([&](auto const& line) {
    ret.width_ = line.size();
    ret.height_ += 1;

    for (auto c : line) {
      ret.data_.push_back(utils::ctoi(c));
    }
  });

  return ret;
}

int main()
{
  auto in = grid::load();

  std::cout << in.count_visible() << '\n';
  std::cout << in.max_score() << '\n';
}
