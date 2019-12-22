#include <intcode.h>

#include <iostream>
#include <string>

class beam {
public:
  beam(std::string const&);

  int test(int x, int y) const;

private:
  ic::computer computer_;
};

beam::beam(std::string const& prog)
    : computer_(prog)
{
}

int beam::test(int x, int y) const
{
  auto new_c = computer_;
  new_c.input(x);
  new_c.input(y);
  return new_c.run();
}

void part_1(beam& b)
{
  auto total = 0;
  for (int x = 0; x < 50; ++x) {
    for (int y = 0; y < 50; ++y) {
      total += b.test(x, y);
    }
  }
  std::cout << total << '\n';
}

void part_2(beam& b)
{
  ;
  ;
}

int main()
{
  std::string line;
  std::getline(std::cin, line);
  auto b = beam(line);

  part_1(b);
  part_2(b);
}
