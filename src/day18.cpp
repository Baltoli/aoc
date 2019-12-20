#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

class maze {
public:
  maze();

  int width() const;
  int height() const;

  char const& at(int x, int y) const;
  char&       at(int x, int y);

  void dump() const;

private:
  std::map<char, std::pair<int, int>> keys_;
  std::map<char, std::pair<int, int>> doors_;
  std::vector<std::vector<char>>      map_;
};

int         maze::width() const { return map_[0].size(); }
int         maze::height() const { return map_.size(); }
char&       maze::at(int x, int y) { return map_[y][x]; }
char const& maze::at(int x, int y) const { return map_[y][x]; }

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
      }
    }
  }
}

int main()
{
  auto m = maze();
  m.dump();
}
