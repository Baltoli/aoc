#include <utils/grid.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <queue>
#include <ranges>
#include <unordered_set>

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

template <bool widened = false>
class warehouse : public base_grid {
public:
  static constexpr char robot     = '@';
  static constexpr char wall      = '#';
  static constexpr char box       = 'O';
  static constexpr char left_box  = '[';
  static constexpr char right_box = ']';
  static constexpr char floor     = '.';

  template <typename Range>
    requires(std::ranges::range<Range>)
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

  warehouse(
      std::vector<element_t> data, std::size_t width, std::size_t height,
      utils::point robot)
      : base_grid(std::move(data), width, height)
  {
    robot_ = robot;
    assert(data_.size() == width_ * height_);
  }

  warehouse<true> widen() const noexcept
    requires(!widened)
  {
    auto widen_tile = [](auto c) -> std::array<element_t, 2> {
      switch (c) {
      case wall:
      case floor: return {c, c};
      case box: return {left_box, right_box};
      case robot: return {robot, floor};
      }
      assert(false);
    };

    auto new_data = std::vector<element_t> {};
    for (auto tile : data_) {
      for (auto w : widen_tile(tile)) {
        new_data.push_back(w);
      }
    }

    return warehouse<true>(
        new_data, width_ * 2, height_, {robot_.x * 2, robot_.y});
  }

  void apply(char move)
  {
    auto delta   = move_delta(move);
    auto end_loc = robot_ + delta;

    constexpr auto is_box
        = [](auto c) { return c == box || c == left_box || c == right_box; };

    for (;; end_loc += delta) {
      if (!is_box(at(end_loc))) {
        break;
      }
    }

    if (at(end_loc) == wall) {
      return;
    }

    assert(at(end_loc) == floor);

    for (auto loc = end_loc; loc != robot_; loc -= delta) {
      at_unchecked(loc) = at(loc - delta);
    }

    at_unchecked(robot_) = floor;
    robot_ += delta;
  }

  void wide_apply(char move)
  {
    if (move == '<' || move == '>') {
      return apply(move);
    }

    auto delta = move_delta(move);

    auto moves = std::unordered_set<utils::point> {};
    auto queue = std::queue<utils::point> {};
    queue.push(robot_);

    while (!queue.empty()) {
      auto curr = queue.front();
      queue.pop();

      auto target = curr + delta;
      if (at(target) == wall) {
        return;
      }

      if (at(target) == left_box) {
        queue.push(target);
        queue.push(target + utils::point {1, 0});
      }

      if (at(target) == right_box) {
        queue.push(target);
        queue.push(target + utils::point {-1, 0});
      }

      moves.insert(curr);
    }

    auto ordered_moves = std::vector<utils::point>(moves.begin(), moves.end());
    std::ranges::sort(ordered_moves, [move](auto const& a, auto const& b) {
      if (move == '^') {
        return a.y < b.y;
      } else {
        assert(move == 'v');
        return a.y > b.y;
      }
    });

    for (auto const& m : ordered_moves) {
      at_unchecked(m + delta) = at(m);
      at_unchecked(m)         = floor;
    }

    at_unchecked(robot_) = floor;
    robot_ += delta;
  }

  template <typename Range>
  void apply(Range&& range)
    requires(std::is_same_v<std::ranges::range_value_t<Range>, char>)
  {
    for (auto c : range) {
      if constexpr (widened) {
        wide_apply(c);
      } else {
        apply(c);
      }
    }
  }

  std::int64_t gps_score() const noexcept
  {
    constexpr auto box_marker = widened ? left_box : box;

    return utils::sum(coords(), [this](auto p) {
      if (at(p) == box_marker) {
        return p.x + (p.y * 100);
      }

      return std::int64_t {0};
    });
  }

  template <bool>
  friend class warehouse;

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

template <bool widened>
std::int64_t run(warehouse<widened> map, std::vector<char> const& moves)
{
  map.apply(moves);
  return map.gps_score();
}

int main()
{
  auto map   = warehouse(utils::lines() | until_empty());
  auto moves = get_moves(utils::lines());

  fmt::print("{}\n", run(map, moves));
  fmt::print("{}\n", run(map.widen(), moves));
}
