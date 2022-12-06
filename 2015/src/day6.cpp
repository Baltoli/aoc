#include <utils/utils.h>

#include <algorithm>
#include <map>
#include <numeric>

enum class mode { on, off, toggle };

using loc = std::pair<int, int>;

loc parse_loc(std::string const& str)
{
  auto cs = utils::split(str, ",");
  return {std::stoi(cs[0]), std::stoi(cs[1])};
}

struct instruction {
  instruction(std::string const& line);

  mode mode_;
  loc  start_;
  loc  end_;
};

class display {
public:
  display(std::vector<instruction> const&);

  int lit_count() const;
  int total_brightness() const;

private:
  void execute(instruction);

  std::map<loc, bool> lights_;
  std::map<loc, int>  brightness_;
};

instruction::instruction(std::string const& line)
{
  auto cs = utils::split(line, " ");

  start_ = parse_loc(cs[cs.size() - 3]);
  end_   = parse_loc(cs[cs.size() - 1]);

  if (cs[0] == "toggle") {
    mode_ = mode::toggle;
  } else if (cs[1] == "on") {
    mode_ = mode::on;
  } else {
    mode_ = mode::off;
  }
}

display::display(std::vector<instruction> const& insts)
    : lights_ {}
{
  for (auto i : insts) {
    execute(i);
  }
}

int display::lit_count() const
{
  return std::count_if(
      lights_.begin(), lights_.end(), [](auto p) { return p.second; });
}

int display::total_brightness() const
{
  return std::accumulate(
      brightness_.begin(), brightness_.end(), 0,
      [](auto acc, auto p) { return acc + p.second; });
}

void display::execute(instruction i)
{
  auto min_x = std::min(i.start_.first, i.end_.first);
  auto max_x = std::max(i.start_.first, i.end_.first);
  auto min_y = std::min(i.start_.second, i.end_.second);
  auto max_y = std::max(i.start_.second, i.end_.second);

  for (int x = min_x; x <= max_x; ++x) {
    for (int y = min_y; y <= max_y; ++y) {
      brightness_.try_emplace({x, y}, 0);
      switch (i.mode_) {
      case mode::on:
        lights_[{x, y}] = true;
        brightness_[{x, y}]++;
        break;
      case mode::off:
        lights_[{x, y}]     = false;
        brightness_[{x, y}] = std::max(0, brightness_[{x, y}] - 1);
        break;
      case mode::toggle:
        lights_.try_emplace({x, y}, false);
        lights_[{x, y}] = !lights_[{x, y}];
        brightness_[{x, y}] += 2;
        break;
      }
    }
  }
}

int main()
{
  auto instrs = std::vector<instruction> {};
  utils::for_each_line([&](auto line) { instrs.emplace_back(line); });

  auto disp = display(instrs);
  std::cout << disp.lit_count() << '\n';
  std::cout << disp.total_brightness() << '\n';
}
