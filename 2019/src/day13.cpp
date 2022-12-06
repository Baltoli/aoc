#include <intcode.h>

#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <thread>

char pretty(long in)
{
  switch (in) {
  case 0: return ' ';
  case 1: return '#';
  case 2: return '@';
  case 3: return '~';
  case 4: return 'O';
  default: assert(false && "Bad tile");
  }
}

class arcade {
public:
  explicit arcade(std::string const&);
  arcade(std::string const&, std::vector<int> const&);

  void enable();
  void step();
  void run();
  void interact();
  int  score() const;

  void dump();
  int  num_blocks() const;

private:
  long score_  = 0;
  long paddle_ = 0;

  ic::computer                       computer_;
  std::map<std::pair<int, int>, int> screen_;
};

arcade::arcade(std::string const& prog)
    : computer_(prog, 1)
{
}

void arcade::enable() { computer_[0] = 2; }

void arcade::step()
{
  auto x     = computer_.run();
  auto y     = computer_.run();
  auto id    = computer_.run();
  auto coord = std::pair {x, y};

  if (computer_.halted()) {
    return;
  } else if (x == -1 && y == 0) {
    if (id > 0) {
      score_ = id;
    }
  } else {
    screen_[coord] = id;
  }
}

void arcade::run()
{
  while (!computer_.halted()) {
    step();
  }
}

void arcade::interact()
{
  while (!computer_.halted()) {
    step();
    if (computer_.backlog() == 0) {
      computer_.input(-1);
    }
  }
}

int arcade::score() const { return score_; }

int arcade::num_blocks() const
{
  return std::count_if(
      screen_.begin(), screen_.end(), [](auto p) { return p.second == 2; });
}

int main()
{
  std::string line;
  std::getline(std::cin, line);

  auto game = arcade(line);
  game.run();

  std::cout << game.num_blocks() << '\n';

  auto game_2 = arcade(line);
  game_2.enable();
  game_2.interact();

  std::cout << game_2.score() << '\n';
}
