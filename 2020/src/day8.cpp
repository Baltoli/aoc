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

int main()
{
  auto instrs = utils::map_lines([](auto const& line) {
    constexpr auto pattern = "([a-z]{3}) ((\\+|-)\\d+)"_ctre;
    return instruction(pattern.match(line));
  });
}
