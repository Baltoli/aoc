#include <utils/ctre.h>
#include <utils/utils.h>

#include <array>
#include <bitset>
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
  tile(long i, std::vector<std::string> const& v)
      : id(i)
      , edges {0, 0, 0, 0}
  {
    assert(v.size() == tile_size && "Bad size for lines vector");

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
  }

  tile h_flip()
  {
    auto ret = *this;
    std::swap(ret.left(), ret.right());
    for (auto i = 0; i < tile_size / 2; ++i) {
      std::swap(ret.top()[i], ret.top()[tile_size - i - 1]);
      std::swap(ret.bottom()[i], ret.bottom()[tile_size - i - 1]);
    }
    return ret;
  }

  tile v_flip()
  {
    auto ret = *this;
    std::swap(ret.top(), ret.bottom());
    for (auto i = 0; i < tile_size / 2; ++i) {
      std::swap(ret.left()[i], ret.left()[tile_size - i - 1]);
      std::swap(ret.right()[i], ret.right()[tile_size - i - 1]);
    }
    return ret;
  }

  bool operator==(tile const& other) const
  {
    return std::tie(id, edges) == std::tie(other.id, other.edges);
  }

  std::bitset<tile_size>& top() { return edges[0]; }
  std::bitset<tile_size>& right() { return edges[1]; }
  std::bitset<tile_size>& bottom() { return edges[2]; }
  std::bitset<tile_size>& left() { return edges[3]; }

  long id;

  // 0 - top, 1 - right, 2 - bottom, 3 - left
  std::array<std::bitset<tile_size>, 4> edges;
};

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

using tile_set = std::unordered_set<tile>;

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

int main()
{
  auto ts      = get_input();
  auto corners = find_corners(ts);

  auto prod = 1L;
  for (auto c : unique_ids(corners)) {
    prod *= c;
  }
  std::cout << prod << '\n';
}
