#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <set>

using namespace ctre::literals;

constexpr auto line_pattern = "Card\\s+(\\d+): (.*) \\| (.*)"_ctre;

struct card {
  std::set<long> win;
  std::set<long> actual;

  card(std::string const& line)
      : win {}
      , actual {}
  {
    auto [x, id, win_str, actual_str] = line_pattern.match(line);

    for (auto const& w : utils::split(win_str.str(), " ")) {
      win.insert(utils::stol(w));
    }

    for (auto const& a : utils::split(actual_str.str(), " ")) {
      actual.insert(utils::stol(a));
    }
  }

  long score() const
  {
    auto inter = utils::intersect(win, actual);
    if (inter.empty()) {
      return 0;
    } else {
      return utils::pow(2, inter.size() - 1);
    }
  }
};

long part_1(std::vector<card> const& cards)
{
  return utils::sum(cards.begin(), cards.end(), [](auto const& card) {
    return card.score();
  });
}

int main()
{
  auto in = utils::construct_lines<card>();

  fmt::print("{}\n", part_1(in));
}
