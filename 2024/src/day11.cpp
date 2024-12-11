#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <vector>

std::vector<std::uint64_t> read_input()
{
  return utils::map(
      utils::split(utils::get_single_line(), " "),
      utils::to_int<std::uint64_t>);
}

constexpr bool even_digits(std::uint64_t i)
{
  return utils::digits(i) % 2 == 0;
}

constexpr std::pair<std::uint64_t, std::uint64_t> split(std::uint64_t i)
{
  assert(even_digits(i) && i != 0);
  auto half_size = utils::digits(i) / 2;
  auto pow_10    = utils::pow(10, half_size);
  auto right     = i % pow_10;
  auto left      = (i - right) / pow_10;
  return {left, right};
}

static_assert(split(17) == std::pair {1, 7});
static_assert(split(510613) == std::pair {510, 613});

template <typename Arr>
void fill(Arr& table, std::uint64_t n, std::uint64_t steps)
{
  for (auto i = 0u; i <= steps; ++i) {
    if (table[i][n] != 0) {
      continue;
    }

    if (i == 0) {
      table[i][n] = 1;
    } else {
      if (n == 0) {
        fill(table, 1, steps - 1);
        table[i][n] = table[i - 1][1];
      } else if (even_digits(n)) {
        auto [left, right] = split(n);
        fill(table, left, steps - 1);
        fill(table, right, steps - 1);
        table[i][n] = table[i - 1][left] + table[i - 1][right];
      } else {
        fill(table, n * 2024, steps - 1);
        table[i][n] = table[i - 1][n * 2024];
      }
    }
  }
}

template <std::size_t max_steps>
std::uint64_t dp_step(std::vector<std::uint64_t> const& stones)
{
  // table[s][n] = size of n after s steps
  auto table = std::array<
      std::unordered_map<std::uint64_t, std::uint64_t>, max_steps + 1> {};

  for (auto s : stones) {
    fill(table, s, max_steps);
  }

  auto sum = std::uint64_t {0};
  for (auto s : stones) {
    sum += table[max_steps][s];
  }

  return sum;
}

int main()
{
  auto in = read_input();

  fmt::print("{}\n", dp_step<25>(in));
  fmt::print("{}\n", dp_step<75>(in));
}
