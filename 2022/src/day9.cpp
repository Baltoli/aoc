#include <utils/utils.h>

#include <fmt/format.h>

#include <set>

int main()
{
  auto head = std::pair<int, int> {0, 0};
  auto tail = std::pair<int, int> {0, 0};

  auto visited = std::set<std::pair<int, int>> {tail};

  utils::for_each_line([&](auto const& line) {
    auto split = utils::split(line, " ");

    auto dir = split[0];
    auto n   = utils::stoi(split[1]);

    for (auto i = 0; i < n; ++i) {
      // ...
    }
  });

  std::cout << visited.size() << '\n';
}
