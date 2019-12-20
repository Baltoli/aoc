#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

class maze {
public:
  maze();

  std::vector<std::pair<int, int>>
  path(std::pair<int, int> from, std::pair<int, int> to) const;

  int width() const;
  int height() const;

  char const& at(int x, int y) const;
  char&       at(int x, int y);
  char const& at(std::pair<int, int> p) const;
  char&       at(std::pair<int, int> p);

  bool floor(char c) const { return c == '.'; }
  bool wall(char c) const { return c == '#'; }
  bool door(char c) const { return std::isupper(c); }
  bool key(char c) const { return std::islower(c); }

  void dump() const;

  auto portal() const { return portal_; }

private:
  std::pair<int, int>                 portal_;
  std::map<char, std::pair<int, int>> keys_;
  std::map<char, std::pair<int, int>> doors_;
  std::vector<std::vector<char>>      map_;
};

int         maze::width() const { return map_[0].size(); }
int         maze::height() const { return map_.size(); }
char&       maze::at(int x, int y) { return map_[y][x]; }
char const& maze::at(int x, int y) const { return map_[y][x]; }
char const& maze::at(std::pair<int, int> p) const
{
  return at(p.first, p.second);
}
char& maze::at(std::pair<int, int> p) { return at(p.first, p.second); }

void maze::dump() const
{
  for (int y = 0; y < height(); ++y) {
    for (int x = 0; x < width(); ++x) {
      std::cout << at(x, y);
    }
    std::cout << '\n';
  }
}

maze::maze()
    : map_ {}
{
  for (std::string line; std::getline(std::cin, line);) {
    map_.emplace_back(line.begin(), line.end());
  }

  for (int y = 0; y < map_.size(); ++y) {
    for (int x = 0; x < map_[y].size(); ++x) {
      if (std::islower(at(x, y))) {
        keys_[at(x, y)] = {x, y};
      } else if (std::isupper(at(x, y))) {
        doors_[std::tolower(at(x, y))] = {x, y};
      } else if (at(x, y) == '@') {
        portal_ = {x, y};
      }
    }
  }
}

std::vector<std::pair<int, int>>
maze::path(std::pair<int, int> from, std::pair<int, int> to) const
{
  assert(!wall(at(from)) && !wall(at(to)) && "Can't pathfind into wall");

  auto ret   = std::vector<std::pair<int, int>> {};
  auto queue = std::queue<std::pair<int, int>> {{from}};
  auto costs = std::map<std::pair<int, int>, int> {{from, 0}};

  auto found = [&] { return costs.find(to) != costs.end(); };

  while (!found()) {
    assert(!queue.empty() && "No path possible");

    auto work = queue.front();
    queue.pop();

    for (int dx : {-1, 0, 1}) {
      for (int dy : {-1, 0, 1}) {
        if (dx == dy) {
          continue;
        }

        auto [fx, fy] = work;
        auto new_loc  = std::pair {fx + dx, fy + dy};
        if (!wall(at(new_loc))) {
          if (costs.find(new_loc) == costs.end()) {
            costs[new_loc] = costs[work] + 1;
            queue.push(new_loc);
          }
        }
      }
    }
  }

  auto ptr = to;
  while (ptr != from) {
    ret.push_back(ptr);
    for (int dx : {-1, 0, 1}) {
      for (int dy : {-1, 0, 1}) {
        if (dx == dy) {
          continue;
        }

        auto [fx, fy] = ptr;
        auto new_loc  = std::pair {fx + dx, fy + dy};
        if (costs.find(new_loc) != costs.end() && costs[new_loc] < costs[ptr]) {
          ptr = new_loc;
        }
      }
    }
  }

  std::reverse(ret.begin(), ret.end());
  return ret;
}

int main()
{
  auto m = maze();
  m.dump();
}
