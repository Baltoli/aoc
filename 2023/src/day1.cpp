#include <utils/utils.h>

#include <fmt/format.h>

#include <algorithm>
#include <cctype>
#include <limits>
#include <map>
#include <numeric>

static bool is_digit(char c) { return std::isdigit(c); }

long part_1(std::vector<std::string> const& lines)
{
  return std::accumulate(
      lines.begin(), lines.end(), 0L, [](auto acc, auto const& line) {
        auto first = std::find_if(line.begin(), line.end(), is_digit);
        auto last  = std::find_if(line.rbegin(), line.rend(), is_digit);
        return acc + utils::stol(fmt::format("{}{}", *first, *last));
      });
}

long part_2(std::vector<std::string> const& lines)
{
  auto values = std::map<std::string, long> {
      {"1", 1},     {"2", 2},     {"3", 3},    {"4", 4},    {"5", 5},
      {"6", 6},     {"7", 7},     {"8", 8},    {"9", 9},    {"one", 1},
      {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6},
      {"seven", 7}, {"eight", 8}, {"nine", 9}};

  return std::accumulate(
      lines.begin(), lines.end(), 0L, [&](auto acc, auto const& line) {
        auto best_first     = std::string::npos;
        auto best_first_val = 0L;

        auto best_last     = 0;
        auto best_last_val = 0L;

        for (auto const& [needle, value] : values) {
          auto first = line.find(needle);
          if (first < best_first) {
            best_first     = first;
            best_first_val = value;
          }

          auto last = line.rfind(needle);
          if (last != std::string::npos && last >= best_last) {
            best_last     = last;
            best_last_val = value;
          }
        }

        return acc
               + utils::stol(
                   fmt::format("{}{}", best_first_val, best_last_val));
      });
}

int main()
{
  auto in = utils::get_lines();
  fmt::print("{}\n", part_1(in));
  fmt::print("{}\n", part_2(in));
}
