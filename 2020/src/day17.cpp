#include <utils/utils.h>

#include <algorithm>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct coord {
  int x;
  int y;
  int z;

  bool operator==(coord const& other) const noexcept
  {
    return std::tie(x, y, z) == std::tie(other.x, other.y, other.z);
  }
};

struct coord_hash {
  std::size_t operator()(coord const& c) const noexcept { return 0; }
};

struct game {
  static constexpr char dead  = '.';
  static constexpr char alive = '#';

  void dump()
  {
    auto min = coord {
        std::numeric_limits<int>::max(), std::numeric_limits<int>::max(),
        std::numeric_limits<int>::max()};

    auto max = coord {
        std::numeric_limits<int>::min(), std::numeric_limits<int>::min(),
        std::numeric_limits<int>::min()};

    for (auto const& [c, val] : data) {
      min.x = std::min(min.x, c.x);
      min.y = std::min(min.y, c.y);
      min.z = std::min(min.z, c.z);
      max.x = std::max(max.x, c.x);
      max.y = std::max(max.y, c.y);
      max.z = std::max(max.z, c.z);
    }

    for (auto z = min.z; z <= max.z; ++z) {
      std::cout << "z=" << z << '\n';
      for (auto y = min.y; y <= max.y; ++y) {
        for (auto x = min.x; x <= max.x; ++x) {
          std::cout << get(coord {x, y, z});
        }
        std::cout << '\n';
      }
      std::cout << '\n';
    }
  }

  char& at(coord c)
  {
    data.try_emplace(c);
    return data.at(c);
  }

  char get(coord c)
  {
    if (data.find(c) == data.end()) {
      return dead;
    }

    return data.at(c);
  }

  template <typename F>
  void for_each_neighbour(coord c, F&& f)
  {
    for (auto dx = -1; dx <= 1; ++dx) {
      for (auto dy = -1; dy <= 1; ++dy) {
        for (auto dz = -1; dz <= 1; ++dz) {
          if (dx != 0 || dy != 0 || dz != 0) {
            std::forward<F>(f)(coord {c.x + dx, c.y + dy, c.z + dz});
          }
        }
      }
    }
  }

  int count_neighbours(coord c)
  {
    int sum = 0;
    for_each_neighbour(
        c, [&](auto const& nc) { return sum += (get(nc) == alive); });
    return sum;
  }

  void clean()
  {
    for (auto it = data.begin(); it != data.end();) {
      if (it->second == dead) {
        it = data.erase(it);
      } else
        ++it;
    }
  }

  void step()
  {
    auto blit = *this;

    auto queue = std::unordered_set<coord, coord_hash> {};
    for (auto const& [c, val] : data) {
      queue.insert(c);
      for_each_neighbour(c, [&](auto const& nc) { queue.insert(nc); });
    }

    for (auto const& c : queue) {
      auto count = count_neighbours(c);
      if (count == 2) {
        continue;
      } else if (count == 3) {
        blit.at(c) = alive;
      } else {
        blit.at(c) = dead;
      }
    }

    std::swap(data, blit.data);
    clean();
  }

  void step(int n)
  {
    for (auto i = 0; i < n; ++i) {
      step();
    }
  }

  int count_alive() const
  {
    return std::count_if(data.begin(), data.end(), [](auto const& p) {
      return p.second == alive;
    });
  }

  std::unordered_map<coord, char, coord_hash> data {};
};

int main()
{
  auto g = game();
  auto y = 0;
  utils::for_each_line([&](auto const& line) {
    for (auto x = 0; x < line.size(); ++x) {
      g.at(coord {x, y, 0}) = line.at(x);
    }
    ++y;
  });

  g.step(6);
  std::cout << g.count_alive() << '\n';
}
