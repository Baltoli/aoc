#include <cassert>
#include <iostream>
#include <string>

int get_fuel_req(int mass) { return (mass / 3) - 2; }

int main() {
  assert(get_fuel_req(12) == 2);
  assert(get_fuel_req(14) == 2);
  assert(get_fuel_req(1969) == 654);
  assert(get_fuel_req(100756) == 33583);

  int total_fuel = 0;

  for (std::string line; std::getline(std::cin, line);) {
    int mass = std::stoi(line);
    total_fuel += get_fuel_req(mass);
  }

  std::cout << total_fuel << '\n';
}
