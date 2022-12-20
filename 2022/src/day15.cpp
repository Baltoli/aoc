#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <optional>
#include <set>

using namespace ctre::literals;

constexpr auto pattern
    = "Sensor at x=(-?\\d+), y=(-?\\d+): closest beacon is at x=(-?\\d+), y=(-?\\d+)"_ctre;

using point = std::pair<long, long>;

struct range {
  long start;
  long end;

  std::optional<range> range_union(range const& other) const
  {
    if (other.end < start - 1 || other.start > end + 1) {
      return std::nullopt;
    }

    return range {std::min(start, other.start), std::max(end, other.end)};
  }

  long size() const { return end - start + 1; }
};

struct input {
  input(std::string_view sv)
  {
    auto m = pattern.match(sv);
    sensor = {utils::svtol(m.get<1>()), utils::svtol(m.get<2>())};
    beacon = {utils::svtol(m.get<3>()), utils::svtol(m.get<4>())};
  }

  long radius() const
  {
    return std::abs(sensor.first - beacon.first)
           + std::abs(sensor.second - beacon.second);
  }

  std::optional<range> excluded(long row) const
  {
    auto distance = std::abs(row - sensor.second);
    if (distance > radius()) {
      return std::nullopt;
    }

    auto ret            = std::set<point> {};
    auto reduced_radius = radius() - distance;

    return range {sensor.first - reduced_radius, sensor.first + reduced_radius};
  }

  point sensor;
  point beacon;
};

void simplify(std::vector<range>& ranges)
{
  bool any_merged;

  do {
    for (auto first_idx = 0; first_idx < ranges.size(); ++first_idx) {
      any_merged = false;

      if (ranges.size() <= 1) {
        break;
      }

      std::swap(ranges.at(0), ranges.at(first_idx));

      auto& first = ranges.at(0);
      for (auto it = ranges.begin() + 1; it != ranges.end(); ++it) {
        if (auto merged = first.range_union(*it)) {
          first = *merged;
          ranges.erase(it);
          any_merged = true;
          break;
        }
      }
    }
  } while (any_merged);
}

std::vector<range> ranges_at_row(std::vector<input> const& lines, long row)
{
  auto ranges = std::vector<range> {};

  for (auto const& line : lines) {
    auto r = line.excluded(row);
    if (r) {
      ranges.push_back(*r);
    }
  }

  simplify(ranges);
  return ranges;
}

long part_1(std::vector<input> const& lines)
{
  constexpr long row = 2'000'000;

  auto ranges = ranges_at_row(lines, row);
  assert(ranges.size() == 1);

  auto beacons = std::set<point> {};
  for (auto const& line : lines) {
    if (line.beacon.second == row) {
      beacons.insert(line.beacon);
    }
  }

  return ranges[0].size() - beacons.size();
}

long part_2(std::vector<input> const& lines)
{
  constexpr long max_row = 4'000'000;

  auto distress = point {-1, -1};

  for (auto i = 0; i <= max_row; ++i) {
    auto rs = ranges_at_row(lines, i);
    if (rs.size() == 2) {
      distress = {std::min(rs[0].end, rs[1].end) + 1, i};
      break;
    }
  }

  return distress.first * 4000000 + distress.second;
}

int main()
{
  auto lines = utils::construct_lines<input>();
  fmt::print("{}\n", part_1(lines));
  fmt::print("{}\n", part_2(lines));
}
