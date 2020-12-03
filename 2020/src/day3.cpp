#include <utils/utils.h>

#include <iostream>

long trees_hit(std::vector<std::string> const& map, int right, int down)
{
  auto ret     = 0l;
  auto x_coord = 0;

  for (auto y = 0; y < map.size(); y += down) {
    auto width = map[y].size();

    if (map[y][x_coord] == '#') {
      ++ret;
    }

    x_coord = (x_coord + right) % width;
  }

  return ret;
}

int main()
{
  auto in = utils::get_lines();

  std::cout << trees_hit(in, 3, 1) << '\n';

  auto p2 = trees_hit(in, 1, 1) * trees_hit(in, 3, 1) * trees_hit(in, 5, 1)
            * trees_hit(in, 7, 1) * trees_hit(in, 1, 2);

  std::cout << p2 << '\n';
}
