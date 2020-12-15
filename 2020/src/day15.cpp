#include <utils/utils.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<int> get_input()
{
  auto in = std::string {};
  std::getline(std::cin, in);

  auto split = utils::split(in, ",");
  auto ret   = std::vector<int> {};
  for (auto i : split) {
    ret.push_back(std::stoi(i));
  }
  return ret;
}

int nth_spoken(std::vector<int> const& start, int n)
{
  auto state = std::unordered_map<int, std::pair<int, int>> {};
  auto last  = 0;

  for (auto turn = 0; turn < n; ++turn) {
    if (turn < start.size()) {
      state[start[turn]] = {turn, turn};
      last               = start[turn];
    } else {
      auto [last_occ, before_last_occ] = state.at(last);
      auto next                        = last_occ - before_last_occ;

      if (state.find(next) == state.end()) {
        state[next] = {turn, turn};
      } else {
        auto [p, pp] = state[next];
        state[next]  = {turn, p};
      }

      last = next;
    }
  }

  return last;
}

int main()
{
  auto in = get_input();
  std::cout << nth_spoken(in, 2020) << '\n';
  std::cout << nth_spoken(in, 30000000) << '\n';
}
