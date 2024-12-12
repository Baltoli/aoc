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

  std::size_t area() const;
  std::size_t perimeter() const;
  std::size_t sides() const;
  std::size_t cost() const;
  std::size_t bulk_cost() const;
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

    validate();
  }

  std::size_t cost() const
  {
    return utils::sum(regions_, [](auto const& r) { return r.cost(); });
  }

  std::size_t bulk_cost() const
  {
    return utils::sum(regions_, [](auto const& r) { return r.bulk_cost(); });
  }

private:
  void validate() const
  {
    auto sum = std::size_t {0};
    for (auto const& r : regions_) {
      assert(!r.points.empty());
      assert(std::ranges::all_of(r.points, [this, &r](auto p) {
        return at(p) == at(*r.points.begin());
      }));
      sum += r.points.size();
    }
    assert(sum == data_.size());
  }

  std::vector<region> regions_;
};

std::size_t region::area() const { return points.size(); }

std::size_t region::perimeter() const
{
  auto perim = std::size_t {0};
  for (auto p : points) {
    auto out_sides = std::size_t {4};
    for (auto n : grid.ortho_neighbours(p)) {
      if (points.contains(n)) {
        out_sides -= 1;
      }
    }
    perim += out_sides;
  }
  return perim;
}

std::size_t region::sides() const { return 0; }

std::size_t region::cost() const { return area() * perimeter(); }

std::size_t region::bulk_cost() const { return area() * sides(); }

int main()
{
  auto in = garden_grid(utils::lines());
  fmt::print("{}\n", in.cost());
  fmt::print("{}\n", in.bulk_cost());
}
