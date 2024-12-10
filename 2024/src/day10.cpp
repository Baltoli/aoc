#include <utils/grid.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <queue>
#include <unordered_map>
#include <unordered_set>

struct location {
  std::uint8_t                     height;
  std::unordered_set<utils::point> reachable;
  std::int64_t                     rating;
};

constexpr location loc_from_char(char c) noexcept
{
  return {
      .height    = utils::c_to<std::uint8_t>(c),
      .reachable = {},
      .rating    = 0,
  };
}

class topo_grid : public utils::grid<utils::no_pad, location> {
public:
  template <typename... Args>
  topo_grid(Args&&... args)
      : utils::grid<utils::no_pad, location>(std::forward<Args>(args)...)
  {
    for (auto p : sorted_coords()) {
      if (at(p).height == 9) {
        at_unchecked(p).reachable.insert(p);
        at_unchecked(p).rating = 1;
      } else {
        for (auto n : ortho_neighbours(p)) {
          if (at(n).height == at(p).height + 1) {
            for (auto r : at(n).reachable) {
              at_unchecked(p).reachable.insert(r);
            }
            at_unchecked(p).rating += at(n).rating;
          }
        }
      }
    }
  }

  template <typename Func>
  std::int64_t run(Func&& f) const
  {
    return utils::sum(coords(), [this, &f](auto const& p) {
      return at(p).height == 0 ? std::forward<Func>(f)(at(p)) : 0;
    });
  }

  std::int64_t part_1() const
  {
    return run([](auto const& loc) { return loc.reachable.size(); });
  }

  std::int64_t part_2() const
  {
    return run([](auto const& loc) { return loc.rating; });
  }

private:
  std::vector<utils::point> sorted_coords() const
  {
    auto all_coords = coords() | std::ranges::to<std::vector>();
    std::ranges::sort(all_coords, [this](auto const& a, auto const& b) {
      return at(a).height > at(b).height;
    });
    return all_coords;
  }
};

int main()
{
  auto in = topo_grid(utils::lines(), loc_from_char);
  fmt::print("{}\n", in.part_1());
  fmt::print("{}\n", in.part_2());
}
