#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>
#include <set>

std::pair<int, int> compute(std::vector<std::string> const& in)
{
  auto heading     = 0;
  auto x           = 0;
  auto y           = 0;
  auto repeat_dist = -1;

  auto visited = std::set<std::pair<int, int>> {{0, 0}};

  for (auto const& dir : in) {
    auto m = ctre::match<R"((L|R)(\d+))">(dir);
    if (m.get<1>() == "L") {
      heading -= 1;
      if (heading < 0) {
        heading += 4;
      }
    } else {
      heading = (heading + 1) % 4;
    }

    auto dist = utils::stoi(m.get<2>().str());
    for (auto i = 0; i < dist; ++i) {
      switch (heading) {
      case 0:
        ++y;
        break;
      case 1:
        ++x;
        break;
      case 2:
        --y;
        break;
      case 3:
        --x;
        break;
      }

      auto [it, ins] = visited.insert({x, y});
      if (!ins && repeat_dist == -1) {
        repeat_dist = std::abs(x) + std::abs(y);
      }
    }
  }

  return {std::abs(x) + std::abs(y), repeat_dist};
}

int main()
{
  auto in = utils::split(utils::get_single_line(), ", ");

  auto [part_1, part_2] = compute(in);

  std::cout << part_1 << '\n';
  std::cout << part_2 << '\n';
}
