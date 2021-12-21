#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>
#include <unordered_map>
#include <vector>

template <typename Int>
Int wrap_add(Int in, Int b, Int limit)
{
  in += (b - 1);
  in = (in % (limit));
  in += 1;
  return in;
}

struct player {
  long position;
  long score;

  player(std::string_view line)
      : score(0)
  {
    auto m   = ctre::match<R"(Player (\d+) starting position: (\d+))">(line);
    position = utils::stoi(m.get<2>().str());
  }

  void move(long roll) { position = wrap_add(position, roll, 10L); }

  void score_points() { score += position; }
};

bool operator==(player const& a, player const& b)
{
  return std::tie(a.position, a.score) == std::tie(b.position, b.score);
}

struct key {
  player p0;
  player p1;
  bool   turn;
};

namespace std {
template <>
struct hash<key> {
  size_t operator()(key const& k) const
  {
    size_t seed {0};
    utils::hash_combine(
        seed, k.p0.position, k.p0.score, k.p1.position, k.p1.score, k.turn);
    return seed;
  }
};
} // namespace std

bool operator==(key const& a, key const& b)
{
  return std::tie(a.p0, a.p1, a.turn) == std::tie(b.p0, b.p1, b.turn);
}

template <typename Dice>
struct game {
  game(std::vector<player> ps)
      : dice()
      , players(ps)
  {
  }

  long play()
  {
    long total_rolls = 0;
    while (true) {
      for (auto i = 0; i < 3; ++i) {
        auto d = dice.roll();
        players[player].move(d);
        total_rolls++;
      }
      players[player].score_points();

      if (players[player].score >= 1000) {
        return total_rolls * players[!player].score;
      }

      player = !player;
    }
  }

  std::unordered_map<key, std::pair<long, long>> cache = {};

  std::pair<long, long> dirac_recursive(player p0, player p1, bool player)
  {
    static std::unordered_map<long, long> dice_table
        = {{3, 1L}, {4, 3L}, {5, 6L}, {6, 7L}, {7, 6L}, {8, 3L}, {9, 1L}};

    auto k = key {p0, p1, player};

    if (!cache.contains(k)) {
      if (p0.score >= 21) {
        cache[k] = {1L, 0L};
      } else if (p1.score >= 21) {
        cache[k] = {0L, 1L};
      } else {
        long p0_wins = 0L;
        long p1_wins = 0L;

        for (auto roll = 3; roll <= 9; ++roll) {
          if (player == 0) {
            auto p0_c = p0;
            p0_c.move(roll);
            p0_c.score_points();

            auto [raw_p0, raw_p1] = dirac_recursive(p0_c, p1, !player);
            p0_wins += dice_table[roll] * raw_p0;
            p1_wins += dice_table[roll] * raw_p1;
          } else {
            auto p1_c = p1;
            p1_c.move(roll);
            p1_c.score_points();

            auto [raw_p0, raw_p1] = dirac_recursive(p0, p1_c, !player);
            p0_wins += dice_table[roll] * raw_p0;
            p1_wins += dice_table[roll] * raw_p1;
          }
        }
        cache[k] = {p0_wins, p1_wins};
      }
    }

    return cache.at(k);
  }

  long dirac()
  {
    auto [p0, p1] = dirac_recursive(players[0], players[1], 0);
    return p0 > p1 ? p0 : p1;
  }

  Dice                dice;
  std::vector<player> players;
  long                player = 0;
};

struct deterministic {
  long state = 1;

  deterministic() = default;

  long roll()
  {
    long old = state;
    state    = wrap_add(state, 1L, 100L);
    return old;
  }
};

std::vector<player> load()
{
  return utils::map_lines([](auto const& line) { return player(line); });
}

int main()
{
  auto in = load();
  auto g  = game<deterministic>(in);
  auto g2 = g;

  std::cout << g.play() << '\n';
  std::cout << g2.dirac() << '\n';
}
