#include <iostream>
#include <map>
#include <string>
#include <vector>

class maze {
public:
  maze();

  char& at(int x, int y);

private:
  std::map<char, std::pair<int, int>> keys_;
  std::map<char, std::pair<int, int>> doors_;
  std::vector<std::vector<char>>      map_;
};

char& maze::at(int x, int y) { return map_[y][x]; }

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

int main() { auto m = maze(); }
