#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <string>
#include <vector>

using loc = std::pair<int, int>;

std::string from_chars(char a, char b)
{
  auto arr = std::array {a, b};
  return std::string(arr.begin(), arr.end());
}

class maze {
public:
  maze();

  std::vector<loc> path(loc from, loc to) const;

  int width() const;
  int height() const;

  char at(int x, int y) const;

  auto start() const { return start_; }
  auto end() const { return end_; }

  void dump() const;

private:
  std::vector<std::vector<char>> map_;

  std::map<loc, loc> portals_;
  loc                start_;
  loc                end_;
};

maze::maze()
    : map_ {}
    , start_ {0, 0}
    , end_ {0, 0}
{
  for (std::string line; std::getline(std::cin, line);) {
    map_.emplace_back(line.begin(), line.end());
  }

  auto key_locs = std::map<std::string, std::vector<loc>> {};

  for (int y = 0; y < height(); ++y) {
    for (int x = 0; x < width() - 1; ++x) {
      if (std::isupper(at(x, y)) && std::isupper(at(x + 1, y))) {
        auto str = from_chars(at(x, y), at(x + 1, y));
        key_locs.try_emplace(str);

        if (at(x - 1, y) == '.') {
          key_locs[str].push_back({x - 1, y});
        } else if (x + 2 < width() && at(x + 2, y) == '.') {
          key_locs[str].push_back({x + 2, y});
        }
      }
    }
  }

  for (int x = 0; x < width(); ++x) {
    for (int y = 0; y < height() - 1; ++y) {
      if (std::isupper(at(x, y)) && std::isupper(at(x, y + 1))) {
        auto str = from_chars(at(x, y), at(x, y + 1));
        key_locs.try_emplace(str);

        if (y > 0 && at(x, y - 1) == '.') {
          key_locs[str].push_back({x, y - 1});
        } else if (y + 2 < width() && at(x, y + 2) == '.') {
          key_locs[str].push_back({x, y + 2});
        }
      }
    }
  }

  start_ = key_locs["AA"][0];
  end_   = key_locs["ZZ"][0];

  for (auto const& [key, locs] : key_locs) {
    if (locs.size() == 2) {
      portals_[locs[0]] = locs[1];
      portals_[locs[1]] = locs[0];
    }
  }
}

int maze::width() const { return map_[0].size(); }
int maze::height() const { return map_.size(); }

char maze::at(int x, int y) const { return map_[y][x]; }

std::vector<loc> maze::path(loc from, loc to) const
{
  auto costs    = std::map<loc, int> {};
  auto get_cost = [&](auto l) {
    if (costs.find(l) == costs.end()) {
      return std::numeric_limits<int>::max();
    } else {
      return costs[l];
    }
  };

  auto queue = std::queue<loc> {{from}};
  while (!queue.empty()) {
    auto work = queue.front();
    queue.pop();
  }

  return {};
}

void maze::dump() const
{
  for (auto const& row : map_) {
    for (auto c : row) {
      std::cout << c;
    }
    std::cout << '\n';
  }
}

int main()
{
  auto m = maze();
  m.dump();

  auto path = m.path(m.start(), m.end());
  std::cout << path.size() << '\n';
}
