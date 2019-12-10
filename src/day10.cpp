#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct loc {
  int x;
  int y;
};

struct asteroids {
  asteroids();

  loc to_loc(int idx) const;
  int to_idx(loc l) const;

  std::vector<char> map;
  int               width;
  int               height;
};

asteroids::asteroids()
    : map {}
{
  height = 0;

  for (std::string line; std::getline(std::cin, line);) {
    width = line.size();
    height++;

    for (auto c : line) {
      if (c == '.') {
        map.push_back(0);
      } else if (c == '#') {
        map.push_back(1);
      } else {
        assert(false && "Bad map");
      }
    }
  }
}

loc asteroids::to_loc(int idx) const { return {idx % width, idx / width}; }
int asteroids::to_idx(loc l) const { return l.y * width + l.x; }

void tests(asteroids const& a)
{
  for (int i = 0; i < (a.width * a.height); ++i) {
    assert(i == a.to_idx(a.to_loc(i)) && "Bad conversion logic");
  }
}

int main()
{
  auto ast = asteroids();
  tests(ast);
}
