#include <utils/utils.h>

#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>
#include <string>

class parcel {
public:
  parcel(std::string const&);

  int area_req() const;

  int volume() const;
  int ribbon_req() const;

private:
  std::vector<int> sides_;
};

parcel::parcel(std::string const& spec)
    : sides_ {}
{
  for (auto s : utils::split(spec, "x")) {
    sides_.push_back(std::stoi(s));
  }

  std::sort(sides_.begin(), sides_.end());
}

int parcel::area_req() const
{
  return (3 * sides_[0] * sides_[1]) + (2 * sides_[0] * sides_[2])
         + (2 * sides_[1] * sides_[2]);
}

int parcel::volume() const
{
  return std::accumulate(sides_.begin(), sides_.end(), 1, std::multiplies {});
}

int parcel::ribbon_req() const
{
  return (2 * sides_[0]) + (2 * sides_[1]) + volume();
}

int main()
{
  auto area_sum = 0;
  auto rib_sum  = 0;

  utils::for_each_line([&](auto line) {
    auto p = parcel(line);

    area_sum += p.area_req();
    rib_sum += p.ribbon_req();
  });

  std::cout << area_sum << '\n';
  std::cout << rib_sum << '\n';
}
