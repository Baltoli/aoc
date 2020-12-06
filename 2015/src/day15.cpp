#include <utils/ctre.h>
#include <utils/utils.h>

#include <algorithm>
#include <map>
#include <numeric>
#include <string>
#include <vector>

struct ingredient {
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

  std::map<std::string, ingredient> coefs;
  std::map<std::string, int>        spoons;
};

int main()
{
  auto in = utils::get_lines();
  auto r  = recipe(in);
}
