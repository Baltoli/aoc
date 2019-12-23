#include <intcode.h>

#include <iostream>
#include <string>
#include <vector>

class droid {
public:
  droid(std::string const&);

  void exec(std::string in);

  int walk();
  int run();

private:
  int compile(std::string const& mode);

  ic::computer             computer_;
  std::vector<std::string> instrs_;
};

droid::droid(std::string const& prog)
    : computer_(prog)
    , instrs_ {}
{
}

void droid::exec(std::string in) { instrs_.push_back(in); }

int droid::compile(std::string const& mode)
{
  for (auto const& inst : instrs_) {
    for (auto c : inst) {
      computer_.input(c);
    }
    computer_.input('\n');
  }

  for (auto c : mode) {
    computer_.input(c);
  }
  computer_.input('\n');

  while (true) {
    auto c = computer_.run();
    if (computer_.halted()) {
      return 0;
    }

    if (c < 255) {
      std::cout << (char)c;
    } else {
      return c;
    }
  }
}

int droid::walk() { return compile("WALK"); }
int droid::run() { return compile("RUN"); }

void part_1(std::string const& line)
{
  auto d = droid(line);

  d.exec("NOT C T");
  d.exec("AND D T");
  d.exec("NOT A J");
  d.exec("OR T J");

  auto out = d.walk();
  std::cout << out << '\n';
}

void part_2(std::string const& line)
{
  auto d = droid(line);

  // ~C D | ~A

  d.exec("NOT C T");
  d.exec("AND D T");
  d.exec("NOT A J");
  d.exec("OR T J");

  auto out = d.run();
  std::cout << out << '\n';
}

int main()
{
  std::string line;
  std::getline(std::cin, line);

  part_1(line);
  part_2(line);
}
