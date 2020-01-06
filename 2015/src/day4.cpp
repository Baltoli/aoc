#include <utils/utils.h>

#include <algorithm>

std::string input(int i) { return "iwrupvqb" + std::to_string(i); }

bool valid(int i, int len)
{
  auto hash = utils::md5_sum(input(i));
  return std::all_of(
      hash.begin(), hash.begin() + len, [](auto c) { return c == '0'; });
}

void task(int len)
{
  int i = 1;
  while (!valid(i, len)) {
    ++i;
  }

  std::cout << i << '\n';
}

int main()
{
  task(5);
  task(6);
}
