#include <utils/utils.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

struct example {
  std::vector<std::string> inputs;
  std::vector<std::string> outputs;

  example(std::string const& line)
  {
    auto parts = utils::split(line, " | ");
    inputs     = utils::split(parts[0], " ");
    outputs    = utils::split(parts[1], " ");
  }
};

std::vector<example> get_input()
{
  return utils::map_lines([](auto const& line) { return example(line); });
}

int part_1(std::vector<example> const& input)
{
  return std::accumulate(
      input.begin(), input.end(), 0, [](auto acc, auto const& ex) {
        return acc
               + std::count_if(
                   ex.outputs.begin(), ex.outputs.end(), [](auto const& digit) {
                     auto s = digit.size();
                     return (s == 2) || (s == 3) || (s == 4) || (s == 7);
                   });
      });
}

int decode(std::string pattern)
{
  auto map = std::unordered_map<std::string, int> {};

  static bool once = false;
  if (!once) {
    map["abcefg"]  = 0;
    map["cf"]      = 1;
    map["acdeg"]   = 2;
    map["acdfg"]   = 3;
    map["bcdf"]    = 4;
    map["abdfg"]   = 5;
    map["abdefg"]  = 6;
    map["acf"]     = 7;
    map["abcdefg"] = 8;
    map["abcdfg"]  = 9;

    once = true;
  }

  std::sort(pattern.begin(), pattern.end());
  return map.at(pattern);
}

int shuffled_decode(
    std::string                                               pattern,
    std::unordered_map<char, std::unordered_set<char>> const& map)
{
  for (auto& c : pattern) {
    assert(map.at(c).size() == 1 && "Ambiguous pattern");
    c = *map.at(c).begin();
  }

  return decode(pattern);
}

int part_2(std::vector<example> const& input)
{
  auto possible = std::unordered_map<char, std::unordered_set<char>> {};

  auto resolved = [&possible] {
    return std::all_of(possible.begin(), possible.end(), [](auto const& cs) {
      return cs.second.size() == 1;
    });
  };

  return 0;
}

int main()
{
  auto in = get_input();

  std::cout << part_1(in) << '\n';
  std::cout << part_2(in) << '\n';
}
