#include <utils/utils.h>

#include <iostream>

enum class mode { north, south, east, west, left, right, forward };

mode from_char(char c)
{
  switch (c) {
  case 'N':
    return mode::north;
  case 'S':
    return mode::south;
  case 'E':
    return mode::east;
  case 'W':
    return mode::west;
  case 'L':
    return mode::left;
  case 'R':
    return mode::right;
  case 'F':
    return mode::forward;
  default:
    __builtin_unreachable();
  }
}

struct instruction {
  instruction(std::string_view sv)
      : op(from_char(sv[0]))
      , arg(utils::svtoi(sv.substr(1)))
  {
  }

  instruction(mode m, int a)
      : op(m)
      , arg(a)
  {
  }

  mode op;
  int  arg;
};

struct ship {
  ship() = default;

  void step(instruction i)
  {
    switch (i.op) {
    case mode::north:
      y += i.arg;
      break;
    case mode::south:
      y -= i.arg;
      break;
    case mode::east:
      x += i.arg;
      break;
    case mode::west:
      x -= i.arg;
      break;
    case mode::right:
      heading += i.arg;
      break;
    case mode::left:
      heading -= i.arg;
      break;
    case mode::forward:
      auto new_i = from_heading(i.arg);
      step(new_i);
      break;
    }
  }

  instruction from_heading(int arg)
  {
    if (heading >= 360) {
      heading -= 360;
    } else if (heading < 0) {
      heading += 360;
    }

    assert(heading >= 0 && heading < 360);

    switch (heading) {
    case 0:
      return instruction(mode::north, arg);
    case 90:
      return instruction(mode::east, arg);
    case 180:
      return instruction(mode::south, arg);
    case 270:
      return instruction(mode::west, arg);
    default:
      assert(false);
    }
  }

  int manhattan() const { return std::abs(x) + std::abs(y); }

  int x       = 0;
  int y       = 0;
  int heading = 90;
};

int main()
{
  auto s = ship();
  auto instrs
      = utils::map_lines([](auto const& line) { return instruction(line); });

  for (auto i : instrs) {
    s.step(i);
  }

  std::cout << s.manhattan() << '\n';
}
