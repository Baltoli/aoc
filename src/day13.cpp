#include <intcode.h>

#include <iostream>
#include <map>
#include <string>

class arcade {
public:
  explicit arcade(std::string const&);

  void step();
  void run();

  int num_blocks() const;

private:
  ic::computer                       computer_;
  std::map<std::pair<int, int>, int> screen_;
};

arcade::arcade(std::string const& prog)
    : computer_(prog)
{
}

void arcade::step()
{
  auto x         = computer_.run();
  auto y         = computer_.run();
  auto id        = computer_.run();
  auto coord     = std::pair {x, y};
  screen_[coord] = id;
}

void arcade::run()
{
  while (!computer_.halted()) {
    step();
  }
}

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
}
