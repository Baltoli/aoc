#include <utils/utils.h>

#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

long get_input()
{
  auto in = std::string {};
  std::getline(std::cin, in);
  return std::stol(in);
}

long first_to_n(long target)
{
  auto presents = std::vector<long>(target, 0);
  for (auto elf = 1; elf < presents.size(); ++elf) {
    for (auto house = elf; house < presents.size(); house += elf) {
      presents.at(house) += elf * 10;
    }
  }

  for (auto house = 1; house < presents.size(); ++house) {
    if (presents.at(house) >= target) {
      return house;
    }
  }

  assert(false && "Solution not found");
}

long non_infinite(long target)
{
  auto presents = std::vector<long>(target, 0);
  for (auto elf = 1; elf < presents.size(); ++elf) {
    for (auto visit = 0; visit < 50; ++visit) {
      auto house = elf + (visit * elf);
      if (house < presents.size()) {
        presents.at(house) += elf * 11;
      }
    }
  }

  for (auto house = 1; house < presents.size(); ++house) {
    if (presents.at(house) >= target) {
      return house;
    }
  }

  assert(false && "Solution not found");
}

int main()
{
  auto in = get_input();
  std::cout << first_to_n(in) << '\n';
  std::cout << non_infinite(in) << '\n';
}
