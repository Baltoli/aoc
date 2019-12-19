#include <intcode.h>

#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>

class robot {
public:
  robot(std::string const&);

  void load();
  int  alignment();

  std::vector<char> explore() const;

  bool platform(char c) const;
  bool position(char c) const;
  bool intersection(int, int) const;

  char at(int x, int y) const;

  int platforms() const;

  void dump() const;

private:
  int width_  = 0;
  int height_ = 0;

  std::vector<char> map_ = {};
  ic::computer      computer_;
};

robot::robot(std::string const& prog)
    : computer_(prog)
{
}

void robot::dump() const
{
  for (int y = 0; y < height_; ++y) {
    std::cout << std::setw(2) << y << ' ';
    for (int x = 0; x < width_; ++x) {
      std::cout << at(x, y);
    }
    std::cout << '\n';
  }
}

int robot::platforms() const
{
  return std::count_if(
      map_.begin(), map_.end(), [&](auto c) { return platform(c); });
}

bool robot::platform(char c) const { return c == '#' || position(c); }

bool robot::position(char c) const
{
  return c == '^' || c == 'v' || c == '<' || c == '>';
}

bool robot::intersection(int x, int y) const
{
  return platform(at(x, y)) && platform(at(x + 1, y)) && platform(at(x - 1, y))
         && platform(at(x, y + 1)) && platform(at(x, y - 1));
}

char robot::at(int x, int y) const
{
  assert(height_ > 0 && width_ > 0 && "Must load before accessing");
  return map_[x + (y * width_)];
}

void robot::load()
{
  auto w = 0;
  while (true) {
    auto c = static_cast<char>(computer_.run());
    if (computer_.halted()) {
      break;
    }

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

// Dirs   1 = Up, 2 = Right, 3 = Down, 4 = Left
// Turns  1 = Right, -1 = Left
std::vector<char> robot::explore() const
{
  auto ret = std::vector<char> {};

  auto pos = std::pair {0, 0};
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      if (position(at(x, y))) {
        pos = {x, y};
      }
    }
  }

  auto visited_set = std::set<std::pair<int, int>> {};
  auto visited     = [&](auto x, auto y) {
    return visited_set.find({x, y}) != visited_set.end();
  };

  auto dir  = 1;
  auto turn = [&](auto t) {
    if (t == 1) {
      ret.push_back('R');
    } else if (t == -1) {
      ret.push_back('L');
    } else {
      assert(false && "Bad turn");
    }
    dir = (dir % 4) + 1;
  };

  auto in_front = [&] {
    auto [x, y] = pos;
    switch (dir) {
    case 1:
      return std::pair {x, y - 1};
    case 2:
      return std::pair {x + 1, y};
    case 3:
      return std::pair {x, y + 1};
    case 4:
      return std::pair {x - 1, y};
    default:
      assert(false);
    }
  };

  while (visited_set.size() < platforms()) {
    visited_set.insert(pos);

    for (int i = 0; i < 4; ++i) {
      auto [f_x, f_y] = in_front();

      if (platform(at(f_x, f_y))
          && (!visited(f_x, f_y) || intersection(f_x, f_y))) {
        ret.push_back('1');
        pos = in_front();
        break;
      } else {
        turn(1);
      }
    }
  }

  return ret;
}

std::vector<int> opt_moves(std::vector<char> const& in)
{
  auto ret = std::vector<int> {};

  auto ones = 0;
  for (auto c : in) {
    if (c == '1') {
      ++ones;
    } else {
      if (ones > 0) {
        ret.push_back(ones);
        ones = 0;
      }
      ret.push_back(c);
    }
  }

  return ret;
}

std::vector<char> opt_turns(std::vector<char> const& in)
{
  auto ret = std::vector<char> {};

  auto rs = 0;
  for (auto c : in) {
    if (c == 'R') {
      ++rs;
    } else {
      if (rs == 1) {
        ret.push_back('R');
      } else if (rs == 3) {
        ret.push_back('L');
      }
      rs = 0;
      ret.push_back(c);
    }
  }

  return ret;
}

struct program {
  std::vector<int> A;
  std::vector<int> B;
  std::vector<int> C;
  std::vector<int> M;
};

program compress(std::vector<int> const& in)
{
  auto ret = program {};
  return ret;
}

int main()
{
  std::string line;
  std::getline(std::cin, line);

  auto rob = robot(line);
  std::cout << rob.alignment() << '\n';

  rob.dump();

  auto path = opt_moves(opt_turns(rob.explore()));
  auto prog = compress(path);
}
