#include <utils/utils.h>

#include <fmt/format.h>

#include <numeric>

std::vector<int> load()
{
  return utils::map_lines([](auto const& line) { return utils::stoi(line); });
}

std::vector<std::vector<int>> compute_table(std::vector<int> const& input)
{
  return {{}};
}

int main()
{
  auto in = load();

  fmt::print(
      "{}\n", std::accumulate(in.begin(), in.end(), 0, [](auto acc, auto i) {
        return acc + i;
      }));
}
