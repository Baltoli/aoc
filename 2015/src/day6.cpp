#include <utils/utils.h>

#include <algorithm>
#include <map>

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

private:
  void execute(instruction);

  std::map<loc, bool> lights_;
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
    : lights_{}
{
  int x = 0;
  for (auto i : insts) {
    std::cout << (++x) << '\n';
    execute(i);
  }
}

int display::lit_count() const
{
  return std::count_if(
      lights_.begin(), lights_.end(), [](auto p) { return p.second; });
}

void display::execute(instruction i)
{
  auto min_x = std::min(i.start_.first, i.end_.first);
  auto max_x = std::max(i.start_.first, i.end_.first);
  auto min_y = std::min(i.start_.second, i.end_.second);
  auto max_y = std::max(i.start_.second, i.end_.second);

  for (int x = min_x; x <= max_x; ++x) {
    for (int y = min_y; y <= max_y; ++y) {
      switch (i.mode_) {
      case mode::on:
        lights_[{x, y}] = true;
        break;
      case mode::off:
        lights_[{x, y}] = false;
        break;
      case mode::toggle:
        lights_.try_emplace({x, y}, false);
        lights_[{x, y}] = !lights_[{x, y}];
        break;
      }
    }
  }
}

int main()
{
  auto instrs = std::vector<instruction>{};
  utils::for_each_line([&](auto line) { instrs.emplace_back(line); });

  auto disp = display(instrs);
  std::cout << disp.lit_count() << '\n';
}
