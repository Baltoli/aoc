#include <intcode.h>

#include <cassert>
#include <iostream>
#include <string>

class robot {
public:
  robot(std::string const&);

  void load();
  int  alignment();

  bool platform(char c) const;
  char at(int x, int y);

private:
  int width_  = 0;
  int height_ = 1;

  std::vector<char> map_ = {};
  ic::computer      computer_;
};

robot::robot(std::string const& prog)
    : computer_(prog)
{
}

bool robot::platform(char c) const
{
  return c == '#' || c == '^' || c == 'v' || c == '<' || c == '>';
}

char robot::at(int x, int y)
{
  assert(height_ > 0 && width_ > 0 && "Must load before accessing");
  return map_[x + (y * width_)];
}

void robot::load()
{
  auto w = 0;
  while (!computer_.halted()) {
    auto c = static_cast<char>(computer_.run());

    if (c != '\n') {
      w++;
      map_.push_back(c);
    } else {
      height_++;
      if (width_ == 0) {
        width_ = w;
      }
    }
  }
}

int robot::alignment()
{
  load();

  int align = 0;

  for (int y = 1; y < height_ - 1; ++y) {
    for (int x = 1; x < width_ - 1; ++x) {
      if (platform(at(x, y)) && platform(at(x - 1, y)) && platform(at(x + 1, y))
          && platform(at(x, y - 1)) && platform(at(x, y + 1))) {
        align += x * y;
      }
    }
  }

  return align;
}

int main()
{
  std::string line;
  std::getline(std::cin, line);

  auto rob = robot(line);

  std::cout << rob.alignment() << '\n';
}
