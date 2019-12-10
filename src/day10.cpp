#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct asteroids {
  asteroids();

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

int main() { auto ast = asteroids(); }
