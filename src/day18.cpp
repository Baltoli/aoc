#include <iostream>
#include <map>
#include <string>
#include <vector>

class maze {
public:
  maze();

private:
  std::map<char, std::pair<int, int>> keys_;
  std::map<char, std::pair<int, int>> doors_;
  std::vector<std::vector<char>>      map_;
};

maze::maze()
    : map_ {}
{
  for (std::string line; std::getline(std::cin, line);) {
    map_.emplace_back(line.begin(), line.end());
  }

  for (int y = 0; y < map_.size(); ++y) {
    for (int x = 0; x < map_[y].size(); ++x) {
      if (std::islower(map_[y][x])) {
        keys_[map_[y][x]] = {x, y};
      } else if (std::isupper(map_[y][x])) {
        doors_[std::tolower(map_[y][x])] = {x, y};
      }
    }
  }
}

int main() { auto m = maze(); }
