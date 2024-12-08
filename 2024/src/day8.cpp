#include <utils/grid.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <unordered_map>
#include <unordered_set>

class antenna_map : public utils::grid<> {
public:
  template <typename Range>
  antenna_map(Range&& lines)
      : utils::grid<>(std::forward<Range>(lines))
  {
    for (auto p : coords()) {
      auto freq = at(p);
      if (freq != '.') {
        antennas_.try_emplace(freq);
        antennas_.at(freq).insert(p);
      }
    }
  }

  std::generator<utils::point> antinodes(utils::point a, utils::point b) const
  {
    if (a == b) {
      co_return;
    }

    auto delta = b - a;

    for (auto an : {a - delta, b + delta}) {
      if (in_bounds(an)) {
        co_yield an;
      }
    }
  }

  std::generator<utils::point>
  all_antinodes(utils::point a, utils::point b) const
  {
    if (a == b) {
      co_return;
    }

    auto delta = b - a;

    for (auto a_p = a; in_bounds(a_p); a_p -= delta) {
      co_yield a_p;
    }

    for (auto b_p = a; in_bounds(b_p); b_p += delta) {
      co_yield b_p;
    }
  }

  std::generator<utils::point> antinodes(char freq, bool all) const
  {
    auto const& freq_antennas = antennas_.at(freq);

    for (auto x : freq_antennas) {
      for (auto y : freq_antennas) {
        for (auto an : all ? all_antinodes(x, y) : antinodes(x, y)) {
          co_yield an;
        }
      }
    }
  }

  std::unordered_set<utils::point> antinodes(bool all) const
  {
    auto result = std::unordered_set<utils::point> {};

    for (auto const& [freq, _] : antennas_) {
      for (auto an : antinodes(freq, all)) {
        result.insert(an);
      }
    }

    return result;
  }

private:
  std::unordered_map<char, std::unordered_set<utils::point>> antennas_;
};

int main()
{
  auto in = antenna_map(utils::lines());
  fmt::print("{}\n", in.antinodes(false).size());
  fmt::print("{}\n", in.antinodes(true).size());
}
