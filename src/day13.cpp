#include <intcode.h>

#include <iostream>
#include <map>
#include <string>
#include <thread>

char pretty(long in)
{
  switch (in) {
  case 0:
    return ' ';
  case 1:
    return '#';
  case 2:
    return '@';
  case 3:
    return '~';
  case 4:
    return 'O';
  default:
    assert(false && "Bad tile");
  }
}

class arcade {
public:
  explicit arcade(std::string const&);

  void enable();
  void step();
  void run();
  void interact();

  void dump();

  int num_blocks() const;

private:
  long ball_     = 0;
  long paddle_   = 0;
  long joystick_ = 0;
  long score_    = 0;

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
  computer_.input(joystick_);
  auto x     = computer_.run();
  auto y     = computer_.run();
  auto id    = computer_.run();
  auto coord = std::pair {x, y};

  if (computer_.halted()) {
    return;
  } else if (x == -1 && y == 0) {
    score_ = id;
  } else {
    screen_[coord] = id;

    if (id == 3) {
      paddle_ = x;
    }

    if (id == 4) {
      ball_ = x;
    }
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
  int i = 0;
  while (!computer_.halted()) {
    step();
    if (i++ > 880) {
      dump();

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
}

int arcade::num_blocks() const
{
  return std::count_if(
      screen_.begin(), screen_.end(), [](auto p) { return p.second == 2; });
}

void arcade::dump()
{
  auto width
      = std::max_element(screen_.begin(), screen_.end(), [](auto p1, auto p2) {
          return p1.first.first < p2.first.first;
        })->first.first;

  auto height
      = std::max_element(screen_.begin(), screen_.end(), [](auto p1, auto p2) {
          return p1.first.second < p2.first.second;
        })->first.second;

  std::cout << "Score: " << score_ << '\n';
  for (int row = 0; row <= height; ++row) {
    for (int col = 0; col <= width; ++col) {
      std::cout << pretty(screen_[{col, row}]);
    }
    std::cout << '\n';
  }
  std::cout << "\033[2J\033[H";
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
}
