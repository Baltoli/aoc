#include <intcode.h>

#include <array>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <thread>

static bool do_dump = false;

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
  arcade(std::string const&, std::vector<int>);

  void enable();
  void step();
  void run();
  void interact();
  int  score() const;

  void dump();
  auto inputs() const { return inputs_; }
  auto missed_by() const { return missed_; }

  int num_blocks() const;

  long paddle_ = 0;
  long used_   = 0;

private:
  long ball_     = 0;
  long ball_y_   = 0;
  long paddle_y_ = 0;
  long missed_   = 0;
  long joystick_ = 0;
  long score_    = 0;

  std::vector<int>                   inputs_;
  ic::computer                       computer_;
  std::map<std::pair<int, int>, int> screen_;
};

arcade::arcade(std::string const& prog, std::vector<int> ins)
    : computer_(prog, 1)
    , inputs_(ins)
{
}

arcade::arcade(std::string const& prog)
    : arcade(prog, {})
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

    if (id == 3) {
      paddle_   = x;
      paddle_y_ = y;
    }

    if (id == 4) {
      ball_   = x;
      ball_y_ = y;
    }

    if (ball_y_ != 0 && paddle_y_ != 0 && ball_y_ == paddle_y_) {
      missed_ = ball_ - paddle_;
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
    if (i++ >= 880) {
      if (computer_.backlog() == 0) {
        if (used_ == inputs_.size()) {
          inputs_.push_back(0);
        }
        computer_.input(inputs_[used_++]);
      }

      if (do_dump) {
        dump();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
      }
    }
    step();
  }
}

int arcade::score() const { return score_; }

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

  std::cout << "Score: " << score_ << ' ' << '\n';
  for (int row = 0; row <= height; ++row) {
    for (int col = 0; col <= width; ++col) {
      std::cout << pretty(screen_[{col, row}]);
    }
    std::cout << '\n';
  }
  std::cout << "\033[2J\033[H";
}

int tie_break() { return (std::rand() % 3) - 1; }

int main()
{
  std::string line;
  std::getline(std::cin, line);

  auto game = arcade(line);
  game.run();

  std::cout << game.num_blocks() << '\n';

  auto inputs    = std::vector<int> {};
  auto cycle     = std::array {-1, 0, 1};
  auto block_cyc = std::array {-1, 0, 1};
  auto cyc_i     = 0;

  while (true) {
    auto breaker = 0;
    auto game_2  = arcade(line, inputs);
    game_2.enable();
    game_2.interact();

    if (game_2.num_blocks() == 0) {
      std::cout << "Winner: " << game_2.score() << '\n';
      break;
    } else {
      std::cout << game_2.num_blocks() << " : " << game_2.score() << " ("
                << game_2.paddle_ << ")"
                << "\t[" << game_2.used_ << "]" << '\t' << game_2.missed_by()
                << '\n';
    }

    cycle[cyc_i]     = game_2.paddle_;
    block_cyc[cyc_i] = game_2.num_blocks();

    inputs = game_2.inputs();

    std::map<int, int> breaks = {{3315, 0}, {3475, -1}, {3513, -1}};

    if (breaks.find(game_2.used_) != breaks.end()) {
      breaker = breaks[game_2.used_];
    } else if (cycle[2] == cycle[0]) {
      breaker = 1;
    }

    for (auto i = 0; i < std::abs(game_2.missed_by()) + breaker; ++i) {
      if (game_2.missed_by() < 0) {
        inputs[inputs.size() - (3 + i)] = -1;
      } else {
        inputs[inputs.size() - (3 + i)] = 1;
      }
    }

    cyc_i = (cyc_i + 1) % 3;

    /* std::this_thread::sleep_for(std::chrono::milliseconds(1000)); */
  }
}
