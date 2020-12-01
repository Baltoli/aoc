#include <utils/utils.h>

#include <cassert>
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

struct pass {
  std::string wire;
  std::string to;
};

using instruction = std::variant<input, and_, or_, lshift, rshift, not_, pass>;

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
  void operator()(pass i);

private:
  void step();
  bool is_set(std::string const&);

  std::map<std::string, uint16_t> signals_;
  std::vector<instruction>        instrs_;
};

circuit::circuit(std::vector<instruction> is)
    : signals_ {{"1", 1}}
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

void circuit::operator()(pass i)
{
  if (is_set(i.wire)) {
    signals_[i.to] = signals_[i.wire];
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
  auto ret = std::vector<instruction> {};

  utils::for_each_line([&](auto line) {
    auto cs = utils::split(line, " ");

    if (cs[0] == "NOT") {
      ret.emplace_back(not_ {cs[1], cs[3]});
    } else if (cs.size() == 3) {
      if (std::isdigit(cs[0][0])) {
        ret.emplace_back(
            input {static_cast<uint16_t>(std::stoi(cs[0])), cs[2]});
      } else {
        ret.emplace_back(pass {cs[0], cs[2]});
      }
    } else if (cs[1] == "AND") {
      ret.emplace_back(and_ {cs[0], cs[2], cs[4]});
    } else if (cs[1] == "OR") {
      ret.emplace_back(or_ {cs[0], cs[2], cs[4]});
    } else if (cs[1] == "LSHIFT") {
      ret.emplace_back(
          lshift {cs[0], static_cast<uint16_t>(std::stoi(cs[2])), cs[4]});
    } else if (cs[1] == "RSHIFT") {
      ret.emplace_back(
          rshift {cs[0], static_cast<uint16_t>(std::stoi(cs[2])), cs[4]});
    } else {
      assert(false && "Invalid instruction");
    }
  });

  return ret;
}

int main()
{
  auto ins = get_input();

  auto circ  = circuit(ins);
  auto out_1 = circ.run("a");
  std::cout << out_1 << '\n';

  for (auto& var : ins) {
    if (std::holds_alternative<input>(var)) {
      std::get<input>(var).val = out_1;
    }
  }
  auto circ_2 = circuit(ins);
  std::cout << circ_2.run("a") << '\n';
}
