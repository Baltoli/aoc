#include <utils/utils.h>

#include <array>
#include <deque>
#include <iostream>
#include <unordered_set>
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

  bool operator==(deck const& other) const { return cards == other.cards; }

  std::deque<int> cards;
};

namespace std {
template <>
struct hash<deck> {
  size_t operator()(deck const& d) const
  {
    auto seed = size_t {0};
    for (auto c : d.cards) {
      utils::hash_combine(seed, c);
    }
    return seed;
  }
};

template <>
struct hash<std::pair<deck, deck>> {
  size_t operator()(std::pair<deck, deck> const& p) const
  {
    auto seed = size_t {0};
    utils::hash_combine(seed, p.first);
    utils::hash_combine(seed, p.second);
    return seed;
  }
};
} // namespace std

struct game {
  game(std::array<deck, 2> ps)
      : players(ps)
      , states {}
      , winner(-1)
  {
  }

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

  void recursive_step()
  {
    if (states.find({players[0], players[1]}) != states.end()) {
      winner = 0;
      return;
    }
    states.insert({players[0], players[1]});

    auto p1 = players[0].cards.front();
    auto p2 = players[1].cards.front();
    for (auto& p : players) {
      p.cards.pop_front();
    }

    if (p1 <= players[0].cards.size() && p2 <= players[1].cards.size()) {
      auto rec_p1 = deck {};
      for (auto i = 0; i < p1; ++i) {
        rec_p1.cards.push_back(players[0].cards[i]);
      }

      auto rec_p2 = deck {};
      for (auto i = 0; i < p2; ++i) {
        rec_p2.cards.push_back(players[1].cards[i]);
      }

      auto rec = game({rec_p1, rec_p2});
      rec.shortcut();
      while (!rec.complete()) {
        rec.recursive_step();
      }

      if (rec.winner == 0) {
        players[0].cards.push_back(p1);
        players[0].cards.push_back(p2);
      } else if (rec.winner == 1) {
        players[1].cards.push_back(p2);
        players[1].cards.push_back(p1);
      } else {
        assert(false && "Bad winner");
      }
    } else {
      if (p1 > p2) {
        players[0].cards.push_back(p1);
        players[0].cards.push_back(p2);
      } else if (p2 > p1) {
        players[1].cards.push_back(p2);
        players[1].cards.push_back(p1);
      } else {
        assert(false && "Tie??");
      }
    }
  }

  bool complete()
  {
    if (winner != -1) {
      return true;
    }

    for (auto i = 0; i < players.size(); ++i) {
      if (players[i].cards.empty()) {
        winner = 1 - i;
        return true;
      }
    }

    return false;
  }

  int winning_score()
  {
    if (complete()) {
      return players.at(winner).score();
    } else {
      return 0;
    }
  }

  void shortcut()
  {
    auto max_card = std::numeric_limits<int>::min();
    for (auto const& p : players) {
      for (auto const& c : p.cards) {
        if (c > max_card) {
          max_card = c;
        }
      }
    }

    if (std::find(players[0].cards.begin(), players[0].cards.end(), max_card)
        != players[0].cards.end()) {
      winner = 0;
    }
  }

  std::array<deck, 2>                       players;
  std::unordered_set<std::pair<deck, deck>> states;
  int                                       winner;
};

std::array<deck, 2> get_input()
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

  return players;
}

int main()
{
  auto decks = get_input();

  auto g = game(decks);
  while (!g.complete()) {
    g.step();
  }
  std::cout << g.winning_score() << '\n';

  auto r_g = game(decks);
  while (!r_g.complete()) {
    r_g.recursive_step();
  }
  std::cout << r_g.winning_score() << '\n';
}
