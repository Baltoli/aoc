#include <utils/grid.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <unordered_set>

using exit_pad = utils::fill_pad<'%'>;

namespace std {
template <>
struct hash<std::pair<utils::point, utils::point>> {
  size_t operator()(std::pair<utils::point, utils::point> const& p) const
  {
    auto seed = std::size_t {0};
    utils::hash_combine(seed, p.first);
    utils::hash_combine(seed, p.second);
    return seed;
  }
};
} // namespace std

class map_grid : public utils::grid<exit_pad> {
public:
  template <typename Range>
  map_grid(Range&& lines)
      : utils::grid<exit_pad>(std::forward<Range>(lines))
  {
    for (auto p : coords()) {
      if (at(p) == '^') {
        start_               = p;
        at_unchecked(start_) = '.';
        break;
      }
    }
  }

  auto part_1() const
  {
    auto visited = std::unordered_set<utils::point> {};

    auto current   = start_;
    auto direction = utils::point {0, -1};

    while (at(current) != exit_pad::value) {
      visited.insert(current);

      auto next = current + direction;

      if (at(next) == '#') {
        direction = direction.right_turn();
        continue;
      }

      current = next;
    }

    return visited.size();
  }

  bool loops_with_obstacle_at(utils::point p) const
  {
    if (p == start_) {
      return false;
    }

    return false;
  }

  auto part_2() const
  {
    auto total = 0L;

    for (auto p : coords()) {
      if (loops_with_obstacle_at(p)) {
        total += 1;
      }
    }

    return total;
  }

private:
  utils::point start_;
};

int main()
{
  auto map = map_grid(utils::lines());
  fmt::print("{}\n", map.part_1());
  fmt::print("{}\n", map.part_2());
}
