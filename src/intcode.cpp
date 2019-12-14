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

  assert(opcode(2105) == 5);
  assert(param_mode(0, 2105) == 1);
  assert(param_mode(1, 2105) == 2);
  assert(param_mode(2, 2105) == 0);
}

int opcode(int instr) { return instr % 100; }

int pc_advance(int opcode)
{
  std::unordered_map<int, int> data {{1, 4}, {2, 4}, {3, 2}, {4, 2}, {5, 3},
                                     {6, 3}, {7, 4}, {8, 4}, {9, 2}};
  return data[opcode];
}

int param_mode(int param, int instr)
{
  assert(param >= 0 && "Invalid: param < 0");
  assert(param < 4 && "Invalid: param > 3");

  int div = std::pow(10, param + 2);
  return (instr / div) % 10;
}

computer::computer(std::string const& code)
    : computer(
        code, [](auto i) { std::cout << i << '\n'; }, false)
{
}

long& computer::current_param(int idx)
{
  long instr = program_[pc_];
  auto mode  = param_mode(idx, instr);

  switch (mode) {
  case 0:
    return program_[program_[pc_ + idx + 1]];
  case 1:
    return program_[pc_ + idx + 1];
  case 2:
    return program_[program_[pc_ + idx + 1] + rel_base_];
  default:
    assert(false && "Invalid mode");
  }
}

long computer::run()
{
  while (true) {
    auto instr = program_[pc_];
    auto op    = opcode(instr);

    switch (op) {
    case 1: {
      current_param(2) = current_param(0) + current_param(1);
      break;
    }

    case 2: {
      current_param(2) = current_param(0) * current_param(1);
      break;
    }

    case 3: {
      if (inputs_.empty()) {
        throw "Not enough inputs";
      }

      current_param(0) = inputs_.front();
      inputs_.pop();
      break;
    }

    case 4: {
      auto out = current_param(0);
      output(out);
      pc_ += pc_advance(op);
      return out;
    }

    case 5: {
      if (current_param(0) != 0) {
        pc_ = current_param(1);
        continue;
      }
      break;
    }

    case 6: {
      if (current_param(0) == 0) {
        pc_ = current_param(1);
        continue;
      }
      break;
    }

    case 7: {
      if (current_param(0) < current_param(1)) {
        current_param(2) = 1;
      } else {
        current_param(2) = 0;
      }
      break;
    }

    case 8: {
      if (current_param(0) == current_param(1)) {
        current_param(2) = 1;
      } else {
        current_param(2) = 0;
      }
      break;
    }

    case 9: {
      rel_base_ += current_param(0);
      break;
    }

    case 99:
      halted_ = true;
      return 43; // Not a real output

    default:
      assert(false && "Invalid opcode");
    }

    pc_ += pc_advance(op);
  }
}

void computer::input(long in) { inputs_.push(in); }

void computer::output(long out) { output_(out); }

long& computer::operator[](size_t i) { return program_[i]; }
long const& computer::operator[](size_t i) const { return program_[i]; }

}
