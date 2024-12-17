#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

using namespace ctre::literals;

constexpr auto reg_pattern = "Register [ABC]: (\\d+)"_ctre;
static_assert(reg_pattern.match("Register A: 87234"));
static_assert(reg_pattern.match("Register B: 0"));

constexpr auto prog_pattern = "Program: ([0-7,]*)"_ctre;
static_assert(prog_pattern.match("Program: 3,2,1,4,5,6"));

enum opcode : std::uint8_t {
  adv = 0,
  bxl = 1,
  bst = 2,
  jnz = 3,
  bxc = 4,
  out = 5,
  bdv = 6,
  cdv = 7,
};

enum reg : std::uint8_t {
  A = 0,
  B = 1,
  C = 2,
};

class machine {
public:
  using inst_t = std::underlying_type_t<opcode>;

  template <std::ranges::range Range>
  machine(Range&& r)
      : pc_ {0}
  {
    auto current_reg = 0;

    for (auto&& line : std::forward<Range>(r)) {
      if (auto [m, val] = reg_pattern.match(line); m) {
        regs_[current_reg++] = utils::to_int<std::int64_t>(val);
      }

      if (auto [m, prog] = prog_pattern.match(line); m) {
        program_ = utils::map(
            utils::split(prog.to_string(), ","),
            [](auto const& str) { return utils::to_int<inst_t>(str); });
      }
    }

    initial_regs_ = regs_;
  }

  std::int64_t combo_op(inst_t op) const
  {
    assert(op <= 6);

    if (op < 4) {
      return op;
    }

    return regs_[op - 4];
  }

  void div(reg result, inst_t data)
  {
    auto num      = regs_[A];
    auto den      = utils::pow(2, combo_op(data));
    regs_[result] = num / den;
  }

  void dispatch(inst_t op, inst_t data)
  {
    auto jumped = false;

    switch (op) {
    case adv: div(A, data); break;
    case bxl: regs_[B] = regs_[B] ^ data; break;
    case bst: regs_[B] = combo_op(data) % 8; break;
    case jnz:
      if (regs_[A] != 0) {
        pc_    = data;
        jumped = true;
      }
      break;
    case bxc: regs_[B] = regs_[B] ^ regs_[C]; break;
    case out: output_.push_back(combo_op(data) % 8); break;
    case bdv: div(B, data); break;
    case cdv: div(C, data); break;
    default: std::unreachable();
    }

    if (!jumped) {
      pc_ += 2;
    }
  }

  void run(std::int64_t a)
  {
    output_.clear();
    regs_    = initial_regs_;
    regs_[A] = a;
    pc_      = 0;

    while ((pc_ + 1) < program_.size()) {
      dispatch(program_[pc_], program_[pc_ + 1]);
    }
  }

  void run() { run(initial_regs_[A]); }

  std::string get_output() const
  {
    return fmt::format("{}", fmt::join(output_, ","));
  }

  std::int64_t lowest_quine()
  {
    for (auto a = 0;; ++a) {
      run(a);
      if (output_ == program_) {
        return a;
      }
    }
  }

private:
  std::array<std::int64_t, 3> regs_;
  std::array<std::int64_t, 3> initial_regs_;
  std::vector<inst_t>         program_;
  std::size_t                 pc_;
  std::vector<std::uint8_t>   output_;
};

int main()
{
  auto in = machine(utils::lines());
  in.run();
  fmt::print("{}\n", in.get_output());
  fmt::print("{}\n", in.lowest_quine());
}
