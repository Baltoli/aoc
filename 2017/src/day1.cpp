#include <utils/utils.h>

#include <iostream>

int compute(std::string const& in, int step)
{
  int count = 0;
  for (auto i = 0; i < in.size(); ++i) {
    auto next = (i + step) % in.size();
    if (in[i] == in[next]) {
      count += utils::ctoi(in[i]);
    }
  }
  return count;
}

int main()
{
  auto in = utils::get_lines()[0];

  std::cout << compute(in, 1) << '\n';
  std::cout << compute(in, in.size() / 2) << '\n';
}
