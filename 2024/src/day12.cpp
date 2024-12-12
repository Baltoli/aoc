#include <utils/grid.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <queue>
#include <unordered_set>

using base_grid = utils::grid<utils::fill_pad<'.'>>;

class garden_grid;

struct region {
  garden_grid&                     grid;
  std::unordered_set<utils::point> points = {};

  std::pair<utils::point, utils::point> bounding_box() const;

  std::size_t area() const;
  std::size_t perimeter(bool unique) const;
  std::size_t cost(bool unique) const;
};

class garden_grid : public base_grid {
public:
  template <typename... Args>
  garden_grid(Args&&... args)
      : base_grid(std::forward<Args>(args)...)
  {
    auto visited = std::unordered_set<utils::point> {};

    for (auto p : coords()) {
      if (visited.contains(p)) {
        continue;
      }

      auto& current = regions_.emplace_back(*this);

      auto queue = std::queue<utils::point> {};
      queue.push(p);

      while (!queue.empty()) {
        auto next = queue.front();
        queue.pop();

        if (visited.contains(next)) {
          continue;
        }

        if (at(next) == at(p)) {
          visited.insert(next);
          current.points.insert(next);
          for (auto n : ortho_neighbours(next)) {
            queue.push(n);
          }
        }
      }
    }
  }

  std::size_t cost(bool unique) const
  {
    return utils::sum(
        regions_, [unique](auto const& r) { return r.cost(unique); });
  }

private:
  std::vector<region> regions_;
};

std::size_t region::area() const { return points.size(); }

std::pair<utils::point, utils::point> region::bounding_box() const
{
  auto i64_min = std::numeric_limits<std::int64_t>::min();
  auto i64_max = std::numeric_limits<std::int64_t>::max();

  auto min = utils::point {i64_max, i64_max};
  auto max = utils::point {i64_min, i64_min};

  for (auto p : points) {
    min.x = std::min(min.x, p.x);
    min.y = std::min(min.y, p.y);
    max.x = std::max(max.x, p.x);
    max.y = std::max(max.y, p.y);
  }

  auto offset = utils::point {1, 1};
  return {min - offset, max + offset};
}

std::size_t region::perimeter(bool unique) const
{
  auto [min, max] = bounding_box();
  auto sides      = 0;

  auto prev_scan = std::unordered_set<std::pair<std::int64_t, std::int64_t>> {};

  for (auto y = min.y; y <= max.y; ++y) {
    auto scan = std::unordered_set<std::pair<std::int64_t, std::int64_t>> {};
    for (auto x = min.x; x < max.x; ++x) {
      auto in_here = points.contains({x, y});
      auto in_next = points.contains({x + 1, y});

      if (in_here != in_next) {
        scan.insert(std::pair {x, in_here - in_next});
      }
    }

    for (auto cross : scan) {
      if (!prev_scan.contains(cross) || !unique) {
        sides += 1;
      }
    }

    prev_scan = scan;
  }

  prev_scan = {};

  for (auto x = min.x; x <= max.x; ++x) {
    auto scan = std::unordered_set<std::pair<std::int64_t, std::int64_t>> {};
    for (auto y = min.y; y < max.y; ++y) {
      auto in_here = points.contains({x, y});
      auto in_next = points.contains({x, y + 1});

      if (in_here != in_next) {
        scan.insert(std::pair {y, in_here - in_next});
      }
    }

    for (auto cross : scan) {
      if (!prev_scan.contains(cross) || !unique) {
        sides += 1;
      }
    }

    prev_scan = scan;
  }

  return sides;
}

std::size_t region::cost(bool unique) const
{
  return area() * perimeter(unique);
}

int main()
{
  auto in = garden_grid(utils::lines());
  fmt::print("{}\n", in.cost(false));
  fmt::print("{}\n", in.cost(true));
}
