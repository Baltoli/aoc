#include <utils/utils.h>

#include <array>
#include <deque>
#include <iostream>
#include <vector>

struct deck {
  int score() const
  {
    auto sum = 0;
    for (auto i = 0; i < cards.size(); ++i) {
      sum += cards[i] * (cards.size() - i);
    }
    return sum;
  }

  std::deque<int> cards;
};

struct game {
  void step()
  {
    auto p1 = players[0].cards.front();
    auto p2 = players[1].cards.front();

    if (p1 > p2) {
      players[0].cards.push_back(p1);
      players[0].cards.push_back(p2);
    } else if (p2 > p1) {
      players[1].cards.push_back(p2);
      players[1].cards.push_back(p1);
    } else {
      assert(false && "Tie??");
    }

    for (auto& p : players) {
      p.cards.pop_front();
    }
  }

  bool complete() const
  {
    return std::any_of(players.begin(), players.end(), [](auto const& p) {
      return p.cards.empty();
    });
  }

  int winning_score() const
  {
    for (auto const& p : players) {
      if (!p.cards.empty()) {
        return p.score();
      }
    }

    return 0;
  }

  std::array<deck, 2> players;
};

game get_input()
{
  auto players = std::array {deck {}, deck {}};

  auto idx = 0;
  utils::for_each_line([&](auto const& line) {
    if (line.empty()) {
      ++idx;
      return;
    }

    if (line[0] == 'P') {
      return;
    }

    players[idx].cards.push_back(std::stoi(line));
  });

  return {players};
}

int main()
{
  auto g = get_input();
  while (!g.complete()) {
    g.step();
  }
  std::cout << g.winning_score() << '\n';
}
