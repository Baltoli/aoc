#include <utils/utils.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>

int part_1(std::vector<std::string> const& lines)
{
  auto total   = 0;
  auto answers = std::set<char> {};

  for (auto const& line : lines) {
    if (line.empty()) {
      total += answers.size();
      answers.clear();
    }

    for (auto c : line) {
      answers.insert(c);
    }
  }

  return total;
}

int part_2(std::vector<std::string> const& lines)
{
  auto total       = 0;
  bool group_start = true;
  auto group_chars = std::set<char> {};

  for (auto const& line : lines) {
    if (line.empty()) {
      total += group_chars.size();
      group_chars.clear();
      group_start = true;
      continue;
    }

    if (group_start) {
      for (auto c : line) {
        group_chars.insert(c);
      }
    } else {
      auto line_chars = std::set<char> {};
      auto inter      = std::set<char> {};

      for (auto c : line) {
        line_chars.insert(c);
      }

      std::set_intersection(
          line_chars.begin(), line_chars.end(), group_chars.begin(),
          group_chars.end(), std::inserter(inter, inter.begin()));

      group_chars = inter;
    }

    group_start = false;
  }

  return total;
}

int main()
{
  auto in = utils::get_lines();

  std::cout << part_1(in) << '\n';
  std::cout << part_2(in) << '\n';
}
