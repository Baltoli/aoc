#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <utility>

using namespace ctre::literals;

using adj_t = std::map<std::string, std::map<std::string, int>>;

constexpr ctll::fixed_string colour_ = "colour";
constexpr ctll::fixed_string rest_   = "rest";
constexpr ctll::fixed_string none_   = "none";
constexpr ctll::fixed_string num_    = "num";

adj_t read_adjacencies()
{
  constexpr auto line_pattern
      = "(?<colour>[a-z]+ [a-z]+) bags contain (?<rest>.*)\\."_ctre;

  constexpr auto rest_pattern
      = "((?<none>no other bags))|((?<num>\\d+) (?<colour>[a-z]+ [a-z]+) bags?(, (?<rest>.*))?)"_ctre;

  auto ret = adj_t {};

  utils::for_each_line([&](auto const& line) {
    auto m         = line_pattern.match(line);
    auto line_rest = std::string_view(m.template get<rest_>());
    auto col       = m.template get<colour_>().str();

    ret.try_emplace(col);

    auto rest = line_rest;
    while (auto rm = rest_pattern.match(rest)) {
      rest = rm.template get<rest_>();

      if (rm.template get<none_>()) {
        continue;
      }

      auto to_col = rm.template get<colour_>().str();
      auto num    = rm.template get<num_>().str();

      ret.at(col).try_emplace(to_col, utils::svtoi(num));
    }
  });

  return ret;
}

int main() { auto adj = read_adjacencies(); }
