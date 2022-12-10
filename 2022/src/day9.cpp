#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <set>

struct move {
  char dir;
  int  dist;

  move(std::string_view sv)
  {
    if (auto m = ctre::match<"([RLUD]) (\\d+)">(sv)) {
      dir  = m.template get<1>().to_view()[0];
      dist = utils::svtoi(m.template get<2>());
    }
  }
};

class snake {
public:
  using point = std::pair<int, int>;

  snake(int n)
      : links_(n, {0, 0})
      , tail_visited_ {links_.back()}
  {
  }

  void apply(move m)
  {
    auto& global_head = links_.at(0);

    for (auto i = 0; i < m.dist; ++i) {
      switch (m.dir) {
      case 'R': global_head.first++; break;
      case 'L': global_head.first--; break;
      case 'U': global_head.second++; break;
      case 'D': global_head.second--; break;
      }

      for (auto tail_idx = 1U; tail_idx < links_.size(); ++tail_idx) {
        auto& head = links_.at(tail_idx - 1);
        auto& tail = links_.at(tail_idx);

        if (std::abs(head.first - tail.first) == 2) {
          tail.first += utils::sgn(head.first - tail.first);
          if (std::abs(head.second - tail.second) > 0) {
            tail.second += utils::sgn(head.second - tail.second);
          }
        } else if (std::abs(head.second - tail.second) == 2) {
          tail.second += utils::sgn(head.second - tail.second);
          if (std::abs(head.first - tail.first) > 0) {
            tail.first += utils::sgn(head.first - tail.first);
          }
        }
      }

      tail_visited_.insert(links_.back());
    }
  }

  auto tail_count() const { return tail_visited_.size(); }

private:
  std::vector<point> links_;
  std::set<point>    tail_visited_;
};

int main()
{
  auto moves = utils::construct_lines<move>();

  auto part_1 = snake(2);
  auto part_2 = snake(10);

  for (auto& s : std::array {part_1, part_2}) {
    for (auto m : moves) {
      s.apply(m);
    }

    fmt::print("{}\n", s.tail_count());
  }
}
