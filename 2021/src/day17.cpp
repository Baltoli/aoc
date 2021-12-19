#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>

struct bounds {
  int x_min;
  int x_max;

  int y_min;
  int y_max;
};

std::pair<int, bool> simulate(bounds const& target, int x_vel, int y_vel)
{
  auto x = 0;
  auto y = 0;

  auto highest_y = y;

  while (true) {
    if (x >= target.x_min && x <= target.x_max && y >= target.y_min
        && y <= target.y_max) {
      return {highest_y, true};
    } else if (
        x_vel == 0
        && (x < target.x_min || x > target.x_max || y < target.y_min)) {
      return {highest_y, false};
    } else {
      x += x_vel;
      y += y_vel;

      if (x_vel > 0) {
        x_vel -= 1;
      } else if (x_vel < 0) {
        x_vel += 1;
      }

      y_vel -= 1;

      highest_y = std::max(highest_y, y);
    }
  }
}

int part_1(bounds const& target)
{
  auto max = 0;

  for (auto y_v = target.y_min; y_v < 5000; ++y_v) {
    for (auto x_v = 0; x_v < target.x_max; ++x_v) {
      auto [m_y, in] = simulate(target, x_v, y_v);
      if (m_y > max && in) {
        max = m_y;
      }
    }
  }

  return max;
}

int part_2(bounds const& target)
{
  auto total = 0;

  for (auto y_v = -500; y_v < 500; ++y_v) {
    for (auto x_v = -500; x_v < 500; ++x_v) {
      auto [m_y, in] = simulate(target, x_v, y_v);
      if (in) {
        ++total;
      }
    }
  }

  return total;
}

int main()
{
  auto input = utils::get_lines()[0];

  auto m
      = ctre::match<R"#(target area: x=(.+)\.\.(.+), y=(.+)\.\.(.+))#">(input);

  auto target = bounds {
      utils::stoi(m.get<1>().str()), utils::stoi(m.get<2>().str()),
      utils::stoi(m.get<3>().str()), utils::stoi(m.get<4>().str())};

  std::cout << part_1(target) << '\n';
  std::cout << part_2(target) << '\n';
}
