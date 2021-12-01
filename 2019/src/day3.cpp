#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <set>

std::map<std::pair<int, int>, int> read_wire_line(std::string const& line)
{
  auto ret = std::map<std::pair<int, int>, int> {};

  int x     = 0;
  int y     = 0;
  int pos   = 0;
  int steps = 0;

  while (true) {
    char dir      = line[pos++];
    int  next_pos = line.find(",", pos);

    auto len = std::stoi(line.substr(pos, next_pos - pos));

    for (int i = 0; i < len; ++i) {
      steps++;

      switch (std::toupper(dir)) {
      case 'R':
        x += 1;
        break;
      case 'L':
        x -= 1;
        break;
      case 'U':
        y += 1;
        break;
      case 'D':
        y -= 1;
        break;
      }

      ret.try_emplace({x, y}, steps);
    }

    if (next_pos == std::string::npos) {
      break;
    }

    pos = next_pos + 1;
  }

  return ret;
}

std::set<std::pair<int, int>>
to_part_1(std::map<std::pair<int, int>, int> const& steps)
{
  std::set<std::pair<int, int>> ret {};

  std::transform(
      steps.begin(), steps.end(), std::inserter(ret, ret.begin()),
      [](auto p) { return p.first; });

  return ret;
}

int main()
{
  std::string line;

  std::getline(std::cin, line);
  auto x_steps = read_wire_line(line);

  std::getline(std::cin, line);
  auto y_steps = read_wire_line(line);

  auto x = to_part_1(x_steps);
  auto y = to_part_1(y_steps);

  // Part 1

  auto cross = std::set<std::pair<int, int>> {};

  std::set_intersection(
      x.begin(), x.end(), y.begin(), y.end(),
      std::inserter(cross, cross.begin()));

  auto dists = std::set<int> {};
  std::transform(
      cross.begin(), cross.end(), std::inserter(dists, dists.begin()),
      [](auto p) { return std::abs(p.first) + std::abs(p.second); });

  std::cout << *std::min_element(dists.begin(), dists.end()) << '\n';

  // Part 2
  auto min_steps = std::numeric_limits<int>::max();
  for (auto inter : cross) {
    int steps = x_steps.at(inter) + y_steps.at(inter);
    if (steps < min_steps) {
      min_steps = steps;
    }
  }

  std::cout << min_steps << '\n';
}
