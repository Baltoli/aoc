#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <set>

int main()
{
  auto head = std::pair<int, int> {0, 0};
  auto tail = std::pair<int, int> {0, 0};

  auto visited = std::set<std::pair<int, int>> {tail};

  utils::for_each_line([&](auto const& line) {
    auto split = utils::split(line, " ");

    auto dir = split[0][0];
    auto n   = utils::stoi(split[1]);

    for (auto i = 0; i < n; ++i) {
      switch (dir) {
      case 'R': head.first++; break;
      case 'L': head.first--; break;
      case 'U': head.second++; break;
      case 'D': head.second--; break;
      }

      if (std::abs(head.first - tail.first) == 2) {
        tail.first += utils::sgn(head.first - tail.first);
        if (std::abs(head.second - tail.second) == 1) {
          tail.second = head.second;
        }
      } else if (std::abs(head.second - tail.second) == 2) {
        tail.second += utils::sgn(head.second - tail.second);
        if (std::abs(head.first - tail.first) == 1) {
          tail.first = head.first;
        }
      }

      fmt::print(
          "H={},{}  T={},{}\n", head.first, head.second, tail.first,
          tail.second);

      visited.insert(tail);
    }

    fmt::print("\n");
  });

  std::cout << visited.size() << '\n';
}
