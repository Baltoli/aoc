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

  constexpr utils::point solve() const noexcept
  {
    auto eq_x     = std::array {b.x * a.y, prize.x * a.y};
    auto eq_y     = std::array {b.y * a.x, prize.y * a.x};
    auto b_factor = eq_y[0] - eq_x[0];
    auto cst      = eq_y[1] - eq_x[1];

    if (cst % b_factor != 0) {
      return {};
    }

    auto b_press     = cst / b_factor;
    auto remaining_a = prize.x - b_press * b.x;
    if (remaining_a % a.x != 0) {
      return {};
    }

    auto a_press = remaining_a / a.x;
    if (a_press < 0 || b_press < 0) {
      return {};
    }

    return {a_press, b_press};
  }
};

constexpr machine test_case = {
    .a     = {94, 34},
    .b     = {22, 67},
    .prize = {8400, 5400},
};

static_assert(test_case.solve() == utils::point {80, 40});

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
std::int64_t run(Range&& r, std::int64_t offset = 0)
{
  return utils::sum(std::forward<Range>(r), [offset](auto m) {
    m.prize.x += offset;
    m.prize.y += offset;

    auto [a, b] = m.solve();
    return a * 3 + b;
  });
}

int main()
{
  auto in = input() | std::ranges::to<std::vector>();
  fmt::print("{}\n", run(in));
  fmt::print("{}\n", run(in, 10000000000000));
}
