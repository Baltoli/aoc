#include <utils/grid.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <functional>
#include <queue>
#include <unordered_set>

using vertex    = std::pair<utils::point, utils::point>;
using base_grid = utils::grid<utils::no_pad>;

class maze : public base_grid {
public:
  static constexpr char wall  = '#';
  static constexpr char floor = '.';
  static constexpr char start = 'S';
  static constexpr char end   = 'E';

  static constexpr utils::point east = {1, 0};

  template <typename... Args>
  maze(Args&&... args)
      : base_grid(std::forward<Args>(args)...)
  {
    start_ = utils::unwrap(find(start));
    end_   = utils::unwrap(find(end));

    at_unchecked(start_) = floor;
    at_unchecked(end_)   = floor;
  }

  std::generator<std::tuple<vertex, std::int64_t>>
  neighbours(vertex const& v) const
  {
    if (at(v.first + v.second) == floor) {
      co_yield {{v.first + v.second, v.second}, 1};
    }

    co_yield {{v.first, v.second.right_turn()}, 1000};
    co_yield {{v.first, v.second.left_turn()}, 1000};
  }

  std::pair<std::int64_t, std::int64_t> solve() const
  {
    auto state = std::unordered_map<
        vertex, std::pair<std::int64_t, std::unordered_set<vertex>>> {};

    for (auto p : coords()) {
      if (at(p) == floor) {
        for (auto d : ortho_neighbour_deltas()) {
          state[{p, d}].first = std::numeric_limits<std::int64_t>::max();
        }
      }
    }
    state[{start_, east}].first = 0;

    auto queue = std::queue<vertex> {};
    queue.push(vertex {start_, east});

    while (!queue.empty()) {
      auto curr = queue.front();
      queue.pop();

      for (auto [n, cost] : neighbours(curr)) {
        auto alt_cost = state.at(curr).first + cost;
        if (alt_cost < state.at(n).first) {
          state[n].first  = alt_cost;
          state[n].second = {curr};
          queue.push(n);
        } else if (alt_cost == state.at(n).first) {
          state[n].second.insert(curr);
        }
      }
    }

    auto readback_starts = std::unordered_set<vertex> {};
    auto best_score      = std::numeric_limits<std::int64_t>::max();
    for (auto const& [v, s] : state) {
      if (v.first == end_) {
        if (s.first < best_score) {
          best_score      = s.first;
          readback_starts = {v};
        } else if (s.first == best_score) {
          readback_starts.insert(v);
        }
      }
    }

    auto seats = std::unordered_set<utils::point> {};
    auto readback_queue
        = std::queue<vertex>(readback_starts.begin(), readback_starts.end());

    while (!readback_queue.empty()) {
      auto curr = readback_queue.front();
      readback_queue.pop();

      seats.insert(curr.first);

      for (auto const& pred : state.at(curr).second) {
        readback_queue.push(pred);
      }
    }

    return {best_score, seats.size()};
  }

private:
  utils::point start_;
  utils::point end_;
};

int main()
{
  auto in               = maze(utils::lines());
  auto [part_1, part_2] = in.solve();

  fmt::print("{}\n", part_1);
  fmt::print("{}\n", part_2);
}
