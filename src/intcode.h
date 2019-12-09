#pragma once

#include <algorithm>
#include <functional>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

namespace ic {

void tests();

int opcode(int instr);
int pc_advance(int opcode);
int param_mode(int param, int instr);

class computer {
public:
  explicit computer(std::string const&);

  template <typename F>
  computer(std::string const&, F);

  template <typename Iterator>
  computer(Iterator begin, Iterator end);

  void run();
  void input(int);
  bool halted() const { return halted_; }

  int&       operator[](size_t);
  int const& operator[](size_t) const;

  decltype(auto) begin() const { return program_.begin(); }
  decltype(auto) end() const { return program_.end(); }

private:
  int              pc_ = 0;
  std::queue<int>  inputs_ {};
  std::vector<int> program_;
  bool             halted_ = false;

  std::function<void(int)> output_;

  int& current_param(int idx);
  void output(int);
};

template <typename F>
computer::computer(std::string const& code, F func)
    : program_ {}
    , output_(func)
{
  auto ptr = 0;

  while (true) {
    auto next_comma = code.find(",", ptr);
    auto str        = code.substr(ptr, next_comma - ptr);

    program_.push_back(std::stoi(str));

    if (next_comma == std::string::npos) {
      break;
    }

    ptr = next_comma + 1;
  }
}

template <typename Iterator>
computer::computer(Iterator begin, Iterator end)
    : program_(begin, end)
{
}

}
