#include <cassert>
#include <iostream>
#include <string>

int fuel_req(int mass)
{
  int req = (mass / 3) - 2;
  return req > 0 ? req : 0;
}

int fixed_req(int mass)
{
  int fuel  = fuel_req(mass);
  int extra = fuel;
  while (extra != 0) {
    extra = fuel_req(extra);
    fuel += extra;
  }
  return fuel;
}

void tests()
{
  assert(fuel_req(2) == 0);
  assert(fuel_req(12) == 2);
  assert(fuel_req(14) == 2);
  assert(fuel_req(654) == 216);
  assert(fuel_req(1969) == 654);
  assert(fuel_req(100756) == 33583);

  assert(fixed_req(14) == 2);
  assert(fixed_req(1969) == 966);
  assert(fixed_req(100756) == 50346);
}

int main()
{
  tests();

  int total_fuel = 0;
  int fixed_fuel = 0;

  for (std::string line; std::getline(std::cin, line);) {
    int mass = std::stoi(line);
    total_fuel += fuel_req(mass);
    fixed_fuel += fixed_req(mass);
  }

  std::cout << total_fuel << '\n';
  std::cout << fixed_fuel << '\n';
}
