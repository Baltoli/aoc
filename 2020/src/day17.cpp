#include <utils/utils.h>

#include <algorithm>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct coord_3d {
  int x;
  int y;
  int z;

  bool operator==(coord_3d const& other) const noexcept
  {
    return std::tie(x, y, z) == std::tie(other.x, other.y, other.z);
  }
};

struct coord_4d {
  int x;
  int y;
  int z;
  int w;

  bool operator==(coord_4d const& other) const noexcept
  {
    return std::tie(x, y, z, w) == std::tie(other.x, other.y, other.z, other.w);
  }
};

namespace std {
template <>
struct hash<coord_3d> {
  std::size_t operator()(coord_3d const& c) const noexcept
  {
    std::size_t seed = 0;
    utils::hash_combine(seed, c.x, c.y, c.z);
    return seed;
  }
};

template <>
struct hash<coord_4d> {
  std::size_t operator()(coord_4d const& c) const noexcept
  {
    std::size_t seed = 0;
    utils::hash_combine(seed, c.x, c.y, c.z, c.w);
    return seed;
  }
};
} // namespace std

template <typename Coord>
struct game {
  static constexpr char dead  = '.';
  static constexpr char alive = '#';

  char& at(Coord c)
  {
    data.try_emplace(c);
    return data.at(c);
  }

  char get(Coord c)
  {
    if (data.find(c) == data.end()) {
      return dead;
    }

    return data.at(c);
  }

  template <typename F>
  void for_each_neighbour(Coord c, F&& f);

  int count_neighbours(Coord c)
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

    auto queue = std::unordered_set<Coord> {};
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

  std::unordered_map<Coord, char> data {};
};

template <>
template <typename F>
void game<coord_3d>::for_each_neighbour(coord_3d c, F&& f)
{
  for (auto dx = -1; dx <= 1; ++dx) {
    for (auto dy = -1; dy <= 1; ++dy) {
      for (auto dz = -1; dz <= 1; ++dz) {
        if (dx != 0 || dy != 0 || dz != 0) {
          std::forward<F>(f)(coord_3d {c.x + dx, c.y + dy, c.z + dz});
        }
      }
    }
  }
}

template <>
template <typename F>
void game<coord_4d>::for_each_neighbour(coord_4d c, F&& f)
{
  for (auto dx = -1; dx <= 1; ++dx) {
    for (auto dy = -1; dy <= 1; ++dy) {
      for (auto dz = -1; dz <= 1; ++dz) {
        for (auto dw = -1; dw <= 1; ++dw) {
          if (dx != 0 || dy != 0 || dz != 0 || dw != 0) {
            std::forward<F>(f)(
                coord_4d {c.x + dx, c.y + dy, c.z + dz, c.w + dw});
          }
        }
      }
    }
  }
}

int main()
{
  auto g  = game<coord_3d>();
  auto g2 = game<coord_4d>();

  auto y = 0;
  utils::for_each_line([&](auto const& line) {
    for (auto x = 0; x < line.size(); ++x) {
      g.at({x, y, 0})     = line.at(x);
      g2.at({x, y, 0, 0}) = line.at(x);
    }
    ++y;
  });

  g.step(6);
  std::cout << g.count_alive() << '\n';

  g2.step(6);
  std::cout << g2.count_alive() << '\n';
}
