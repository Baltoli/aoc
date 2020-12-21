#include <utils/ctre.h>
#include <utils/utils.h>

#include <array>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace ctre::literals;

constexpr size_t tile_size = 10;

bool on(char c)
{
  assert((c == '#' || c == '.') && "Bad char");
  return c == '#';
}

struct tile {
  tile()
      : id(0)
      , edges {0, 0, 0, 0}
  {
  }

  tile(long i, std::vector<std::string> const& v)
      : id(i)
      , edges {0, 0, 0, 0}
  {
    for (auto row = 0; row < tile_size; ++row) {
      auto chars = v[row];

      for (auto col = 0; col < tile_size; ++col) {
        if (row == 0) {
          top()[col] = on(chars[col]);
        } else if (row == tile_size - 1) {
          bottom()[col] = on(chars[col]);
        }
      }

      left()[row]  = on(chars[0]);
      right()[row] = on(chars[tile_size - 1]);
    }

    for (auto row = 1; row < tile_size - 1; ++row) {
      for (auto col = 1; col < tile_size - 1; ++col) {
        image_at(row - 1, col - 1) = v[row][col];
      }
    }
  }

  tile h_flip() const
  {
    auto ret = *this;
    std::swap(ret.left(), ret.right());
    for (auto i = 0; i < tile_size / 2; ++i) {
      std::swap(ret.top()[i], ret.top()[tile_size - i - 1]);
      std::swap(ret.bottom()[i], ret.bottom()[tile_size - i - 1]);
    }
    return ret;
  }

  tile v_flip() const
  {
    auto ret = *this;
    std::swap(ret.top(), ret.bottom());
    for (auto i = 0; i < tile_size / 2; ++i) {
      std::swap(ret.left()[i], ret.left()[tile_size - i - 1]);
      std::swap(ret.right()[i], ret.right()[tile_size - i - 1]);
    }
    return ret;
  }

  tile rotated(int times) const
  {
    auto ret = *this;
    for (auto i = 0; i < times; ++i) {
      std::rotate(ret.edges.begin(), ret.edges.begin() + 1, ret.edges.end());
      for (auto i = 0; i < tile_size / 2; ++i) {
        std::swap(ret.left()[i], ret.left()[tile_size - i - 1]);
        std::swap(ret.right()[i], ret.right()[tile_size - i - 1]);
      }
    }
    return ret;
  }

  bool operator==(tile const& other) const
  {
    return std::tie(id, edges) == std::tie(other.id, other.edges);
  }

  char& image_at(int row, int col)
  {
    return image[row * (tile_size - 2) + col];
  }

  std::array<char, (tile_size - 2) * (tile_size - 2)> image;

  std::bitset<tile_size>&       top() { return edges[0]; }
  std::bitset<tile_size>&       right() { return edges[1]; }
  std::bitset<tile_size>&       bottom() { return edges[2]; }
  std::bitset<tile_size>&       left() { return edges[3]; }
  std::bitset<tile_size> const& top() const { return edges[0]; }
  std::bitset<tile_size> const& right() const { return edges[1]; }
  std::bitset<tile_size> const& bottom() const { return edges[2]; }
  std::bitset<tile_size> const& left() const { return edges[3]; }

  long id;

  // 0 - top, 1 - right, 2 - bottom, 3 - left
  std::array<std::bitset<tile_size>, 4> edges;
};
using tile_set = std::unordered_set<tile>;

namespace std {
template <>
struct hash<tile> {
  size_t operator()(tile const& t) const
  {
    auto seed = size_t {0};
    utils::hash_combine(seed, t.id);
    for (auto i = 0; i < t.edges.size(); ++i) {
      utils::hash_combine(seed, t.edges[i]);
    }
    return seed;
  }
};
} // namespace std

tile_set get_input()
{
  auto id_pattern = "Tile (\\d+):"_ctre;
  auto ret        = tile_set {};

  auto line    = std::string {};
  auto current = std::vector<std::string> {};
  auto cur_id  = 0;

  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      auto new_t = tile(cur_id, current);
      current.clear();
      cur_id = 0;

      ret.insert(new_t);
      ret.insert(new_t.h_flip());
      ret.insert(new_t.v_flip());
      ret.insert(new_t.h_flip().v_flip());
    } else if (auto m = id_pattern.match(line)) {
      cur_id = utils::svtoi(m.template get<1>());
    } else {
      current.push_back(line);
    }
  }

  return ret;
}

std::unordered_map<unsigned long, long> edge_counts(tile_set const& ts)
{
  auto ret = std::unordered_map<unsigned long, long> {};
  for (auto const& t : ts) {
    for (auto const& edge : t.edges) {
      auto num = edge.to_ulong();
      ret.try_emplace(num, 0);
      ret.at(num) += 1;
    }
  }
  return ret;
}

tile_set find_corners(tile_set const& ts)
{
  auto ret    = tile_set {};
  auto counts = edge_counts(ts);

  for (auto const& t : ts) {
    for (auto i = 0; i < 4; ++i) {
      auto adj = (i + 1) % 4;
      auto e_1 = t.edges[i].to_ulong();
      auto e_2 = t.edges[adj].to_ulong();

      if (counts.at(e_1) == 2 && counts.at(e_2) == 2) {
        ret.insert(t);
      }
    }
  }

  return ret;
}

std::unordered_set<long> unique_ids(tile_set const& ts)
{
  auto ret = std::unordered_set<long> {};
  for (auto const& t : ts) {
    ret.insert(t.id);
  }
  return ret;
}

long part_1(tile_set const& ts)
{
  auto corners = find_corners(ts);

  auto prod = 1L;
  for (auto c : unique_ids(corners)) {
    prod *= c;
  }

  return prod;
}

struct solution {
  solution(tile_set ts)
      : tiles(ts)
      , dim(std::sqrt(tiles.size() / 4))
      , arrangement(dim, std::vector<tile>(dim, tile()))
  {
    auto count   = edge_counts(tiles);
    auto corners = find_corners(tiles);

    for (auto const& corner : corners) {
      auto top  = corner.top().to_ulong();
      auto left = corner.left().to_ulong();

      if (count.at(top) == 2 && count.at(left) == 2) {
        place(0, 0, corner);
      }
    }

    for (auto i = 1; i < dim; ++i) {
      for (auto const& t : tiles) {
        if (placed.find(t.id) == placed.end()) {
          for (auto rot = 0; rot < 4; ++rot) {
            auto rot_t = t.rotated(rot);
            if (rot_t.left() == arrangement[0][i - 1].right()
                && count.at(rot_t.top().to_ulong()) == 2) {
              place(0, i, rot_t);
            }

            if (rot_t.top() == arrangement[i - 1][0].bottom()
                && count.at(rot_t.left().to_ulong()) == 2) {
              place(i, 0, rot_t);
            }
          }
        }
      }
    }

    for (auto row = 1; row < dim; ++row) {
      for (auto col = 1; col < dim; ++col) {
        for (auto const& t : tiles) {
          if (placed.find(t.id) == placed.end()) {
            for (auto rot = 0; rot < 4; ++rot) {
              auto rot_t = t.rotated(rot);
              if (rot_t.left() == arrangement[row][col - 1].right()
                  && rot_t.top() == arrangement[row - 1][col].bottom()) {
                place(row, col, rot_t);
              }
            }
          }
        }
      }
    }
  }

  bool place(int row, int col, tile t)
  {
    if (placed.find(t.id) != placed.end()) {
      return false;
    }

    if (arrangement[row][col].id > 0) {
      return false;
    }

    placed.insert(t.id);
    arrangement[row][col] = t;
    return true;
  }

  void dump()
  {
    for (auto const& row : arrangement) {
      for (auto const& t : row) {
        std::cout << std::setw(4) << t.id << ' ';
      }
      std::cout << '\n';
    }
  }

  tile_set                       tiles;
  int                            dim;
  std::unordered_set<long>       placed;
  std::vector<std::vector<tile>> arrangement;
};

long part_2(tile_set const& ts)
{
  auto soln = solution(ts);
  soln.dump();
  return 0;
}

int main()
{
  auto ts = get_input();
  std::cout << part_1(ts) << '\n';
  std::cout << part_2(ts) << '\n';
}
