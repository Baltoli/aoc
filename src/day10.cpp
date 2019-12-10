#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
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

  int visible_count(loc l) const;
  loc most_visible() const;

  double angle(loc laser, loc ast) const;

  std::vector<loc> destroyed(int max) const;

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

int asteroids::visible_count(loc l) const
{
  auto masked = std::vector<int>(map.size(), 0);

  for (int i = 0; i < (width * height); ++i) {
    if (map[i] == 1) {
      auto a_loc = to_loc(i);
      auto dx    = a_loc.x - l.x;
      auto dy    = a_loc.y - l.y;
      auto step  = std::gcd(dx, dy);

      if (step == 0) {
        continue;
      }

      dx /= step;
      dy /= step;

      int x, y;
      for (x = l.x + dx, y = l.y + dy; map[to_idx({x, y})] == 0;
           x += dx, y += dy) {
      }

      masked[to_idx({x, y})] = 1;
    }
  }

  return std::count(masked.begin(), masked.end(), 1);
}

loc asteroids::most_visible() const
{
  int max_vis = -1;
  loc max_loc;

  for (int i = 0; i < (width * height); ++i) {
    if (map[i] == 1) {
      auto vis = visible_count(to_loc(i));
      if (vis > max_vis) {
        max_vis = vis;
        max_loc = to_loc(i);
      }
    }
  }

  return max_loc;
}

double asteroids::angle(loc laser, loc ast) const
{
  constexpr double pi = 3.141592653589793238463;

  auto dx = laser.x - ast.x;
  auto dy = laser.y - ast.y;

  auto raw = ((std::atan2(dy, dx) / pi) + 1) / 2;
  return raw >= 0.75 ? raw - 0.75 : raw + 0.25;
}

std::vector<loc> asteroids::destroyed(int max) const
{
  auto ret  = std::vector<loc> {};
  auto work = std::map<double, std::vector<loc>> {};

  auto laser = most_visible();

  for (int i = 0; i < (width * height); ++i) {
    if (map[i] == 1) {
      auto ang = angle(laser, to_loc(i));
      work.try_emplace(ang);

      work[ang].push_back(to_loc(i));
    }
  }

  while (ret.size() < max) {
    for (auto [k, vec] : work) {
      if (!vec.empty()) {
        ret.push_back(vec[0]);
        vec.erase(vec.begin());
      }
    }
  }

  return ret;
}

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

  std::cout << ast.visible_count(ast.most_visible()) << '\n';

  auto answer = ast.destroyed(200)[199];
  std::cout << (answer.x * 100 + answer.y) << '\n';
}
