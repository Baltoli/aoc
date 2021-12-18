#include <utils/utils.h>

#include <iostream>
#include <limits>
#include <map>
#include <set>

int max_risk() { return std::numeric_limits<int>::max() - 100; }

struct point {
  point(int r)
      : risk(r)
      , distance(max_risk())
      , visited(false)
  {
  }

  int  risk;
  int  distance;
  bool visited;
};

struct map {
  std::vector<std::vector<int>>        data;
  std::map<std::pair<int, int>, point> points;

  point& at(int x, int y)
  {
    auto risk = x >= 0 && x < width() && y >= 0 && y < height()
                    ? data.at(y).at(x)
                    : max_risk();

    points.try_emplace({x, y}, risk);
    return points.at({x, y});
  }

  int width() const { return data[0].size(); }
  int height() const { return data[1].size(); }

  int compute_cost_to(int x, int y)
  {
    auto cur_x = 0;
    auto cur_y = 0;

    auto& initial    = at(0, 0);
    initial.distance = 0;

    // heap of unvisited {x, y} pairs
    // comparator looks up in at() set

    auto heap     = std::vector<std::pair<int, int>> {{x, y}};
    auto heap_cmp = [this](auto a, auto b) {
      return at(a.first, a.second).distance > at(b.first, b.second).distance;
    };

    while (cur_x != x || cur_y != y) {
      /* std::cout << "consider: " << cur_x << ' ' << cur_y << '\n'; */
      auto& current = at(cur_x, cur_y);

      for (auto dx = -1; dx <= 1; ++dx) {
        for (auto dy = -1; dy <= 1; ++dy) {
          auto& next = at(cur_x + dx, cur_y + dy);
          if ((dx != 0 || dy != 0) && !next.visited) {
            next.distance
                = std::min(next.distance, current.distance + next.risk);
            // push unvisited neighbour nto heap

            heap.push_back({cur_x + dx, cur_y + dy});
            std::push_heap(heap.begin(), heap.end(), heap_cmp);
          }
        }
      }

      current.visited = true;

      // get x, y here from front of heap
      std::pop_heap(heap.begin(), heap.end(), heap_cmp);
      auto top = heap.back();
      heap.pop_back();

      cur_x = top.first;
      cur_y = top.second;
    }

    return at(x, y).distance;
  }

  static map load()
  {
    auto m = map {};
    utils::for_each_line([&](auto const& line) {
      auto v = std::vector<int> {};
      for (auto c : line) {
        v.push_back(utils::ctoi(c));
      }
      m.data.push_back(v);
    });
    return m;
  }
};

int part_1(map m) { return m.compute_cost_to(m.width() - 1, m.height() - 1); }

int main()
{
  auto m = map::load();

  std::cout << part_1(m) << '\n';
}
