#include <utils/utils.h>

#include <iostream>
#include <map>
#include <variant>

struct input {
  uint16_t    val;
  std::string to;
};

struct and_ {
  std::string left;
  std::string right;
  std::string to;
};

struct or_ {
  std::string left;
  std::string right;
  std::string to;
};

struct lshift {
  std::string wire;
  uint16_t    val;
  std::string to;
};

struct rshift {
  std::string wire;
  uint16_t    val;
  std::string to;
};

struct not_ {
  std::string wire;
  std::string to;
};

using instruction = std::variant<input, and_, or_, lshift, rshift, not_>;

class circuit {
public:
  circuit(std::vector<instruction>);

  uint16_t run(std::string);

  void operator()(input i);
  void operator()(and_ i);
  void operator()(or_ i);
  void operator()(lshift i);
  void operator()(rshift i);
  void operator()(not_ i);

private:
  void step();
  bool is_set(std::string const&);

  std::map<std::string, uint16_t> signals_;
  std::vector<instruction>        instrs_;
};

circuit::circuit(std::vector<instruction> is)
    : signals_{}
    , instrs_(is)
{
}

bool circuit::is_set(std::string const& w)
{
  return signals_.find(w) != signals_.end();
}

void circuit::operator()(input i) { signals_[i.to] = i.val; }

void circuit::operator()(and_ i)
{
  if (is_set(i.left) && is_set(i.right)) {
    signals_[i.to] = signals_[i.left] & signals_[i.right];
  }
}

void circuit::operator()(or_ i)
{
  if (is_set(i.left) && is_set(i.right)) {
    signals_[i.to] = signals_[i.left] | signals_[i.right];
  }
}

void circuit::operator()(lshift i)
{
  if (is_set(i.wire)) {
    signals_[i.to] = signals_[i.wire] << i.val;
  }
}

void circuit::operator()(rshift i)
{
  if (is_set(i.wire)) {
    signals_[i.to] = signals_[i.wire] >> i.val;
  }
}

void circuit::operator()(not_ i)
{
  if (is_set(i.wire)) {
    signals_[i.to] = ~signals_[i.wire];
  }
}

uint16_t circuit::run(std::string w)
{
  while (signals_.find(w) == signals_.end()) {
    step();
  }

  return signals_[w];
}

void circuit::step()
{
  for (auto i : instrs_) {
    std::visit(*this, i);
  }
}

std::vector<instruction> get_input()
{
  auto ret = std::vector<instruction>{};

  utils::for_each_line([&](auto line) {});

  return ret;
}

int main()
{
  auto circ = circuit(get_input());
  std::cout << circ.run("a") << '\n';
}
