#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> read_input()
{
  std::string line;
  std::getline(std::cin, line);

  auto ret = std::vector<int> {};

  std::transform(line.begin(), line.end(), std::back_inserter(ret), [](auto c) {
    return c - '0';
  });

  return ret;
}

int main()
{
  auto in = read_input();
  ;
}
