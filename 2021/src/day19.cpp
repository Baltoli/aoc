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
    auto n_x      = r & 1 ? -1 : 1;
    auto n_y      = r & 2 ? -1 : 1;
    auto n_z      = r & 4 ? -1 : 1;
    auto distance = (r >> 3) & 0b111;

    auto indices = std::array {0, 1, 2};
    for (int i = 0; i < distance; ++i) {
      std::next_permutation(indices.begin(), indices.end());
    }

    auto vals = std::array {x * n_x, y * n_y, z * n_z};
    return coord {vals[indices[0]], vals[indices[1]], vals[indices[2]]};
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

std::ostream& operator<<(std::ostream& os, scanner const& s)
{
  os << "--- scanner " << s.id << " ---\n";
  for (auto const& c : s.beacons) {
    os << c << '\n';
  }
  return os;
}

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

std::pair<bool, coord> align(scanner a, scanner b)
{
  for (auto const& a_beac : a.beacons) {
    for (auto const& b_beac : b.beacons) {
      auto dx = a_beac.x - b_beac.x;
      auto dy = a_beac.y - b_beac.y;
      auto dz = a_beac.z - b_beac.z;

      auto overlap_count = std::count_if(
          b.beacons.begin(), b.beacons.end(), [&](auto const& pre_translate) {
            return a.beacons.contains(coord {
                pre_translate.x + dx, pre_translate.y + dy,
                pre_translate.z + dz});
          });

      if (overlap_count >= 12) {
        return {true, {dx, dy, dz}};
      }
    }
  }

  return {false, {0, 0, 0}};
}

int main()
{
  auto in = load();

  auto done_ids = std::unordered_set<int> {};
  auto scanners = std::unordered_set<coord> {};

  while (done_ids.size() < in.size() - 1) {
    for (auto scan = 1; scan < in.size(); ++scan) {
      if (!done_ids.contains(in[scan].id)) {
        for (auto r = 0; r < 48; ++r) {
          auto rotated_scan         = in[scan].rotated(r);
          auto [aligned, translate] = align(in[0], rotated_scan);
          if (aligned) {
            for (auto const& a : rotated_scan.beacons) {
              in[0].beacons.insert(coord {
                  a.x + translate.x, a.y + translate.y, a.z + translate.z});
            }

            done_ids.insert(rotated_scan.id);
            scanners.insert(translate);
          }
        }
      }
    }
  }
  std::cout << in[0].beacons.size() << '\n';

  auto max_dist = std::numeric_limits<int>::min();
  for (auto a : scanners) {
    for (auto b : scanners) {
      if (a != b) {
        auto dist
            = std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z);
        max_dist = std::max(dist, max_dist);
      }
    }
  }
  std::cout << max_dist << '\n';
}
