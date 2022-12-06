#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <unordered_map>
#include <vector>

enum op { half, triple, inc, jump, jump_even, jump_one };

struct instruction {
  instruction(std::string_view line)
  {
    static auto table = std::unordered_map<std::string_view, op> {
        {"hlf", half}, {"tpl", triple},    {"inc", inc},
        {"jmp", jump}, {"jie", jump_even}, {"jio", jump_one}};

    if (auto m = ctre::match<"([a-z]{3}) ([ab])">(line)) {
      opcode = table.at(m.get<1>());
      reg    = m.get<2>().to_view()[0];
    } else if (auto m = ctre::match<"jmp (.*)">(line)) {
      opcode = jump;
      offset = utils::svtoi(m.get<1>());
    } else if (auto m = ctre::match<"([a-z]{3}) ([ab]), (.*)">(line)) {
      opcode = table.at(m.get<1>());
      reg    = m.get<2>().to_view()[0];
      offset = utils::svtoi(m.get<3>());
    } else {
      assert(false && "Bad instruction");
    }
  }

  op   opcode;
  char reg;
  int  offset;
};

struct machine {
  machine(std::vector<instruction> p)
      : registers {{'a', 0}, {'b', 0}}
      , pc(0)
      , program(p)
  {
  }

  std::unordered_map<char, int> registers;

  int                      pc;
  std::vector<instruction> program;

  void step()
  {
    auto const& i = program[pc];

    switch (i.opcode) {
    case half:
      registers[i.reg] /= 2;
      ++pc;
      break;
    case triple:
      registers[i.reg] *= 3;
      ++pc;
      break;
    case inc:
      registers[i.reg] += 1;
      ++pc;
      break;
    case jump: pc += i.offset; break;
    case jump_even:
      if (registers[i.reg] % 2 == 0) {
        pc += i.offset;
      } else {
        ++pc;
      }
      break;
    case jump_one:
      if (registers[i.reg] == 1) {
        pc += i.offset;
      } else {
        ++pc;
      }
      break;
    }
  }

  void run()
  {
    while (pc >= 0 && pc < program.size()) {
      step();
    }
  }
};

std::vector<instruction> load()
{
  return utils::construct_lines<instruction>();
}

int main()
{
  auto in = load();
  auto m  = machine(in);
  auto m2 = machine(in);

  m.run();
  fmt::print("{}\n", m.registers['b']);

  m2.registers['a'] = 1;
  m2.run();
  fmt::print("{}\n", m2.registers['b']);
}
