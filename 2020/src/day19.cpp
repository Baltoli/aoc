#include <utils/utils.h>

#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

struct result {
  bool             success;
  std::string_view rest;
};

struct node {
  virtual ~node() = default;

  virtual result match(std::string_view) const = 0;
};
using regex = std::shared_ptr<node>;

struct character final : public node {
  character(char ch)
      : c(ch)
  {
  }

  result match(std::string_view sv) const override
  {
    if (sv.length() > 0 && sv[0] == c) {
      return {true, sv.substr(1)};
    } else {
      return {false, sv};
    }
  }

  char c;
};

struct seq final : public node {
  seq(std::vector<regex> rs)
      : parts(rs)
  {
  }

  result match(std::string_view sv) const override
  {
    auto ret = result {true, sv};
    for (auto re : parts) {
      ret = re->match(ret.rest);
      if (!ret.success) {
        return {false, sv};
      }
    }
    return ret;
  }

  std::vector<regex> parts;
};

struct choice final : public node {
  choice(regex l, regex r)
      : left(l)
      , right(r)
  {
  }

  result match(std::string_view sv) const override
  {
    auto [l_suc, l_res] = left->match(sv);
    if (l_suc) {
      return {l_suc, l_res};
    } else {
      return right->match(sv);
    }
  }

  regex left;
  regex right;
};

void tests()
{
  auto p1     = character('a');
  auto [s, r] = p1.match("abacus");
  assert(s && r == "bacus");

  auto p2       = character('f');
  auto [s2, r2] = p2.match("efrk");
  assert(!s2 && r2 == "efrk");

  auto seq1 = seq(
      {std::make_shared<character>('a'), std::make_shared<character>('b'),
       std::make_shared<character>('c')});
  auto [s3, r3] = seq1.match("abcdefg");
  assert(s3 && r3 == "defg");

  auto seq2 = seq(
      {std::make_shared<character>('b'),
       std::make_shared<seq>(
           std::vector<regex> {std::make_shared<character>('c')})});
  auto [s4, r4] = seq2.match("bdef");
  assert(!s4 && r4 == "bdef");

  auto or1 = choice(
      std::make_shared<character>('a'), std::make_shared<character>('b'));
  auto [s5, r5] = or1.match("aihu");
  auto [s6, r6] = or1.match("biho");
  assert(s5 && s6 && r5 == "ihu" && r6 == "iho");

  auto or2 = choice(
      std::make_shared<character>('c'),
      std::make_shared<seq>(std::vector<regex> {
          std::make_shared<character>('a'), std::make_shared<character>('b')}));
  auto [s7, r7] = or2.match("cfij");
  auto [s8, r8] = or2.match("abprl");
  auto [s9, r9] = or2.match("jjj");
  assert(s7 && s8 && r7 == "fij" && r8 == "prl");
  assert(!s9 && r9 == "jjj");
}

int main()
{
  tests();
  // Parsing - first pass collects all the regex sources into a map by ID. Then,
  // go through and parse them one by one - if one makes a reference to
  // something not already in the "compiled" map then compile it recursively and
  // do so.
  // Memoizing like this means that we compile each regex exactly once, and
  // sharing pointers to safe structures means no exponential allocation.
}
