#include <utils/utils.h>

#include <iostream>
#include <unordered_set>

int main()
{
  int  total   = 0;
  auto answers = std::unordered_set<char> {};

  utils::for_each_line([&](auto const& line) {
    if (line.empty()) {
      total += answers.size();
      answers.clear();
      return;
    }

    for (auto c : line) {
      answers.insert(c);
    }
  });

  std::cout << total << '\n';
}
