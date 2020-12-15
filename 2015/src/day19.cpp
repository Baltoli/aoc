#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using namespace ctre::literals;

constexpr auto replace_pattern = "(?<from>\\w+) => (?<to>\\w+)"_ctre;

constexpr ctll::fixed_string from_ = "from";
constexpr ctll::fixed_string to_   = "to";

struct replacement {
  replacement(std::string_view sv)
  {
    auto m = replace_pattern.match(sv);
    from   = m.template get<from_>();
    to     = m.template get<to_>();
  }

  std::unordered_set<std::string> apply_all(std::string s) const
  {
    auto ret = std::unordered_set<std::string> {};
    auto pos = 0;

    while (true) {
      auto next = s.find(from, pos);
      if (next == std::string::npos) {
        break;
      }

      auto rs = s;
      rs.replace(next, from.size(), to);
      ret.insert(rs);

      pos = next + 1;
    }

    return ret;
  }

  std::unordered_set<std::string> rev_apply_all(std::string s) const
  {
    auto rs = *this;
    std::swap(rs.from, rs.to);
    return rs.apply_all(s);
  }

  std::string from;
  std::string to;
};

std::pair<std::vector<replacement>, std::string> get_input()
{
  auto reps      = std::vector<replacement> {};
  auto start     = std::string {};
  auto reps_done = false;

  utils::for_each_line([&](auto const& line) {
    if (reps_done) {
      start = line;
    } else if (line.empty()) {
      reps_done = true;
    } else {
      reps.emplace_back(line);
    }
  });

  return {reps, start};
}

int part_1(std::vector<replacement> const& reps, std::string const& start)
{
  auto all = std::unordered_set<std::string> {};

  for (auto const& r : reps) {
    for (auto new_str : r.apply_all(start)) {
      all.insert(new_str);
    }
  }

  return all.size();
}

int part_2(
    std::vector<replacement> reps, std::string start, std::string const& target)
{
  auto current = start;
  auto prev    = std::string {};
  auto steps   = 0;

  std::random_device rd;
  std::mt19937       g(rd());

  while (current != target) {
    if (current == prev) {
      current = start;
      steps   = 0;
    }

    prev = current;
    std::shuffle(reps.begin(), reps.end(), g);
    for (auto const& r : reps) {
      auto next = std::string::npos;
      while (true) {
        auto pos = current.rfind(r.to, next);
        if (pos == std::string::npos) {
          break;
        }
        next = pos - 1;

        current.replace(pos, r.to.size(), r.from);
        ++steps;
      }
    }
  }

  return steps;
}

int main()
{
  auto [reps, start] = get_input();
  std::cout << part_1(reps, start) << '\n';
  std::cout << part_2(reps, start, "e") << '\n';
}
