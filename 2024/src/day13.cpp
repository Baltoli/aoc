#include <utils/ctre.h>
#include <utils/grid.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>

using namespace ctre::literals;

constexpr auto a_pattern     = "Button A: X\\+(?<x>\\d+), Y\\+(?<y>\\d+)"_ctre;
constexpr auto b_pattern     = "Button B: X\\+(?<x>\\d+), Y\\+(?<y>\\d+)"_ctre;
constexpr auto prize_pattern = "Prize: X=(?<x>\\d+), Y=(?<y>\\d+)"_ctre;

static_assert(a_pattern.match("Button A: X+45, Y+76"));
static_assert(b_pattern.match("Button B: X+84, Y+14"));
static_assert(prize_pattern.match("Prize: X=9612, Y=4342"));

struct machine {
  utils::point a;
  utils::point b;
  utils::point prize;

  constexpr bool solvable() const noexcept
  {
    return prize.x % utils::gcd(a.x, b.x) == 0
           && prize.y % utils::gcd(a.y, b.y) == 0;
  }

  constexpr utils::point solve() const noexcept
  {
    auto [a_1, b_1, g] = utils::egcd(a.x, b.x);
    auto scale         = prize.x / g;
    auto coef_a        = scale * a_1;
    auto coef_b        = scale * b_1;

    auto a_k_factor = b.x / g;
    auto b_k_factor = a.x / g;

    auto [min_k, max_k] = [=] {
      return std::pair {
          std::min(-(coef_a / a_k_factor), (coef_b / b_k_factor)),
          std::max(-(coef_a - 100) / a_k_factor, (coef_b - 100) / b_k_factor)};
    }();

    min_k = std::min(min_k, max_k);
    max_k = std::max(min_k, max_k);

    constexpr auto in_range
        = [](auto presses) { return presses >= 0 && presses <= 100; };

    auto best_score = std::numeric_limits<std::int64_t>::min();
    auto best_play  = utils::point {0, 0};

    for (auto k = min_k; k <= max_k; ++k) {
      auto a_presses = coef_a + a_k_factor * k;
      auto b_presses = coef_b - b_k_factor * k;

      if (in_range(a_presses) && in_range(b_presses)) {
        assert(a_presses * a.x + b_presses * b.x == prize.x);
        if (a_presses * a.y + b_presses * b.y == prize.y) {
          auto score = a_presses * 3 + b_presses;
          if (score > best_score) {
            best_score = score;
            best_play  = {a_presses, b_presses};
          }
        }
      }
    }

    return best_play;
  }
};

constexpr machine test_case = {
    .a     = {94, 34},
    .b     = {22, 67},
    .prize = {8400, 5400},
};

static_assert(test_case.solvable());
static_assert(test_case.solve() == utils::point {80, 40});

constexpr machine fail_case = {
    .a     = {26, 66},
    .b     = {67, 21},
    .prize = {12748, 12176},
};

static_assert(!fail_case.solvable());

std::generator<machine> input()
{
  auto current = machine {};

  for (auto line : utils::lines()) {
    if (line.empty()) {
      co_yield current;
      current = {};
    } else {
      if (auto [m, x, y] = a_pattern.match(line); m) {
        current.a.x = utils::to_int<std::int64_t>(x);
        current.a.y = utils::to_int<std::int64_t>(y);
      } else if (auto [m, x, y] = b_pattern.match(line); m) {
        current.b.x = utils::to_int<std::int64_t>(x);
        current.b.y = utils::to_int<std::int64_t>(y);
      } else if (auto [m, x, y] = prize_pattern.match(line); m) {
        current.prize.x = utils::to_int<std::int64_t>(x);
        current.prize.y = utils::to_int<std::int64_t>(y);
      } else {
        assert(false);
      }
    }
  }
}

template <typename Range>
std::int64_t part_1(Range&& r)
{
  return utils::sum(std::forward<Range>(r), [](auto const& m) {
    if (m.solvable()) {
      auto [a, b] = m.solve();
      return a * 3 + b;
    }

    return std::int64_t {0};
  });
}

int main()
{
  auto in = input() | std::ranges::to<std::vector>();
  fmt::print("{}\n", part_1(in));
}
