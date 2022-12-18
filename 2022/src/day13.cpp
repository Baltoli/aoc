#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <memory>

struct item {
  virtual ~item() = default;

  virtual int is_less(std::unique_ptr<item> const& other) const
  {
    assert(false);
  }

  bool is_divider() const;
};

struct list;

struct integer : public item {
  integer(int i)
      : value(i)
  {
  }

  int is_less(std::unique_ptr<item> const& other) const override;

  std::unique_ptr<list> to_list() const;

  int value;
};

struct list : public item {
  list(std::vector<std::unique_ptr<item>> l)
      : values(std::move(l))
  {
  }

  int is_less(std::unique_ptr<item> const& other) const override;

  std::vector<std::unique_ptr<item>> values;
};

int integer::is_less(std::unique_ptr<item> const& other) const
{
  if (auto n = dynamic_cast<integer*>(other.get())) {
    if (value < n->value) {
      return -1;
    } else if (value > n->value) {
      return 1;
    } else {
      return 0;
    }
  } else if (auto l = dynamic_cast<list*>(other.get())) {
    return to_list()->is_less(other);
  } else {
    assert(false);
  }
}

int list::is_less(std::unique_ptr<item> const& other) const
{
  if (auto n = dynamic_cast<integer*>(other.get())) {
    return is_less(n->to_list());
  } else if (auto l = dynamic_cast<list*>(other.get())) {
    for (auto i = 0u; i < std::min(values.size(), l->values.size()); ++i) {
      auto elt_result = values[i]->is_less(l->values[i]);
      if (elt_result != 0) {
        return elt_result;
      }
    }

    if (values.size() < l->values.size()) {
      return -1;
    } else if (values.size() > l->values.size()) {
      return 1;
    } else {
      return 0;
    }
  } else {
    assert(false);
  }
}

std::unique_ptr<list> integer::to_list() const
{
  auto v = std::vector<std::unique_ptr<item>> {};
  v.push_back(std::make_unique<integer>(value));
  return std::make_unique<list>(std::move(v));
}

bool item::is_divider() const
{
  if (auto l1 = dynamic_cast<list const*>(this); l1 && l1->values.size() == 1) {
    if (auto l2 = dynamic_cast<list const*>(l1->values[0].get());
        l2 && l2->values.size() == 1) {
      if (auto n = dynamic_cast<integer const*>(l2->values[0].get())) {
        return n->value == 2 || n->value == 6;
      }
    }
  }

  return false;
}

std::unique_ptr<list> make_divider(int n)
{
  auto inner = std::make_unique<integer>(n);
  auto v1    = std::vector<std::unique_ptr<item>> {};
  v1.push_back(std::move(inner));
  auto l1 = std::make_unique<list>(std::move(v1));

  auto v2 = std::vector<std::unique_ptr<item>> {};
  v2.push_back(std::move(l1));
  return std::make_unique<list>(std::move(v2));
}

using parse_result = std::pair<std::unique_ptr<item>, std::string_view>;
parse_result parse(std::string_view sv);

parse_result parse_int(std::string_view sv)
{
  auto val = utils::svtoi(sv);
  auto len = (val / 10) + 1;
  return {std::make_unique<integer>(val), sv.substr(len)};
}

parse_result parse_list(std::string_view sv)
{
  auto vals = std::vector<std::unique_ptr<item>> {};
  sv        = sv.substr(1);

  while (sv[0] != ']') {
    auto [item, next_sv] = parse(sv);

    vals.push_back(std::move(item));
    sv = next_sv;

    if (sv[0] == ',') {
      sv = sv.substr(1);
    }
  }

  return {std::make_unique<list>(std::move(vals)), sv.substr(1)};
}

parse_result parse(std::string_view sv)
{
  if (std::isdigit(sv[0])) {
    return parse_int(sv);
  }

  if (sv[0] == '[') {
    return parse_list(sv);
  }

  assert(false && "Bad top-level");
}

int part_1(std::vector<std::string> const& input)
{
  auto sum = 0;
  for (auto i = 0u; i < input.size(); i += 3) {
    auto [left, lsv]  = parse_list(input[i]);
    auto [right, rsv] = parse_list(input[i + 1]);
    assert(lsv.empty() && rsv.empty());

    auto idx = (i / 3) + 1;
    if (left->is_less(right) == -1) {
      sum += idx;
    }
  }
  return sum;
}

int part_2(std::vector<std::string> const& input)
{
  auto items = std::vector<std::unique_ptr<item>> {};
  for (auto const& line : input) {
    if (!line.empty()) {
      items.push_back(parse(line).first);
    }
  }

  items.push_back(make_divider(2));
  items.push_back(make_divider(6));

  std::sort(items.begin(), items.end(), [](auto const& a, auto const& b) {
    return a->is_less(b) == -1;
  });

  auto idx = 1;

  for (auto i = 0; i < items.size(); ++i) {
    if (items[i]->is_divider()) {
      idx *= (i + 1);
    }
  }

  return idx;
}

int main()
{
  auto input = utils::get_lines();

  fmt::print("{}\n", part_1(input));
  fmt::print("{}\n", part_2(input));
}

template <>
struct fmt::formatter<item> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(item const& number, FormatContext& ctx)
  {
    if (auto n = dynamic_cast<integer const*>(&number)) {
      return fmt::format_to(ctx.out(), "{}", n->value);
    } else if (auto l = dynamic_cast<list const*>(&number)) {
      fmt::format_to(ctx.out(), "[");
      auto comma = "";
      for (auto const& v : l->values) {
        fmt::format_to(ctx.out(), "{}{}", comma, *v);
        comma = ",";
      }
      return fmt::format_to(ctx.out(), "]");
    } else {
      assert(false);
    }
  }
};
