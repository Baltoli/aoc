#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace ctre::literals;

struct recipe {
  recipe(std::string_view line)
  {
    constexpr auto pattern = "(.*) \\(contains (.*)\\)"_ctre;

    auto m      = pattern.match(line);
    ingredients = utils::split(m.template get<1>().str(), " ");
    allergens   = utils::split(m.template get<2>().str(), ", ");
  }

  std::vector<std::string> ingredients;
  std::vector<std::string> allergens;
};

std::vector<recipe> get_input()
{
  return utils::map_lines([](auto const& line) { return recipe(line); });
}

std::unordered_map<std::string, std::unordered_set<std::string>>
candidates(std::vector<recipe> const& rs)
{
  auto ret
      = std::unordered_map<std::string, std::unordered_set<std::string>> {};

  for (auto const& recipe : rs) {
    for (auto const& all : recipe.allergens) {
      if (ret.find(all) == ret.end()) {
        ret[all] = {};
        for (auto const& poss : recipe.ingredients) {
          ret.at(all).insert(poss);
        }
      } else {
        auto to_erase = std::unordered_set<std::string> {};
        for (auto const& poss : ret.at(all)) {
          if (std::find(
                  recipe.ingredients.begin(), recipe.ingredients.end(), poss)
              == recipe.ingredients.end()) {
            to_erase.insert(poss);
          }
        }

        for (auto const& ingred : to_erase) {
          ret.at(all).erase(ingred);
        }
      }
    }
  }

  return ret;
}

std::unordered_map<std::string, std::string>
resolve(std::unordered_map<std::string, std::unordered_set<std::string>> cands)
{
  auto is_done = [&] {
    return std::all_of(cands.begin(), cands.end(), [](auto const& p) {
      return p.second.size() == 1;
    });
  };

  auto done = std::unordered_set<std::string> {};
  while (!is_done()) {
    auto to_erase = std::string {};

    for (auto const& [k, vs] : cands) {
      if (vs.size() == 1 && done.find(k) == done.end()) {
        to_erase = *vs.begin();
        done.insert(k);
        break;
      }
    }

    for (auto& [k, vs] : cands) {
      if (vs.size() != 1) {
        vs.erase(to_erase);
      }
    }
  }

  auto ret = std::unordered_map<std::string, std::string> {};
  for (auto const& [k, vs] : cands) {
    ret[k] = *vs.begin();
  }
  return ret;
}

std::unordered_set<std::string>
allergens(std::unordered_map<std::string, std::string> const& map)
{
  auto ret = std::unordered_set<std::string> {};
  for (auto const& [k, v] : map) {
    ret.insert(v);
  }
  return ret;
}

std::vector<std::string>
sorted(std::unordered_map<std::string, std::string> const& map)
{
  auto rev_map = std::unordered_map<std::string, std::string> {};
  auto ret     = std::vector<std::string> {};

  for (auto const& [k, v] : map) {
    rev_map[v] = k;
    ret.push_back(v);
  }

  std::sort(ret.begin(), ret.end(), [&](auto const& a, auto const& b) {
    return rev_map.at(a) < rev_map.at(b);
  });

  return ret;
}

int main()
{
  auto in    = get_input();
  auto cands = candidates(in);
  auto map   = resolve(cands);
  auto alls  = allergens(map);

  auto sum = 0;
  for (auto const& r : in) {
    for (auto const& ingred : r.ingredients) {
      sum += (alls.find(ingred) == alls.end());
    }
  }
  std::cout << sum << '\n';

  auto comma = "";
  for (auto const& str : sorted(map)) {
    std::cout << comma << str;
    comma = ",";
  }
  std::cout << '\n';
}
