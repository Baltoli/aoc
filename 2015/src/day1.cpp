#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>

int part_1(std::string const& steps)
{
  return std::count(steps.begin(), steps.end(), '(')
         - std::count(steps.begin(), steps.end(), ')');
}

int part_2(std::string const& steps)
{
  auto floor = 0;
  auto step  = 0;

  for (; floor != -1; ++step) {
    if (steps[step] == '(') {
      ++floor;
    }

    if (steps[step] == ')') {
      --floor;
    }
  }

  return step;
}

int main()
{
  std::string line;
  std::getline(std::cin, line);

  std::cout << part_1(line) << '\n';
  std::cout << part_2(line) << '\n';
}
