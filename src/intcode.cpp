#include "intcode.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

namespace ic {

void tests()
{
  assert(param_mode(0, 1002) == 0);
  assert(param_mode(1, 1002) == 1);
  assert(param_mode(2, 1002) == 0);

  assert(param_mode(0, 11103) == 1);
  assert(param_mode(1, 11103) == 1);
  assert(param_mode(2, 11103) == 1);

  assert(param_mode(0, 3) == 0);
  assert(param_mode(1, 3) == 0);
  assert(param_mode(2, 3) == 0);
}

int param_mode(int param, int instr)
{
  assert(param >= 0 && "Invalid: param < 0");
  assert(param < 3 && "Invalid: param > 2");

  int div = std::pow(10, param + 2);
  return (instr / div) % 10;
}

computer::computer(std::string const& code)
    : program_ {}
{
  auto ptr = 0;

  while (true) {
    auto next_comma = code.find(",", ptr);
    auto str        = code.substr(ptr, next_comma - ptr);

    program_.push_back(std::stoi(str));

    if (next_comma == std::string::npos) {
      break;
    }

    ptr = next_comma + 1;
  }
}

void computer::input(int in) { inputs_.push(in); }

void computer::output(int out) {}

int& computer::operator[](size_t i) { return program_[i]; }
int const& computer::operator[](size_t i) const { return program_[i]; }

}
