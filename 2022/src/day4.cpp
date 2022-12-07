#include <utils/utils.h>

#include <cassert>
#include <iostream>

class range {
public:
  range(std::string const& line)
  {
    auto parts = utils::split(line, "-");
    assert(parts.size() == 2 && "Wrong split size");

    begin_ = utils::svtol(parts[0]);
    end_   = utils::svtol(parts[1]);
  }

  bool contains(range const& other)
  {
    return begin_ <= other.begin_ && end_ >= other.end_;
  }

  bool overlaps(range const& other)
  {
    return !(end_ < other.begin_ || begin_ > other.end_);
  }

private:
  long begin_;
  long end_;
};

int main()
{
  auto total_1 = 0;
  auto total_2 = 0;

  utils::for_each_line([&](auto const& line) {
    auto parts = utils::split(line, ",");
    assert(parts.size() == 2 && "Wrong split size");

    auto a = range(parts[0]);
    auto b = range(parts[1]);

    if (a.contains(b) || b.contains(a)) {
      total_1 += 1;
    }

    if (a.overlaps(b)) {
      total_2 += 1;
    }
  });

  std::cout << total_1 << '\n';
  std::cout << total_2 << '\n';
}
