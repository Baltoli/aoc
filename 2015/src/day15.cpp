#include <utils/ctre.h>
#include <utils/utils.h>

#include <algorithm>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

struct ingredient {
  template <typename Match>
  ingredient(Match const& m)
  {
  }

  int capacity;
  int durability;
  int flavour;
  int texture;
  int calories;
};

struct recipe {
  recipe(std::vector<std::string> const& in) { }

  int total_spoons() const
  {
    return std::accumulate(
        spoons.begin(), spoons.end(), 0,
        [](auto const& acc, auto const& p) { return acc + p.second; });
  }

  int score() const
  {
    constexpr auto clamp = [](auto i) { return i > 0 ? i : 0; };

    auto cap = std::accumulate(
        spoons.begin(), spoons.end(), 0,
        [this](auto const& acc, auto const& p) {
          return acc + (coefs.at(p.first).capacity * p.second);
        });

    auto dur = std::accumulate(
        spoons.begin(), spoons.end(), 0,
        [this](auto const& acc, auto const& p) {
          return acc + (coefs.at(p.first).durability * p.second);
        });

    auto fla = std::accumulate(
        spoons.begin(), spoons.end(), 0,
        [this](auto const& acc, auto const& p) {
          return acc + (coefs.at(p.first).flavour * p.second);
        });

    auto tex = std::accumulate(
        spoons.begin(), spoons.end(), 0,
        [this](auto const& acc, auto const& p) {
          return acc + (coefs.at(p.first).texture * p.second);
        });

    return clamp(cap) * clamp(dur) * clamp(fla) * clamp(tex);
  }

  std::set<std::string>             names;
  std::map<std::string, ingredient> coefs;
  std::map<std::string, int>        spoons;
};

int main()
{
  auto in = utils::get_lines();
  auto r  = recipe(in);
}
