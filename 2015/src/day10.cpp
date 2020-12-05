#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

std::string look_and_say(std::string_view in)
{
  auto ss = std::stringstream {};

  for (auto i = 0; i < in.size();) {
    auto c     = in[i];
    auto count = 0;

    while (in[i] == c) {
      ++count;
      ++i;
    }

    ss << count << c;
  }

  return ss.str();
}

int main()
{
  auto in = std::string {};
  std::getline(std::cin, in);

  for (auto i = 0; i < 50; ++i) {
    if (i == 40) {
      std::cout << in.size() << '\n';
    }

    in = look_and_say(in);
  }

  std::cout << in.size() << '\n';
}
