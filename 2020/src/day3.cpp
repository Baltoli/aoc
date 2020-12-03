#include <utils/utils.h>

#include <iostream>

int main()
{
  auto right_step = 3;
  auto x_coord    = 0;
  auto trees_hit  = 0;

  utils::for_each_line([&](auto const& line) {
    auto width = line.size();

    if (line[x_coord] == '#') {
      ++trees_hit;
    }

    x_coord = (x_coord + right_step) % width;
  });

  std::cout << trees_hit << '\n';
}
