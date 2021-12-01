#include <utils/utils.h>

#include <cassert>
#include <iostream>

int solution(int window_size, std::vector<int> const& in)
{
  auto bigger = 0;

  auto window_sum = [&](auto i) {
    assert(i <= in.size() - window_size);
    auto sum = 0;
    for (auto j = i; j < i + window_size; ++j) {
      sum += in[j];
    }
    return sum;
  };

  for (auto i = 1; i <= in.size() - window_size; ++i) {
    if (window_sum(i) > window_sum(i - 1)) {
      ++bigger;
    }
  }

  return bigger;
}

int main(void)
{
  auto input = utils::map_lines([](auto& s) { return std::atoi(s.c_str()); });

  std::cout << solution(1, input) << '\n';
  std::cout << solution(3, input) << '\n';
}
