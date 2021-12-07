#include <utils/utils.h>

#include <iostream>
#include <map>
#include <sstream>
#include <utility>

struct line {
  int x1;
  int y1;
  int x2;
  int y2;

  line(std::string const& s)
  {
    auto parts = utils::split(s, " -> ");

    auto start = utils::split(parts[0], ",");
    auto end   = utils::split(parts[1], ",");

    x1 = utils::stoi(start[0]);
    y1 = utils::stoi(start[1]);
    x2 = utils::stoi(end[0]);
    y2 = utils::stoi(end[1]);
  }
};

std::map<std::pair<int, int>, int> precompute(std::vector<line> const& lines)
{
  auto map = std::map<std::pair<int, int>, int> {};

  for (auto const& l : lines) {
    if (l.x1 == l.x2) {
      for (auto y = std::min(l.y1, l.y2); y <= std::max(l.y1, l.y2); ++y) {
        map.try_emplace({l.x1, y}, 0);
        map.at({l.x1, y})++;
      }
    } else if (l.y1 == l.y2) {
      for (auto x = std::min(l.x1, l.x2); x <= std::max(l.x1, l.x2); ++x) {
        map.try_emplace({x, l.y1}, 0);
        map.at({x, l.y1})++;
      }
    }
  }

  return map;
}

int part_1(std::vector<line> const& lines)
{
  auto map = precompute(lines);
  return std::count_if(map.begin(), map.end(), [](auto const& entry) {
    return entry.second >= 2;
  });
}

int part_2(std::vector<line> const& lines)
{
  auto map = precompute(lines);

  for (auto const& l : lines) {
    if (std::abs(l.x1 - l.x2) == std::abs(l.y1 - l.y2)) {
      auto x_step = l.x1 < l.x2 ? 1 : -1;
      auto y_step = l.y1 < l.y2 ? 1 : -1;

      auto x = l.x1;
      auto y = l.y1;

      auto next = true;

      while (next) {
        if (x == l.x2) {
          next = false;
        }

        map.try_emplace({x, y}, 0);
        map.at({x, y})++;

        x += x_step;
        y += y_step;
      }
    }
  }

  return std::count_if(map.begin(), map.end(), [](auto const& entry) {
    return entry.second >= 2;
  });
}

int main()
{
  auto in = utils::map_lines([](auto const& l) { return line(l); });

  std::cout << part_1(in) << '\n';
  std::cout << part_2(in) << '\n';
}
