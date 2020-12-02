#include <utils/utils.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>

int main()
{
  int n_correct = 0;

  utils::for_each_line([&](auto const& line) {
    auto sp   = utils::split(line, ":");
    auto pass = sp[1];

    auto pat   = utils::split(sp[0], " ");
    auto range = utils::split(pat[0], "-");
    auto chr   = pat[1][0];

    assert(pat[1].size() == 1);

    auto min = std::stoi(range[0]);
    auto max = std::stoi(range[1]);

    assert(min <= max);

    auto count = std::count(pass.begin(), pass.end(), chr);
    if (count >= min && count <= max) {
      ++n_correct;
    }
  });

  std::cout << n_correct << '\n';
}
