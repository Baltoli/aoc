#include <utils/ctre.h>
#include <utils/utils.h>

#include <array>
#include <cassert>
#include <iostream>
#include <unordered_set>

struct coord {
  int x;
  int y;
  int z;

  coord rotated(int r) const
  {
    auto new_coord = *this;

    auto axis     = (r >> 3) & 0b11;
    auto negate   = r & 0b1;
    auto distance = (r >> 1) & 0b11;

    auto neg_sign = negate ? -1 : 1;

    auto rotate_parts = std::array<int, 4> {};
    switch (axis) {
    case 0:
      rotate_parts = {+y, +z, -y, -z};
      break;
    case 1:
      rotate_parts = {+x, +z, -x, -z};
      break;
    case 2:
      rotate_parts = {+y, +x, -y, -x};
      break;
    }

    std::rotate(
        rotate_parts.begin(), rotate_parts.begin() + distance,
        rotate_parts.end());

    switch (axis) {
    case 0:
      new_coord.x = x * neg_sign;
      new_coord.y = rotate_parts[0];
      new_coord.z = rotate_parts[1];
      break;
    case 1:
      new_coord.x = rotate_parts[0];
      new_coord.y = y * neg_sign;
      new_coord.z = rotate_parts[1];
      break;
    case 2:
      new_coord.x = rotate_parts[1];
      new_coord.y = rotate_parts[0];
      new_coord.z = z * neg_sign;
      break;
    }

    auto rot_2 = std::array {new_coord.x, new_coord.y, new_coord.z};
    std::rotate(rot_2.begin(), rot_2.begin() + axis, rot_2.end());
    return {rot_2[0], rot_2[1], rot_2[2]};
  }
};

std::ostream& operator<<(std::ostream& os, coord const& c)
{
  os << c.x << ',' << c.y << ',' << c.z;
  return os;
}

namespace std {
template <>
struct hash<coord> {
  size_t operator()(coord const& c) const
  {
    auto seed = size_t {0};
    utils::hash_combine(seed, c.x);
    utils::hash_combine(seed, c.y);
    utils::hash_combine(seed, c.z);
    return seed;
  }
};
} // namespace std

bool operator==(coord const& a, coord const& b)
{
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

struct scanner {
  int                       id;
  std::unordered_set<coord> beacons;

  scanner rotated(int rotation)
  {
    auto r_beacons = std::unordered_set<coord> {};
    for (auto const& r : beacons) {
      r_beacons.insert(r.rotated(rotation));
    }
    return {id, r_beacons};
  }
};

std::vector<scanner> load()
{
  using namespace ctre::literals;

  auto lines = utils::get_lines();
  auto ret   = std::vector<scanner> {};

  auto header_pattern = R"(--- scanner (\d+) ---)"_ctre;
  auto coord_pattern  = R"((-?\d+),(-?\d+),(-?\d+))"_ctre;

  auto current = scanner {};
  for (auto const& line : lines) {
    if (auto header_m = header_pattern.match(line)) {
      current.id = utils::stoi(header_m.get<1>().str());
    } else if (auto coord_m = coord_pattern.match(line)) {
      current.beacons.insert(
          {utils::stoi(coord_m.get<1>().str()),
           utils::stoi(coord_m.get<2>().str()),
           utils::stoi(coord_m.get<3>().str())});
    } else if (line.empty()) {
      ret.push_back(current);
      current = scanner {};
    } else {
      assert(false && "Malformed input");
    }
  }

  return ret;
}

int main()
{
  auto in = load();

  // Part 1 logic
  // for every pair of scanners:
  //   not quite every pair - until all have been overlapped?
  //   check all rotations of second one
  //   shift so rotated and base have same smaller x coord
  //    get the y + z translations from this
  //   construct intersection of the two sets of points - if >= 12 then overlap
  //   translate factor is x, y, z from before - add to all and construct global
  //    set of points
  //   beacon position relative to **other frame**
}
