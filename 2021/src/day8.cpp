#include <utils/utils.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <set>
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
  static auto map  = std::unordered_map<std::string, int> {};
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
    std::string pattern, std::unordered_map<char, std::set<char>> const& map)
{
  for (auto& c : pattern) {
    assert(map.at(c).size() == 1 && "Ambiguous pattern");
    c = *map.at(c).begin();
  }

  return decode(pattern);
}

template <typename Set>
Set intersect(Set const& a, Set const& b)
{
  auto result = Set {};
  std::set_intersection(
      a.begin(), a.end(), b.begin(), b.end(),
      std::inserter(result, result.begin()));
  return result;
}

char invert(char c, std::unordered_map<char, std::set<char>> const& map)
{
  return std::find_if(
             map.begin(), map.end(),
             [c](auto const& e) { return e.second.contains(c); })
      ->first;
}

int part_2_worker(example const& ex)
{
  auto possible = std::unordered_map<char, std::set<char>> {};
  for (auto c = 'a'; c <= 'g'; ++c) {
    possible[c] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
  }

  auto resolved = [&possible] {
    return std::all_of(possible.begin(), possible.end(), [](auto const& cs) {
      return cs.second.size() == 1;
    });
  };

  auto five_shared = std::set<char> {'a', 'b', 'c', 'd', 'e', 'f', 'g'};

  for (auto const& in : ex.inputs) {
    if (in.size() == 2) {
      for (auto c : in) {
        possible[c] = intersect(possible[c], std::set {'c', 'f'});
      }
    }

    if (in.size() == 3) {
      for (auto c : in) {
        possible[c] = intersect(possible[c], std::set {'a', 'c', 'f'});
      }
    }

    if (in.size() == 4) {
      for (auto c : in) {
        possible[c] = intersect(possible[c], std::set {'b', 'c', 'd', 'f'});
      }
    }

    if (in.size() == 5) {
      auto set    = std::set<char>(in.begin(), in.end());
      auto result = std::set<char> {};

      std::set_intersection(
          five_shared.begin(), five_shared.end(), set.begin(), set.end(),
          std::inserter(result, result.begin()));

      five_shared = result;
    }
  }

  for (auto& [k, cs] : possible) {
    if (cs.size() != 2) {
      cs.erase('c');
      cs.erase('f');
    }
  }

  for (auto& [k, cs] : possible) {
    if (cs.size() != 2) {
      cs.erase('b');
      cs.erase('d');
    }
  }

  for (auto& [k, cs] : possible) {
    if (cs.size() != 1) {
      cs.erase('a');
    }
  }

  auto adg = std::set<char> {'a', 'd', 'g'};
  for (auto c : five_shared) {
    auto result = std::set<char> {};

    std::set_intersection(
        possible[c].begin(), possible[c].end(), adg.begin(), adg.end(),
        std::inserter(result, result.begin()));

    possible[c] = result;
  }

  for (auto c = 'a'; c <= 'g'; ++c) {
    if (possible.at(c).size() == 1) {
      auto ec = *possible.at(c).begin();
      for (auto& e : possible) {
        if (e.second.size() != 1) {
          e.second.erase(ec);
        }
      }
    }
  }

  auto inverted_five = std::set {
      invert('a', possible), invert('b', possible), invert('d', possible),
      invert('g', possible)};
  for (auto const& in : ex.inputs) {
    auto set    = std::set(in.begin(), in.end());
    auto result = std::set<char> {};

    std::set_difference(
        set.begin(), set.end(), inverted_five.begin(), inverted_five.end(),
        std::inserter(result, result.begin()));

    if (result.size() == 1) {
      possible[*result.begin()] = {'f'};

      for (auto& e : possible) {
        if (e.second.size() != 1) {
          e.second.erase('f');
        }
      }
    }
  }

  auto output = 0;
  for (auto& out : ex.outputs) {
    output *= 10;
    output += shuffled_decode(out, possible);
  }
  return output;
}

int part_2(std::vector<example> const& input)
{
  return std::accumulate(
      input.begin(), input.end(), 0,
      [](auto acc, auto const& ex) { return acc + part_2_worker(ex); });
}

int main()
{
  auto in = get_input();

  std::cout << part_1(in) << '\n';
  std::cout << part_2(in) << '\n';
}
