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
  static constexpr auto mem_size = 4096;

public:
  explicit computer(std::string const&);

  template <typename F>
  computer(std::string const&, F);

  template <typename F>
  computer(std::string const&, F, bool);

  void run();
  void input(long);
  bool halted() const { return halted_; }

  long&       operator[](size_t);
  long const& operator[](size_t) const;

  decltype(auto) begin() const { return program_.begin(); }
  decltype(auto) end() const { return program_.end(); }

private:
  int               pc_       = 0;
  int               rel_base_ = 0;
  std::queue<long>  inputs_{};
  std::vector<long> program_;
  bool              halt_on_output_;
  bool              halted_ = false;

  std::function<void(long)> output_;

  long& current_param(int idx);
  void  output(long);
};

template <typename F>
computer::computer(std::string const& code, F func, bool hop)
    : program_(mem_size, 0L)
    , output_(func)
    , halt_on_output_(hop)
{
  auto ptr = 0;
  auto i   = 0;

  while (true) {
    auto next_comma = code.find(",", ptr);
    auto str        = code.substr(ptr, next_comma - ptr);

    program_[i++] = std::stol(str);

    if (next_comma == std::string::npos) {
      break;
    }

    ptr = next_comma + 1;
  }
}

template <typename F>
computer::computer(std::string const& code, F func)
    : computer(code, func, false)
{
}
}
