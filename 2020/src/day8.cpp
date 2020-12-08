#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>
#include <vector>

using namespace ctre::literals;

struct instruction {
  template <typename Match>
  instruction(Match&& m)
      : opcode(m.template get<1>())
      , arg(utils::svtoi(m.template get<2>()))
  {
  }

  std::string opcode;
  int         arg;
};

struct computer {
  computer(std::vector<instruction> const& p)
      : program(p)
      , executed(p.size(), false)
      , accumulator(0)
      , pc(0)
  {
  }

  void step()
  {
    auto const& inst = program.at(pc);

    if (inst.opcode == "nop") {
      pc += 1;
      return;
    }

    if (inst.opcode == "acc") {
      accumulator += inst.arg;
      pc += 1;
      return;
    }

    if (inst.opcode == "jmp") {
      pc += inst.arg;
      return;
    }
  }

  void run_until_loop()
  {
    while (true) {
      if (executed.at(pc)) {
        return;
      }

      executed.at(pc) = true;
      step();
    }
  }

  std::vector<instruction> const& program;
  std::vector<bool>               executed;
  int                             accumulator;
  int                             pc;
};

int main()
{
  auto instrs = utils::map_lines([](auto const& line) {
    constexpr auto pattern = "([a-z]{3}) ((\\+|-)\\d+)"_ctre;
    return instruction(pattern.match(line));
  });

  auto comp = computer(instrs);
  comp.run_until_loop();
  std::cout << comp.accumulator << '\n';
}
