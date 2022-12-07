#include <utils/utils.h>

#include <cassert>
#include <cctype>
#include <iostream>
#include <map>
#include <numeric>
#include <set>

int priority(char c)
{
  if (std::islower(c)) {
    return c - 96;
  } else if (std::isupper(c)) {
    return c - 38;
  } else {
    abort();
  }
}

std::set<char> character_set(std::string_view str)
{
  auto chars = std::set<char> {};
  auto it    = chars.begin();
  std::copy(str.begin(), str.end(), std::inserter(chars, it));
  return chars;
}

char rucksack_2(std::vector<std::string> const& entries)
{
  assert(!entries.empty() && "Need entries");

  auto shared_set = character_set(entries[0]);
  for (auto e : entries) {
    shared_set = utils::intersect(shared_set, character_set(e));
  }

  assert(shared_set.size() == 1 && "Shared count is wrong");
  return *shared_set.begin();
}

char rucksack_2(std::string const& line)
{
  return rucksack_2(std::vector {
      line.substr(0, line.size() / 2),
      line.substr(line.size() / 2, line.size() / 2)});
}

template <typename T>
int score(T const& data)
{
  return std::accumulate(
      data.begin(), data.end(), 0, [](auto acc, auto const& line) {
        return acc + priority(rucksack_2(line));
      });
}

int main()
{
  auto lines   = utils::get_lines();
  auto grouped = utils::group(lines, 3);

  std::cout << score(lines) << '\n';
  std::cout << score(grouped) << '\n';
}
