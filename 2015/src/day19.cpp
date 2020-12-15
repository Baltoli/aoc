#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>
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

  std::unordered_set<std::string> apply_all(std::string s)
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

  for (auto r : reps) {
    for (auto new_str : r.apply_all(start)) {
      all.insert(new_str);
    }
  }

  return all.size();
}

int main()
{
  auto [reps, start] = get_input();
  std::cout << part_1(reps, start) << '\n';
}
