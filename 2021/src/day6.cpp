#include <utils/utils.h>

#include <iostream>
#include <numeric>
#include <unordered_map>

long sim(std::vector<long> const& input, long days)
{
  auto state = std::unordered_map<long, long> {};
  for (auto i = 0; i <= 8; ++i) {
    state[i] = 0;
  }

  for (auto i : input) {
    state[i]++;
  }

  for (auto d = 0; d < days; ++d) {
    auto zeros = state[0];
    for (auto i = 1; i <= 8; ++i) {
      state[i - 1] = state[i];
    }

    state[6] += zeros;
    state[8] = zeros;
  }

  return std::accumulate(
      state.begin(), state.end(), 0L,
      [](auto acc, auto const& entry) { return acc + entry.second; });
}

int main(void)
{
  auto data  = utils::get_lines()[0];
  auto state = std::vector<long> {};
  for (auto s : utils::split(data, ",")) {
    state.push_back(utils::stoi(s));
  }

  std::cout << sim(state, 80) << '\n';
  std::cout << sim(state, 256) << '\n';

  return 0;
}
