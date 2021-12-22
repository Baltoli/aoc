#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <iostream>

struct span {
  long min;
  long max;
};

struct cube {
  bool on;
  span x;
  span y;
  span z;

  cube(std::string_view in)
  {
    auto m = ctre::match<
        R"((on|off) x=(-?\d+)\.\.(-?\d+),y=(-?\d+)\.\.(-?\d+),z=(-?\d+)\.\.(-?\d+))">(
        in);

    on = m.get<1>() == "on";
    x  = {utils::svtol(m.get<2>()), utils::svtol(m.get<3>())};
    y  = {utils::svtol(m.get<4>()), utils::svtol(m.get<5>())};
    z  = {utils::svtol(m.get<6>()), utils::svtol(m.get<7>())};
  }
};

std::vector<cube> load()
{
  return utils::map_lines([](auto const& l) { return cube(l); });
}

int main() { }
