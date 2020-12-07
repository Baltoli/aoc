#include <utils/ctre.h>
#include <utils/utils.h>

#include <algorithm>
#include <cassert>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

using namespace ctre::literals;

std::vector<int> coefs(std::string_view line)
{
  constexpr auto pattern
      = "\\w+: \\w+ (-?\\d+), \\w+ (-?\\d+), \\w+ (-?\\d+), \\w+ (-?\\d+), \\w+ (\\d+)"_ctre;
  auto m = pattern.match(line);

  return {
      utils::svtoi(m.template get<1>()), utils::svtoi(m.template get<2>()),
      utils::svtoi(m.template get<3>()), utils::svtoi(m.template get<4>()),
      utils::svtoi(m.template get<5>())};
}

int score(
    std::vector<int> const& coefs, std::vector<std::vector<int>> const& ingreds,
    bool cals)
{
  auto score = 1;

  if (cals) {
    auto sum = 0;

    for (auto i = 0; i < 4; ++i) {
      sum += coefs[i] * ingreds[i][4];
    }

    if (sum != 500) {
      return 0;
    }
  }

  for (auto comp = 0; comp < 4; ++comp) {
    auto comp_score = 0;
    for (auto ingred = 0; ingred < 4; ++ingred) {
      comp_score += coefs[ingred] * ingreds[ingred][comp];
    }
    score *= (comp_score > 0 ? comp_score : 0);
  }

  return score;
}

std::vector<int>
best_scoring(std::vector<std::vector<int>> const& ingreds, bool cals)
{
  auto best      = std::vector<int> {};
  auto max_score = std::numeric_limits<int>::min();

  for (auto m1 = 0; m1 < 100; ++m1) {
    for (auto m2 = 0; m2 < m1; ++m2) {
      for (auto m3 = 0; m3 < m2; ++m3) {
        auto c1 = m3;
        auto c2 = m2 - m3;
        auto c3 = m1 - m2;
        auto c4 = 100 - m1;

        auto test = score({c1, c2, c3, c4}, ingreds, cals);
        if (test > max_score) {
          best      = {c1, c2, c3, c4};
          max_score = test;
        }
      }
    }
  }

  assert(best.size() == 4);
  return best;
}

int main()
{
  auto ingreds = std::vector<std::vector<int>> {};

  utils::for_each_line(
      [&](auto const& line) { ingreds.push_back(coefs(line)); });

  assert(ingreds.size() == 4);

  auto best = best_scoring(ingreds, false);
  std::cout << score(best, ingreds, false) << '\n';

  best = best_scoring(ingreds, true);
  std::cout << score(best, ingreds, true) << '\n';
}
