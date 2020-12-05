#include <utils/ctre.h>
#include <utils/utils.h>

#include <algorithm>
#include <map>
#include <set>
#include <vector>

using namespace ctre::literals;

constexpr ctll::fixed_string from = "from";
constexpr ctll::fixed_string dir  = "dir";
constexpr ctll::fixed_string gain = "gain";
constexpr ctll::fixed_string to   = "to";

using adj_t = std::map<std::pair<std::string, std::string>, int>;

struct input {
  template <typename Match>
  input(Match const& m)
      : from_(m.template get<from>())
      , gain_(
            utils::svtoi(m.template get<gain>())
            * (m.template get<dir>() == "gain" ? 1 : -1))
      , to_(m.template get<to>())
  {
  }

  std::string from_;
  int         gain_;
  std::string to_;
};

int score(std::vector<std::string> const& perm, adj_t const& adj)
{
  auto s = adj.at({perm[perm.size() - 1], perm[0]})
           + adj.at({perm[0], perm[perm.size() - 1]});

  for (auto i = 0; i < perm.size() - 1; ++i) {
    s += adj.at({perm[i], perm[i + 1]});
    s += adj.at({perm[i + 1], perm[i]});
  }

  return s;
}

int main()
{
  constexpr auto line_pattern
      = "(?<from>[A-Z][a-z]+) would (?<dir>gain|lose) (?<gain>\\d+)([a-z]+| )+(?<to>[A-Z][a-z]+)."_ctre;

  auto adj   = adj_t {};
  auto names = std::set<std::string> {};

  utils::for_each_line([&](auto const& line) {
    auto in = input(line_pattern.match(line));
    adj.insert({{in.from_, in.to_}, in.gain_});
    names.insert(in.from_);
    names.insert(in.to_);
  });

  auto max_score = std::numeric_limits<int>::min();
  auto perm      = std::vector<std::string>(names.begin(), names.end());
  while (std::next_permutation(perm.begin(), perm.end())) {
    auto new_score = score(perm, adj);
    if (new_score > max_score) {
      max_score = new_score;
    }
  }

  std::cout << max_score << '\n';
}
