#include <utils/utils.h>

#include <iostream>
#include <map>

class map {
public:
  int at(int x, int y) const
  {
    if (x < 0 || x >= width() || y < 0 || y >= height()) {
      return 9;
    }

    return data_.at(x + y * width_);
  }

  static map load()
  {
    auto ret = map();
    utils::for_each_line([&ret](auto const& line) {
      for (auto c : line) {
        ret.data_.push_back(utils::ctoi(c));
      }

      ret.width_ = line.size();
      ret.height_++;
    });
    return ret;
  }

  std::pair<bool, int> risk(int x, int y) const
  {
    auto val = at(x, y);

    return {
        (val < at(x - 1, y)) && (val < at(x + 1, y)) && (val < at(x, y - 1))
            && (val < at(x, y + 1)),
        val + 1};
  }

  int width() const { return width_; }
  int height() const { return height_; }

private:
  map()
      : data_()
      , width_(0)
      , height_(0)
  {
  }

  std::vector<int> data_;
  int              width_;
  int              height_;
};

int part_1(map const& m)
{
  int total = 0;

  for (auto y = 0; y < m.height(); ++y) {
    for (auto x = 0; x < m.height(); ++x) {
      auto [low, risk] = m.risk(x, y);
      if (low) {
        total += risk;
      }
    }
  }

  return total;
}

int main()
{
  auto data = map::load();

  std::cout << part_1(data) << '\n';
}
