#include <utils/grid.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <ranges>

namespace ranges = std::ranges;
namespace views  = std::ranges::views;

using base_grid = utils::grid<utils::no_pad>;

constexpr utils::point move_delta(char move) noexcept
{
  switch (move) {
  case '^': return {0, -1};
  case 'v': return {0, 1};
  case '<': return {-1, 0};
  case '>': return {1, 0};
  }
  std::unreachable();
}

class warehouse : public base_grid {
public:
  static constexpr char robot = '@';
  static constexpr char wall  = '#';
  static constexpr char box   = 'O';
  static constexpr char floor = '.';

  template <typename Range>
  warehouse(Range&& range)
      : base_grid(std::forward<Range>(range))
  {
    for (auto p : coords()) {
      if (at(p) == robot) {
        robot_ = p;
        break;
      }
    }
  }

  void apply(char move)
  {
    auto delta   = move_delta(move);
    auto end_loc = robot_ + delta;

    for (;; end_loc += delta) {
      if (at(end_loc) != box) {
        break;
      }
    }

    if (at(end_loc) == wall) {
      return;
    }

    if (at(end_loc) != floor) {
      fmt::print("{}\n", move);
    }
    assert(at(end_loc) == floor);

    for (auto loc = end_loc; loc != robot_; loc -= delta) {
      at_unchecked(loc) = at(loc - delta);
    }

    at_unchecked(robot_) = floor;
    robot_ += delta;
  }

  template <typename Range>
  void apply(Range&& range)
    requires(std::is_same_v<std::ranges::range_value_t<Range>, char>)
  {
    for (auto c : range) {
      apply(c);
    }
  }

  std::int64_t gps_score() const noexcept
  {
    return utils::sum(coords(), [this](auto p) {
      if (at(p) == box) {
        return p.x + (p.y * 100);
      }

      return std::int64_t {0};
    });
  }

private:
  utils::point robot_;
};

template <typename Range>
std::vector<char> get_moves(Range&& range)
{
  auto out = std::vector<char> {};

  for (auto&& line : std::forward<Range>(range)) {
    ranges::copy(std::forward<decltype(line)>(line), std::back_inserter(out));
  }

  return out;
}

auto until_empty()
{
  return views::take_while([](auto const& line) { return !line.empty(); });
}

int main()
{
  auto map   = warehouse(utils::lines() | until_empty());
  auto moves = get_moves(utils::lines());
  map.apply(moves);
  fmt::print("{}\n", map.gps_score());
}
