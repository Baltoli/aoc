#include <utils/ctre.h>
#include <utils/utils.h>

#include <map>

using namespace ctre::literals;

using facts_t = std::map<std::string, int>;

template <typename Match>
facts_t facts(Match&& m)
{
  return facts_t {
      {std::string(m.template get<2>()), utils::svtoi(m.template get<3>())},
      {std::string(m.template get<4>()), utils::svtoi(m.template get<5>())},
      {std::string(m.template get<6>()), utils::svtoi(m.template get<7>())}};
}

facts_t base_facts()
{
  return facts_t {{"children", 3},    {"cats", 7},   {"samoyeds", 2},
                  {"pomeranians", 3}, {"akitas", 0}, {"vizslas", 0},
                  {"goldfish", 5},    {"trees", 3},  {"cars", 2},
                  {"perfumes", 1}};
}

bool consistent(facts_t const& sub, facts_t const& base)
{
  for (auto&& [item, quant] : sub) {
    if (base.at(item) != quant) {
      return false;
    }
  }

  return true;
}

bool consistent_v2(facts_t const& sub, facts_t const& base)
{
  for (auto&& [item, quant] : sub) {
    if (item == "cats" || item == "trees") {
      if (base.at(item) >= quant) {
        return false;
      }
    } else if (item == "pomeranians" || item == "goldfish") {
      if (base.at(item) <= quant) {
        return false;
      }
    } else {
      if (base.at(item) != quant) {
        return false;
      }
    }
  }

  return true;
}

int main()
{
  constexpr auto pattern
      = "Sue (\\d+): (\\w+): (\\d+), (\\w+): (\\d+), (\\w+): (\\d+)"_ctre;

  auto base = base_facts();

  int v1, v2;

  utils::for_each_line([&](auto const& line) {
    auto m = pattern.match(line);
    auto f = facts(m);

    if (consistent(f, base)) {
      v1 = utils::svtoi(m.template get<1>());
    }

    if (consistent_v2(f, base)) {
      v2 = utils::svtoi(m.template get<1>());
    }
  });

  std::cout << v1 << '\n' << v2 << '\n';
}
