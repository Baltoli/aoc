#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <functional>
#include <optional>
#include <queue>
#include <vector>

namespace {

static long add(long a, long b) { return a + b; }
static long mul(long a, long b) { return a * b; }

std::function<long(long, long)> get_op(std::string_view os)
{
  if (os == "+") {
    return add;
  } else if (os == "*") {
    return mul;
  } else {
    abort();
  }
}

std::optional<long> get_operand(std::string_view os)
{
  if (os == "old") {
    return std::nullopt;
  } else {
    return utils::svtol(os);
  }
}

} // namespace

class monkey;

class game {
public:
  game()
      : field_(1)
      , monkeys_ {}
  {
  }

  void add(monkey m);

  void throw_to(int monkey, long worry);

  void play_rounds(long, bool);

  long score() const;

  long field() const { return field_; }

private:
  long                field_;
  std::vector<monkey> monkeys_;
};

class monkey {
public:
  monkey(std::vector<std::string>& input)
      : count_(0)
      , items_ {}
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
    if_t_         = utils::svtoi(true_str.get<1>());

    auto false_str
        = ctre::match<"    If false: throw to monkey (.*)">(input[5]);
    if_f_ = utils::svtoi(false_str.get<1>());

    assert(input.size() >= 7 && "Bad input");
    input.erase(input.begin(), input.begin() + 7);
  }

  void receive(long w) { items_.push(w); }

  void execute(bool divide)
  {
    while (!items_.empty()) {
      auto [monkey, worry] = step(divide);
      game_->throw_to(monkey, worry);
    }
  }

  long factor() const { return modulus_; }

  long count() const { return count_; }

  void set_game(game& g) { game_ = &g; }

private:
  long apply(long worry) const
  {
    return op_(lhs_.value_or(worry), rhs_.value_or(worry));
  }

  std::pair<int, long> step(bool divide)
  {
    count_++;

    auto worry = items_.front();
    items_.pop();

    worry = apply(worry);
    if (divide) {
      worry /= 3;
    }

    worry = worry % game_->field();

    auto monkey = (worry % modulus_ == 0) ? if_t_ : if_f_;

    return {monkey, worry};
  }

  long count_;

  std::queue<long>                items_;
  std::optional<long>             lhs_;
  std::optional<long>             rhs_;
  std::function<long(long, long)> op_;
  long                            modulus_;
  int                             if_t_;
  int                             if_f_;

  game* game_;
};

void game::add(monkey m)
{
  monkeys_.push_back(m);
  monkeys_.back().set_game(*this);
  field_ *= m.factor();
}

void game::throw_to(int monkey, long worry)
{
  monkeys_.at(monkey).receive(worry);
}

void game::play_rounds(long n, bool divide)
{
  for (auto i = 0; i < n; ++i) {
    for (auto& m : monkeys_) {
      m.execute(divide);
    }
  }
}

long game::score() const
{
  auto scores = std::vector<long> {};

  for (auto m : monkeys_) {
    scores.push_back(m.count());
  }

  std::sort(scores.begin(), scores.end(), std::greater<long>());

  return scores.at(0) * scores.at(1);
}

int main()
{
  auto input = utils::get_lines();
  auto g     = game();
  auto g2    = game();

  while (!input.empty()) {
    auto m = monkey(input);
    g.add(m);
    g2.add(m);
  }

  g.play_rounds(20, true);
  fmt::print("{}\n", g.score());

  g2.play_rounds(10000, false);
  fmt::print("{}\n", g2.score());
}
