#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <array>
#include <numeric>
#include <optional>
#include <unordered_set>

struct span;
struct cube;

namespace std {
template <>
struct hash<span> {
  size_t operator()(span const&) const;
};

template <>
struct hash<cube> {
  size_t operator()(cube const&) const;
};
} // namespace std

struct span {
  long min;
  long max;

  long size() const { return (max - min) + 1; }

  bool valid() const { return max >= min; }

  bool contains(span other) const
  {
    return other.min >= min && other.max <= max;
  }

  std::optional<span> intersect(span other) const
  {
    if (max < other.min || min > other.max) {
      return std::nullopt;
    } else {
      return span {std::max(min, other.min), std::min(max, other.max)};
    }
  }

  bool operator==(span const& other) const
  {
    return min == other.min && max == other.max;
  }

  bool contiguous(span const& other) const
  {
    return min == other.max + 1 || max == other.min - 1;
  }

  span append(span const& other) const
  {
    return span {std::min(min, other.min), std::max(max, other.max)};
  }
};

struct cube {
  bool on;
  span x;
  span y;
  span z;

  cube(std::string_view in)
  {
    auto m = ctre::match<
        R"((on|off) x=(-?\d+)\.\.(-?\d+),y=(-?\d+)\.\.(-?\d+),z=(-?\d+)\.\.(-?\d+))">(
        in);

    on = m.get<1>() == "on";
    x  = {utils::svtol(m.get<2>()), utils::svtol(m.get<3>())};
    y  = {utils::svtol(m.get<4>()), utils::svtol(m.get<5>())};
    z  = {utils::svtol(m.get<6>()), utils::svtol(m.get<7>())};
  }

  cube(bool on, span x, span y, span z)
      : on(on)
      , x(x)
      , y(y)
      , z(z)
  {
  }

  bool contains(cube const& other) const
  {
    return x.contains(other.x) && y.contains(other.y) && z.contains(other.z);
  }

  static long total_volume(std::vector<cube> const& cs)
  {
    return std::accumulate(
        cs.begin(), cs.end(), 0L,
        [](auto acc, auto const& c) { return acc + c.volume(); });
  }

  std::vector<cube> intersect(cube const& other) const
  {
    if (!on && !other.on) {
      return {};
    } else if (!on && other.on) {
      return other.intersect(*this);
    } else {
      auto x_in = x.intersect(other.x);
      auto y_in = y.intersect(other.y);
      auto z_in = z.intersect(other.z);

      if (x_in && y_in && z_in) {
        auto inter = cube(true, *x_in, *y_in, *z_in);

        if (other.on) {
          inter.on        = false;
          auto cut_result = other.intersect(inter);
          return cut_result;
        } else {
          auto x_ranges = std::array {
              span {x.min, inter.x.min - 1}, inter.x,
              span {inter.x.max + 1, x.max}};
          auto y_ranges = std::array {
              span {y.min, inter.y.min - 1}, inter.y,
              span {inter.y.max + 1, y.max}};
          auto z_ranges = std::array {
              span {z.min, inter.z.min - 1}, inter.z,
              span {inter.z.max + 1, z.max}};

          auto all_parts = std::vector<cube> {};
          for (auto xs : x_ranges) {
            for (auto ys : y_ranges) {
              for (auto zs : z_ranges) {
                if (xs.valid() && ys.valid() && zs.valid()) {
                  all_parts.emplace_back(true, xs, ys, zs);
                }
              }
            }
          }

          auto valid_parts = std::vector<cube> {};
          std::copy_if(
              all_parts.begin(), all_parts.end(),
              std::back_inserter(valid_parts),
              [&inter](auto const& part) { return !inter.contains(part); });

          return valid_parts;
        }
      } else {
        auto ret = std::vector {*this};
        if (other.on) {
          ret.push_back(other);
        }
        return ret;
      }
    }
  }

  long volume() const { return x.size() * y.size() * z.size(); }

  bool operator==(cube const& other) const
  {
    return x == other.x && y == other.y && z == other.z && on == other.on;
  }
};

std::vector<cube> load()
{
  return utils::map_lines([](auto const& l) { return cube(l); });
}

long compute(std::vector<cube> const& cs)
{
  auto working_set = std::unordered_set {cs[0]};

  for (auto i = 1; i < cs.size(); ++i) {
    auto new_set = std::unordered_set<cube> {};
    auto to_add  = cs[i];

    for (auto const& old_cube : working_set) {
      auto results = to_add.intersect(old_cube);
      for (auto r : results) {
        new_set.insert(r);
      }

      if (to_add.on) {
        new_set.insert(to_add);
      }
    }

    working_set = new_set;
  }

  return std::accumulate(
      working_set.begin(), working_set.end(), 0L,
      [](auto acc, auto const& cube) { return acc + cube.volume(); });
}

int main()
{
  auto in       = load();
  auto small_in = std::vector<cube> {};

  std::copy_if(
      in.begin(), in.end(), std::back_inserter(small_in), [](auto const& cube) {
        auto range = span {-50, 50};
        return range.contains(cube.x) && range.contains(cube.y)
               && range.contains(cube.z);
      });

  fmt::print("{}\n", compute(small_in));
  fmt::print("{}\n", compute(in));
}

namespace std {

size_t hash<span>::operator()(span const& s) const
{
  auto seed = size_t {0};
  utils::hash_combine(seed, s.min, s.max);
  return seed;
}

size_t hash<cube>::operator()(cube const& c) const
{
  auto seed = size_t {0};
  utils::hash_combine(seed, c.x, c.y, c.z, c.on);
  return seed;
}

} // namespace std
