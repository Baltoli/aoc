#include <utils/ctre.h>
#include <utils/utils.h>

#include <cassert>
#include <iostream>

using namespace ctre::literals;

struct instruction {
  std::string dir;
  long        dist;
};

long part_1(std::vector<instruction> const& in)
{
  long x     = 0;
  long depth = 0;

  for (auto const& i : in) {
    if (i.dir == "forward") {
      x += i.dist;
    } else if (i.dir == "down") {
      depth += i.dist;
    } else if (i.dir == "up") {
      depth -= i.dist;
    } else {
      assert(false && "Bad direction");
    }
  }

  return x * depth;
}

long part_2(std::vector<instruction> const& in)
{
  long x     = 0;
  long aim   = 0;
  long depth = 0;

  for (auto const& i : in) {
    if (i.dir == "forward") {
      x += i.dist;
      depth += aim * i.dist;
    } else if (i.dir == "down") {
      aim += i.dist;
    } else if (i.dir == "up") {
      aim -= i.dist;
    } else {
      assert(false && "Bad direction");
    }
  }

  return x * depth;
}

int main()
{
  constexpr auto pattern = R"(([a-z]+) (\d+))"_ctre;

  auto input = utils::map_lines([](auto const& line) {
    auto m = pattern.match(line);
    return instruction {
        std::string(m.template get<1>()), utils::svtol(m.template get<2>())};
  });

  std::cout << part_1(input) << '\n';
  std::cout << part_2(input) << '\n';
}
