#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <unordered_map>

enum op { inp, add, mul, div_, mod, eql };

struct instruction {
  op          opcode;
  std::string ra;
  std::string rb;

  instruction(std::string_view in)
  {
    static auto table = std::unordered_map<std::string, op> {
        {"inp", op::inp},  {"add", op::add}, {"mul", op::mul},
        {"div", op::div_}, {"mod", op::mod}, {"eql", op::eql}};

    auto m = ctre::match<R"((...) ([xyzw]) (.*)?)">(in);

    opcode = table.at(m.get<1>().str());
    ra     = m.get<2>().str();
    rb     = m.get<3>().str();

    if (rb.empty()) {
      rb = "0";
    }
  }
};

struct device {
  device(std::string in)
      : input(in)
      , next_in(0)
      , registers {{'x', 0}, {'y', 0}, {'z', 0}, {'w', 0}}
  {
  }

  std::string                   input;
  int                           next_in;
  std::unordered_map<char, int> registers;

  void dispatch(instruction const& inst)
  {
    int& target = registers.at(inst.ra[0]);
    int  rhs    = std::isalpha(inst.rb[0]) ? registers.at(inst.rb[0])
                                           : utils::stoi(inst.rb);

    switch (inst.opcode) {
    case op::inp:
      target = utils::ctoi(input[next_in++]);
      break;
    case op::add:
      target = target + rhs;
      break;
    case op::mul:
      target = target * rhs;
      break;
    case op::div_:
      target = target / rhs;
      break;
    case op::mod:
      target = target % rhs;
      break;
    case op::eql:
      target = target == rhs ? 1 : 0;
      break;
    }
  }
};

std::vector<instruction> load()
{
  return utils::construct_lines<instruction>();
}

int main()
{
  for (auto mn : {"74929995999389", "11118151637112"}) {
    auto pc      = device(mn);
    auto program = load();

    for (auto const& i : program) {
      pc.dispatch(i);
    }

    fmt::print("{}\n", pc.registers['z']);
  }
}
