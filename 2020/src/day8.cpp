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
  computer(std::vector<instruction>& p)
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

  bool run()
  {
    while (true) {
      if (executed.at(pc)) {
        return false;
      }

      executed.at(pc) = true;
      step();

      if (pc == program.size()) {
        return true;
      }
    }
  }

  bool toggle(int i)
  {
    auto& inst = program.at(i);

    if (inst.opcode == "nop") {
      inst.opcode = "jmp";
      return true;
    }

    if (inst.opcode == "jmp") {
      inst.opcode = "nop";
      return true;
    }

    return false;
  }

  void run_until_repaired()
  {
    for (auto i = 0; i < program.size(); ++i) {
      reset();

      if (toggle(i)) {
        if (run()) {
          return;
        }

        toggle(i);
      }
    }
  }

  void reset()
  {
    pc          = 0;
    accumulator = 0;
    std::fill(executed.begin(), executed.end(), false);
  }

  std::vector<instruction>& program;
  std::vector<bool>         executed;
  int                       accumulator;
  int                       pc;
};

int main()
{
  auto instrs = utils::map_lines([](auto const& line) {
    constexpr auto pattern = "([a-z]{3}) ((\\+|-)\\d+)"_ctre;
    return instruction(pattern.match(line));
  });

  auto comp = computer(instrs);
  assert(comp.run() == false);
  std::cout << comp.accumulator << '\n';

  comp.run_until_repaired();
  std::cout << comp.accumulator << '\n';
}
