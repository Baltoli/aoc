#include <intcode.h>

#include <array>
#include <cassert>
#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <tuple>

struct loc {
  int x;
  int y;

  int distance(loc const& other)
  {
    return std::abs(x - other.x) + std::abs(y - other.y);
  }

  bool operator==(loc const& other) const
  {
    return std::tie(x, y) == std::tie(other.x, other.y);
  }

  bool operator!=(loc const& other) const { return !(*this == other); }

  bool operator<(loc const& other) const
  {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
};

loc move(loc start, int dir)
{
  switch (dir) {
  case 1:
    return {start.x, start.y - 1};
  case 2:
    return {start.x, start.y + 1};
  case 3:
    return {start.x - 1, start.y};
  case 4:
    return {start.x + 1, start.y};
  default:
    assert(false && "Bad direction");
  }
}

int opposite(int dir)
{
  return std::map<int, int> {{1, 2}, {2, 1}, {3, 4}, {4, 3}}[dir];
}

char pretty(long at)
{
  switch (at) {
  case 0:
    return '#';
  case 1:
    return '.';
  case 2:
    return 'O';
  default:
    assert(false && "Bad at");
  }
}

class robot {
public:
  robot(std::string const&);

  bool explored(loc) const;

  std::vector<long> path(loc a, loc b) const;

  long step(long dir);
  void explore();

  void dump() const;

  loc oxygen_ = {0, 0};

private:
  loc                 location_ = {0, 0};
  std::map<loc, long> map_      = {{{0, 0}, 1}};
  std::set<loc>       queue_    = {};

  ic::computer computer_;
};

robot::robot(std::string const& prog)
    : computer_(prog)
{
}

void robot::dump() const
{
  auto min = loc {0, 0};
  auto max = loc {0, 0};

  for (auto const& [k, v] : map_) {
    if (k.x < min.x) {
      min.x = k.x;
    }

    if (k.y < min.y) {
      min.y = k.y;
    }

    if (k.x > max.x) {
      max.x = k.x;
    }

    if (k.y > max.y) {
      max.y = k.y;
    }
  }

  std::cout << "\033[2J\033[H";
  for (int row = min.y; row <= max.y; ++row) {
    for (int col = min.x; col <= max.x; ++col) {
      if (col == location_.x && row == location_.y) {
        std::cout << 'D';
      } else if (map_.find({col, row}) != map_.end()) {
        std::cout << pretty(map_.at({col, row}));
      } else {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }
}

long robot::step(long dir)
{
  if (dir == 0) {
    auto dirs = std::array {1, 2, 3, 4};
    for (auto d : dirs) {
      auto next = move(location_, d);
      if (!explored(next)) {
        queue_.insert(next);
      }
    }

    return 1;
  }

  auto next_loc = move(location_, dir);

  computer_.input(dir);
  auto status = computer_.run();

  map_[next_loc] = status;

  switch (status) {
  case 0:
    break;
  case 1:
    location_ = next_loc;
    break;
  case 2:
    oxygen_   = next_loc;
    location_ = next_loc;
    break;
  default:
    assert(false && "Bad status");
  }

  auto dirs = std::array {1, 2, 3, 4};
  for (auto d : dirs) {
    auto next = move(location_, d);
    if (!explored(next)) {
      queue_.insert(next);
    }
  }

  return status;
}

bool robot::explored(loc l) const { return map_.find(l) != map_.end(); }

std::vector<long> robot::path(loc a, loc b) const
{
  auto ret   = std::vector<long> {};
  auto costs = std::map<loc, int> {{a, 0}};
  auto work  = std::queue<loc> {{a}};
  auto dirs  = std::array {1, 2, 3, 4};

  while (!work.empty()) {
    auto current = work.front();
    work.pop();

    if (current == b) {
      break;
    }

    for (auto d : dirs) {
      auto next  = move(current, d);
      auto found = costs.find(next);

      if (explored(next) && map_.at(next) > 0 && found == costs.end()) {
        costs[next] = costs[current] + 1;
        work.push(next);
      }
    }
  }

  auto ptr = b;
  while (ptr != a) {
    auto min_cost = std::numeric_limits<int>::max();
    auto min_dir  = -1;

    for (auto d : dirs) {
      auto next = move(ptr, d);
      if (costs.find(next) != costs.end() && costs.at(next) < min_cost) {
        min_cost = costs.at(next);
        min_dir  = d;
      }
    }

    assert(min_dir > 0 && "No path");
    ret.push_back(opposite(min_dir));

    ptr = move(ptr, min_dir);
  }

  std::reverse(ret.begin(), ret.end());
  return ret;
}

void robot::explore()
{
  step(0);

  while (!queue_.empty()) {
    auto beg
        = std::min_element(queue_.begin(), queue_.end(), [&](auto a, auto b) {
            return a.distance(location_) < b.distance(location_);
          });

    auto dest = queue_.extract(beg).value();
    auto dirs = path(location_, dest);

    for (auto d : dirs) {
      /* dump(); */
      step(d);
      /* std::this_thread::sleep_for(std::chrono::milliseconds(50)); */
    }
  }
}

int main()
{
  std::string line;
  std::getline(std::cin, line);

  auto rob = robot(line);
  rob.explore();
  rob.dump();

  std::cout << rob.path(loc {0, 0}, rob.oxygen_).size() << '\n';
}
