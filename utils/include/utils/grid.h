#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <generator>
#include <limits>
#include <ranges>
#include <vector>

namespace utils {

struct no_pad { };

template <char>
struct fill_pad { };

template <typename Pad>
struct is_pad_policy {
  static constexpr bool value = false;
};

template <>
struct is_pad_policy<no_pad> {
  static constexpr bool value = true;
};

template <char C>
struct is_pad_policy<fill_pad<C>> {
  static constexpr bool value = true;
};

template <typename Pad>
constexpr bool is_pad_policy_v = is_pad_policy<Pad>::value;

struct point {
  std::int64_t x;
  std::int64_t y;
};

point operator+(point const& a, point const& b)
{
  return point {a.x + b.x, a.y + b.y};
}

point operator*(point const& p, std::int64_t s)
{
  return point {p.x * s, p.y * s};
}

template <typename Pad = no_pad>
  requires(is_pad_policy_v<Pad>)
class grid;

template <typename Pad>
struct getter {
  static char at(grid<Pad> const&, point);
};

template <typename Pad>
  requires(is_pad_policy_v<Pad>)
class grid {
public:
  template <typename Range>
  grid(Range const& lines)
      : data_ {}
      , width_ {0}
      , height_ {0}
  {
    for (auto const& line : lines) {
      assert(width_ == 0 || width_ == line.size());
      width_ = line.size();

      std::ranges::copy(line, std::back_inserter(data_));
      height_ += 1;
    }

    assert(width_ <= std::numeric_limits<std::int64_t>::max());
    assert(height_ <= std::numeric_limits<std::int64_t>::max());
  }

  std::generator<point> coords() const
  {
    for (auto x = 0L; x < width_; ++x) {
      for (auto y = 0L; y < height_; ++y) {
        co_yield point {x, y};
      }
    }
  }

  std::generator<point> neighbour_deltas() const
  {
    for (auto dx = -1; dx <= 1; ++dx) {
      for (auto dy = -1; dy <= 1; ++dy) {
        if (dx != 0 || dy != 0) {
          co_yield point {dx, dy};
        }
      }
    }
  }

  char at(point p) const { return getter<Pad>::at(*this, p); }

  template <typename T>
  friend struct getter;

private:
  char at_unchecked(point p) const { return data_[p.y * width_ + p.x]; }

  bool in_bounds(point p) const
  {
    return p.x >= 0 && p.x < width_ && p.y >= 0 && p.y < height_;
  }

  std::vector<char> data_;
  std::size_t       width_;
  std::size_t       height_;
};

template <>
struct getter<no_pad> {
  static char at(grid<no_pad> const& g, point p)
  {
    if (!g.in_bounds(p)) {
      throw std::invalid_argument("out of bounds");
    }

    return g.at_unchecked(p);
  }
};

template <char C>
struct getter<fill_pad<C>> {
  static char at(grid<fill_pad<C>> const& g, point p)
  {
    if (!g.in_bounds(p)) {
      return C;
    }

    return g.at_unchecked(p);
  }
};

} // namespace utils