#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <list>
#include <vector>

std::list<std::uint64_t> read_input()
{
  auto vec = utils::map(
      utils::split(utils::get_single_line(), " "),
      utils::to_int<std::uint64_t>);
  return std::list(vec.begin(), vec.end());
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

void naive_step(std::list<std::uint64_t>& stones)
{
  for (auto it = stones.begin(); it != stones.end(); ++it) {
    if (*it == 0) {
      *it = 1;
    } else if (even_digits(*it)) {
      auto [left, right] = split(*it);
      stones.insert(it, left);
      auto right_it = stones.insert(it, right);
      stones.erase(it);
      it = right_it;
    } else {
      *it *= 2024;
    }
  }
}

int main()
{
  auto in = read_input();
  for (auto i = 0u; i < 25; ++i) {
    naive_step(in);
  }
  fmt::print("{}\n", in.size());
}
