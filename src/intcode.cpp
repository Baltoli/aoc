#include "intcode.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

namespace ic {

void tests()
{
  assert(opcode(1002) == 2);
  assert(param_mode(0, 1002) == 0);
  assert(param_mode(1, 1002) == 1);
  assert(param_mode(2, 1002) == 0);

  assert(opcode(11103) == 3);
  assert(param_mode(0, 11103) == 1);
  assert(param_mode(1, 11103) == 1);
  assert(param_mode(2, 11103) == 1);

  assert(opcode(3) == 3);
  assert(param_mode(0, 3) == 0);
  assert(param_mode(1, 3) == 0);
  assert(param_mode(2, 3) == 0);
}

int opcode(int instr) { return instr % 100; }

int pc_advance(int opcode)
{
  std::unordered_map<int, int> data {{1, 4}, {2, 4}, {3, 2}, {4, 2}};
  return data[opcode];
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

void computer::run()
{
  while (true) {
    auto instr = program_[pc_];
    auto op    = opcode(instr);

    switch (op) {
    case 1: {
      auto lhs = param_mode(0, instr) ? program_[pc_ + 1]
                                      : program_[program_[pc_ + 1]];
      auto rhs = param_mode(1, instr) ? program_[pc_ + 2]
                                      : program_[program_[pc_ + 2]];
      program_[program_[pc_ + 3]] = lhs + rhs;
      break;
    }

    case 2: {
      auto lhs = param_mode(0, instr) ? program_[pc_ + 1]
                                      : program_[program_[pc_ + 1]];
      auto rhs = param_mode(1, instr) ? program_[pc_ + 2]
                                      : program_[program_[pc_ + 2]];
      program_[program_[pc_ + 3]] = lhs * rhs;
      break;
    }

    case 3: {
      auto in = inputs_.front();
      inputs_.pop();
      program_[program_[pc_ + 1]] = in;
      break;
    }

    case 4:
      output(
          param_mode(0, instr) ? program_[pc_ + 1]
                               : program_[program_[pc_ + 1]]);
      break;

    case 99:
      return;

    default:
      assert(false && "Invalid opcode");
    }

    pc_ += pc_advance(op);
  }
}

void computer::input(int in) { inputs_.push(in); }

void computer::output(int out) { std::cout << out << '\n'; }

int& computer::operator[](size_t i) { return program_[i]; }
int const& computer::operator[](size_t i) const { return program_[i]; }

}
