#include <utils/utils.h>

#include <fmt/format.h>

#include <utils/ctre.h>

#include <ranges>
#include <unordered_map>
#include <vector>

namespace ranges = std::ranges;
namespace views  = std::ranges::views;

std::int64_t
part_1(std::vector<std::int64_t> a_vec, std::vector<std::int64_t> b_vec)
{
  ranges::sort(a_vec);
  ranges::sort(b_vec);

  auto abs_diff = [](auto t) {
    return std::apply([](auto a, auto b) { return std::abs(a - b); }, t);
  };

  auto diffs = views::zip(a_vec, b_vec) | views::transform(abs_diff);
  return ranges::fold_left(diffs, 0L, std::plus<> {});
}

std::int64_t
part_2(std::vector<std::int64_t> a_vec, std::vector<std::int64_t> b_vec)
{
  auto counts = std::unordered_map<std::int64_t, std::int64_t> {};

  for (auto b : b_vec) {
    counts.try_emplace(b, 0);
    counts[b]++;
  }

  auto scores = a_vec | views::transform([&](auto a) {
                  counts.try_emplace(a, 0);
                  return a * counts[a];
                });

  return ranges::fold_left(scores, 0L, std::plus<> {});
}

int main()
{
  auto a_vec = std::vector<std::int64_t> {};
  auto b_vec = std::vector<std::int64_t> {};

  utils::for_each_line([&](auto const& line) {
    auto [_, a, b] = ctre::match<R"((\d+)   (\d+))">(line);
    a_vec.push_back(utils::svtol(a));
    b_vec.push_back(utils::svtol(b));
  });

  fmt::print("{}\n", part_1(a_vec, b_vec));
  fmt::print("{}\n", part_2(a_vec, b_vec));
}