#include <utils/utils.h>

#include <iostream>
#include <numeric>

class game {
public:
  game(char o, char p)
      : opponent_(o)
      , player_(p)
  {
  }

  game(std::string_view line)
      : game(line[0], line[2])
  {
  }

  long score() const { return player_score() + game_score(); }

  game correct_game() const { return game(opponent_, correct_move()); }

private:
  long player_score() const
  {
    switch (player_) {
    case 'X': return 1;
    case 'Y': return 2;
    case 'Z': return 3;
    default: abort();
    }
  }

  long game_score() const
  {
    if (opponent_ == 'A') {
      switch (player_) {
      case 'X': return 3;
      case 'Y': return 6;
      case 'Z': return 0;
      default: abort();
      }
    } else if (opponent_ == 'B') {
      switch (player_) {
      case 'X': return 0;
      case 'Y': return 3;
      case 'Z': return 6;
      default: abort();
      }
    } else if (opponent_ == 'C') {
      switch (player_) {
      case 'X': return 6;
      case 'Y': return 0;
      case 'Z': return 3;
      default: abort();
      }
    } else {
      abort();
    }
  }

  char correct_move() const
  {
    if (player_ == 'X') {
      switch (opponent_) {
      case 'A': return 'Z';
      case 'B': return 'X';
      case 'C': return 'Y';
      default: abort();
      }
    } else if (player_ == 'Y') {
      switch (opponent_) {
      case 'A': return 'X';
      case 'B': return 'Y';
      case 'C': return 'Z';
      default: abort();
      }
    } else if (player_ == 'Z') {
      switch (opponent_) {
      case 'A': return 'Y';
      case 'B': return 'Z';
      case 'C': return 'X';
      default: abort();
      }
    } else {
      abort();
    }
  }

  char opponent_;
  char player_;
};

long total_score(std::vector<game> const& games)
{
  return std::accumulate(
      games.begin(), games.end(), 0L,
      [](auto acc, auto const& game) { return acc + game.score(); });
}

int main()
{
  auto games = utils::construct_lines<game>();
  auto correct_games
      = utils::map(games, [](auto const& g) { return g.correct_game(); });

  std::cout << total_score(games) << '\n';
  std::cout << total_score(correct_games) << '\n';
}
