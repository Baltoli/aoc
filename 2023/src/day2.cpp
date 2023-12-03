#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <map>
#include <numeric>
#include <vector>

using namespace ctre::literals;

constexpr auto game_pattern  = "Game (\\d+): (.*)"_ctre;
constexpr auto color_pattern = "(\\d+) (red|green|blue)"_ctre;

constexpr long max_red   = 12;
constexpr long max_green = 13;
constexpr long max_blue  = 14;

struct hand {
  long red   = 0;
  long green = 0;
  long blue  = 0;

  static hand parse(std::string const& part)
  {
    auto ret = hand {};
    auto map = std::map<std::string, long*> {
        {"red", &ret.red}, {"green", &ret.green}, {"blue", &ret.blue}};

    auto colors = utils::split(part, ", ");
    for (auto const& c : colors) {
      auto m = color_pattern.match(c);

      *map[m.get<2>().to_string()] = utils::stol(m.get<1>().to_string());
    }

    return ret;
  }

  bool possible() const
  {
    return red <= max_red && green <= max_green && blue <= max_blue;
  }

  long power() const { return red * green * blue; }
};

struct game {
  long              id;
  std::vector<hand> rounds = {};

  static game parse(std::string const& line)
  {
    auto ret = game {};
    auto m   = game_pattern.match(line);

    ret.id     = utils::stol(m.get<1>().to_string());
    auto hands = utils::split(m.get<2>().str(), "; ");
    for (auto const& h : hands) {
      ret.rounds.push_back(hand::parse(h));
    }

    return ret;
  }

  bool possible() const
  {
    return std::all_of(rounds.begin(), rounds.end(), [](auto const& h) {
      return h.possible();
    });
  }

  hand minimum() const
  {
    auto r = std::max_element(rounds.begin(), rounds.end(), [](auto a, auto b) {
               return a.red < b.red;
             })->red;

    auto g = std::max_element(rounds.begin(), rounds.end(), [](auto a, auto b) {
               return a.green < b.green;
             })->green;

    auto b = std::max_element(rounds.begin(), rounds.end(), [](auto a, auto b) {
               return a.blue < b.blue;
             })->blue;

    return {r, g, b};
  }
};

long part_1(std::vector<game> const& input)
{
  return std::accumulate(
      input.begin(), input.end(), 0L, [](auto acc, auto const& game) {
        return acc + (game.possible() ? game.id : 0L);
      });
}

long part_2(std::vector<game> const& input)
{
  return std::accumulate(
      input.begin(), input.end(), 0L,
      [](auto acc, auto const& game) { return acc + game.minimum().power(); });
}

int main()
{
  auto in = utils::map_lines(game::parse);

  fmt::print("{}\n", part_1(in));
  fmt::print("{}\n", part_2(in));
}
