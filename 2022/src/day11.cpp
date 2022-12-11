#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <functional>
#include <optional>
#include <queue>
#include <vector>

namespace {

static int add(int a, int b) { return a + b; }
static int mul(int a, int b) { return a * b; }

std::function<int(int, int)> get_op(std::string_view os)
{
  if (os == "+") {
    return add;
  } else if (os == "*") {
    return mul;
  } else {
    abort();
  }
}

std::optional<int> get_operand(std::string_view os)
{
  if (os == "old") {
    return std::nullopt;
  } else {
    return utils::svtoi(os);
  }
}

} // namespace

class monkey;

class game {
public:
  game()
      : monkeys_ {}
  {
  }

  void throw_to(int monkey, int worry);

private:
  std::vector<monkey> monkeys_;
};

class monkey {
public:
  monkey(std::vector<std::string>& input)
      : items_ {}
      , lhs_(std::nullopt)
      , rhs_(std::nullopt)
      , op_ {}
      , modulus_(0)
      , if_t_(0)
      , if_f_(0)
  {
    auto item_list = utils::map(
        utils::split(
            ctre::match<"  Starting items: (.*)">(input[1])
                .get<1>()
                .to_string(),
            ", "),
        utils::stoi);
    for (auto i : item_list) {
      items_.push(i);
    }

    auto op_str
        = ctre::match<"  Operation: new = (.*) (\\*|\\+) (.*)">(input[2]);
    lhs_ = get_operand(op_str.get<1>());
    op_  = get_op(op_str.get<2>());
    rhs_ = get_operand(op_str.get<3>());

    auto mod_str = ctre::match<"  Test: divisible by (.*)">(input[3]);
    modulus_     = utils::svtoi(mod_str.get<1>());

    auto true_str = ctre::match<"    If true: throw to monkey (.*)">(input[4]);
    if_t_         = utils::svtoi(mod_str.get<1>());

    auto false_str
        = ctre::match<"    If false: throw to monkey (.*)">(input[5]);
    if_f_ = utils::svtoi(mod_str.get<1>());

    assert(input.size() >= 7 && "Bad input");
    input.erase(input.begin(), input.begin() + 7);
  }

  void receive(int w) { items_.push(w); }

  void execute()
  {
    while (!items_.empty()) {
      auto [monkey, worry] = step();
      game_->throw_to(monkey, worry);
    }
  }

private:
  int apply(int worry) const
  {
    return op_(lhs_.value_or(worry), rhs_.value_or(worry));
  }

  std::pair<int, int> step()
  {
    auto worry = items_.front();
    items_.pop();

    worry = apply(worry);
    worry /= 3;

    auto monkey = (worry % modulus_ == 0) ? if_t_ : if_f_;

    return {monkey, worry};
  }

  std::queue<int>              items_;
  std::optional<int>           lhs_;
  std::optional<int>           rhs_;
  std::function<int(int, int)> op_;
  int                          modulus_;
  int                          if_t_;
  int                          if_f_;

  game* game_;
};

void game::throw_to(int monkey, int worry)
{
  monkeys_.at(monkey).receive(worry);
}

int main()
{
  auto input = utils::get_lines();
  while (!input.empty()) {
    auto m = monkey(input);
  }
}
