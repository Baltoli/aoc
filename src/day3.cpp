#include <algorithm>
#include <iostream>
#include <set>

std::set<std::pair<int, int>> read_wire_line()
{
  auto ret = std::set<std::pair<int, int>> {};

  std::string line;
  std::getline(std::cin, line);

  int x = 0;
  int y = 0;
  int pos = 0;

  while (true) {
    char dir = line[pos++];
    int next_pos = line.find(",", pos);

    auto len = std::stoi(line.substr(pos, next_pos - pos));

    for (int i = 0; i < len; ++i) {
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

      ret.emplace(x, y);
    }

    if (next_pos == std::string::npos) {
      break;
    }

    pos = next_pos + 1;
  }

  return ret;
}

int main()
{
  auto x = read_wire_line();
  auto y = read_wire_line();
  auto cross = std::set<std::pair<int, int>> {};

  std::set_intersection(x.begin(), x.end(), y.begin(), y.end(),
      std::inserter(cross, cross.begin()));

  auto dists = std::set<int> {};
  std::transform(cross.begin(), cross.end(),
      std::inserter(dists, dists.begin()),
      [](auto p) { return std::abs(p.first) + std::abs(p.second); });

  std::cout << *std::min_element(dists.begin(), dists.end()) << '\n';
}
