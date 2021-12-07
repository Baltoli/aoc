#include <utils/utils.h>

#include <cassert>
#include <iomanip>
#include <ios>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>

struct board {
  int width  = 0;
  int height = 0;

  bool done = false;

  std::vector<int> data;

  bool wins(std::set<int> const& marked) const;
  int  score(std::set<int> const& marked) const;
};

bool board::wins(std::set<int> const& marked) const
{
  for (auto row = 0; row < height; ++row) {
    auto all_row = true;

    for (auto x = row * width; x < (row + 1) * width; ++x) {
      all_row = all_row && marked.contains(data[x]);
    }

    if (all_row) {
      return true;
    }
  }

  for (auto col = 0; col < width; ++col) {
    auto all_col = true;

    for (auto y = 0; y < height; ++y) {
      all_col = all_col && marked.contains(data[(y * width) + col]);
    }

    if (all_col) {
      return true;
    }
  }

  return false;
}

int board::score(std::set<int> const& marked) const
{
  auto sum = 0;
  for (auto n : data) {
    if (!marked.contains(n)) {
      sum += n;
    }
  }
  return sum;
}

template <typename It>
std::vector<board> get_boards(It begin, It end)
{
  auto ret = std::vector<board> {};

  auto current = board {};

  for (auto it = begin; it != end; ++it) {
    if (it->empty()) {
      assert(current.width > 0 && current.height > 0);
      assert(current.data.size() == current.width * current.height);

      ret.push_back(current);
      current = board {};
    } else {
      auto line = std::regex_replace(*it, std::regex("\\s+"), " ");
      line      = std::regex_replace(line, std::regex("^\\s"), "");

      auto numbers = utils::split(line, " ");

      current.width = numbers.size();
      current.height += 1;

      for (auto const& n : numbers) {
        current.data.push_back(utils::stoi(n));
      }
    }
  }

  return ret;
}

std::pair<int, int>
solution(std::vector<std::string> const& calls, std::vector<board>& boards)
{
  auto marks   = std::set<int> {};
  auto results = std::vector<int> {};

  for (auto const& c : calls) {
    auto nc = utils::stoi(c);
    marks.insert(nc);

    for (auto& b : boards) {
      if (b.wins(marks) && !b.done) {
        results.push_back(nc * b.score(marks));
        b.done = true;
      }
    }
  }

  return {results.front(), results.back()};
}

int main()
{
  auto raw_in = utils::get_lines();
  auto calls  = utils::split(raw_in[0], ",");

  auto boards = get_boards(raw_in.begin() + 2, raw_in.end());

  auto [part_1, part_2] = solution(calls, boards);
  std::cout << part_1 << '\n';
  std::cout << part_2 << '\n';
}
