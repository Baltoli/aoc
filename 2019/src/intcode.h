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

  explicit computer(std::string const&, int);

  long run();
  void input(long);
  int  backlog() const;
  bool halted() const { return halted_; }

  long&       operator[](size_t);
  long const& operator[](size_t) const;

  decltype(auto) begin() const { return program_.begin(); }
  decltype(auto) end() const { return program_.end(); }

private:
  int               pc_       = 0;
  int               rel_base_ = 0;
  std::queue<long>  inputs_ {};
  std::vector<long> program_;
  bool              halted_ = false;
  int               buffer_;

  long& current_param(int idx);
};

} // namespace ic
