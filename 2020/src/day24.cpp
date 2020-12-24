#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace ctre::literals;

enum class move { E, SE, SW, W, NW, NE };

struct point {
  point()
      : x(0)
      , y(0)
      , z(0)
  {
  }

  int x;
  int y;
  int z;

  point& operator+=(move m)
  {
    switch (m) {
    case move::E:
      ++x;
      --y;
      break;
    case move::SE:
      --z;
      ++x;
      break;
    case move::SW:
      --z;
      ++y;
      break;
    case move::W:
      --x;
      ++y;
      break;
    case move::NW:
      ++z;
      --x;
      break;
    case move::NE:
      ++z;
      --y;
      break;
    }

    assert(x + y + z == 0);
    return *this;
  }

  point operator+(move m) const
  {
    auto ret = *this;
    ret += m;
    return ret;
  }

  bool operator==(point const& other) const
  {
    return std::tie(x, y, z) == std::tie(other.x, other.y, other.z);
  }
};

namespace std {
template <>
struct hash<point> {
  size_t operator()(point const& p) const noexcept
  {
    auto seed = size_t {0};
    utils::hash_combine(seed, p.x);
    utils::hash_combine(seed, p.y);
    utils::hash_combine(seed, p.z);
    return seed;
  }
};
} // namespace std

move parse_move(std::string_view match)
{
  if (match == "e") {
    return move::E;
  } else if (match == "se") {
    return move::SE;
  } else if (match == "sw") {
    return move::SW;
  } else if (match == "w") {
    return move::W;
  } else if (match == "nw") {
    return move::NW;
  } else if (match == "ne") {
    return move::NE;
  } else {
    assert(false && "Bad move");
  }
}

std::vector<move> parse_line(std::string_view line)
{
  constexpr auto pattern = "(e|se|sw|w|nw|ne)(.*)"_ctre;

  auto ret = std::vector<move> {};
  for (auto m = pattern.match(line); !line.empty();
       line = m.template get<2>(), m = pattern.match(line)) {
    ret.push_back(parse_move(m.template get<1>()));
  }
  return ret;
}

int main()
{
  auto floor = std::unordered_map<point, bool> {};

  utils::for_each_line([&](auto const& line) {
    auto moves = parse_line(line);
    auto p     = point();
    for (auto m : moves) {
      p += m;
    }

    floor.try_emplace(p, false);
    floor.at(p) = !floor.at(p);
  });

  std::cout << std::count_if(floor.begin(), floor.end(), [](auto const& p) {
    return p.second;
  }) << '\n';
}
