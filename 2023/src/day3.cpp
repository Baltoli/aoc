#include <utils/utils.h>

#include <fmt/format.h>

#include <optional>

class map {
public:
  map(std::vector<std::string> const& lines)
      : height_(0)
      , width_(0)
      , data_ {}
  {
    for (auto const& line : lines) {
      data_ += line;
      height_ += 1;
      width_ = line.size();
    }
  }

  std::optional<long> number(long x, long y) const
  {
    if (!std::isdigit(at(x, y))) {
      return std::nullopt;
    }

    auto start = x;
    auto end   = x;

    while (std::isdigit(at(--start, y))) {
    }

    while (std::isdigit(at(++end, y))) {
    }

    return utils::svtol(std::string_view(
        data_.begin() + idx(start + 1, y), data_.begin() + idx(end, y)));
  }

  long part_1() const
  {
    auto sum = 0L;

    for (auto y = 0L; y < height_; ++y) {
      for (auto x = 0L; x < width_; ++x) {
        for (auto part : part_numbers(x, y)) {
          sum += part;
        }
      }
    }

    return sum;
  }

  long part_2() const
  {
    auto sum = 0L;

    for (auto y = 0L; y < height_; ++y) {
      for (auto x = 0L; x < width_; ++x) {
        sum += gear_ratio(x, y);
      }
    }

    return sum;
  }

private:
  std::vector<long> part_numbers(long x, long y) const
  {
    auto vals = std::set<std::pair<long, long>> {};

    if (is_symbol(at(x, y))) {
      for (auto [x, y] : neighbours(x, y)) {
        if (auto num = number(x, y)) {
          vals.insert({*num, y});
        }
      }
    }

    auto ret = std::vector<long> {};
    for (auto [v, y] : vals) {
      ret.push_back(v);
    }

    return ret;
  }

  long gear_ratio(long x, long y) const
  {
    auto numbers = part_numbers(x, y);

    if (numbers.size() == 2 && at(x, y) == '*') {
      return numbers[0] * numbers[1];
    } else {
      return 0;
    }
  }

  std::vector<std::pair<long, long>> neighbours(long x, long y) const
  {
    auto ret = std::vector<std::pair<long, long>> {};

    for (auto dy = -1; dy <= 1; ++dy) {
      for (auto dx = -1; dx <= 1; ++dx) {
        if ((dx == 0 && dy == 0) || x + dx < 0 || x + dx >= width_ || y + dy < 0
            || y + dy >= height_) {
          continue;
        }

        ret.emplace_back(x + dx, y + dy);
      }
    }

    return ret;
  }

  long idx(long x, long y) const { return y * width_ + x; }

  char at(long x, long y) const
  {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
      return '.';
    }

    return data_[idx(x, y)];
  }

  bool is_symbol(char c) const { return !std::isdigit(c) && c != '.'; }

  long        height_;
  long        width_;
  std::string data_;
};

int main()
{
  auto input = map(utils::get_lines());

  fmt::print("{}\n", input.part_1());
  fmt::print("{}\n", input.part_2());
}
