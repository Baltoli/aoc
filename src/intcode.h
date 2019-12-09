#pragma once

#include <algorithm>
#include <queue>
#include <string>
#include <vector>

namespace ic {

void tests();

int param_mode(int param, int instr);

class computer {
public:
  explicit computer(std::string const&);

  template <typename Iterator>
  computer(Iterator begin, Iterator end);

  void input(int);

  int&       operator[](size_t);
  int const& operator[](size_t) const;

  decltype(auto) begin() const { return program_.begin(); }
  decltype(auto) end() const { return program_.end(); }

private:
  int              pc_ = 0;
  std::queue<int>  inputs_ {};
  std::vector<int> program_;

  void output(int);
};

template <typename Iterator>
computer::computer(Iterator begin, Iterator end)
    : program_(begin, end)
{
}

}
