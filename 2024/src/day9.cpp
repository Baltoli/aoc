#include <utils/utils.h>

#include <fmt/format.h>

#include <list>
#include <ranges>

constexpr std::int64_t empty_id = -1;

template <typename Range>
std::vector<std::int64_t> build_bytes(Range&& r)
{
  auto ret     = std::vector<std::int64_t> {};
  auto is_file = true;
  auto id      = 0;

  for (auto c : std::forward<Range>(r)) {
    auto size = utils::ctoi(c);
    for (auto i = 0; i < size; ++i) {
      if (is_file) {
        ret.push_back(id);
      } else {
        ret.push_back(empty_id);
      }
    }

    if (is_file) {
      id++;
    }

    is_file = !is_file;
  }

  return ret;
}

void dump(std::vector<std::int64_t> const& bs)
{
  for (auto v : bs) {
    if (v == empty_id) {
      fmt::print(".");
    } else {
      fmt::print("{}", v);
    }
  }
  fmt::print("\n");
}

void defrag(std::vector<std::int64_t>& bs)
{
  auto begin = std::find(bs.begin(), bs.end(), empty_id);
  auto end   = std::find_if(
      bs.rbegin(), bs.rend(), [](auto id) { return id != empty_id; });

  while (begin < end.base()) {
    std::swap(*begin, *end);

    while (*++begin != empty_id) {
    }

    while (*++end == empty_id) {
    }
  }
}

void defrag_whole(std::vector<std::int64_t>& bs) { }

std::int64_t checksum(std::vector<std::int64_t> const& bs)
{
  auto sum = std::int64_t {0};

  for (auto i = 0; i < bs.size(); ++i) {
    if (bs.at(i) != empty_id) {
      sum += i * bs.at(i);
    }
  }

  return sum;
}

int main()
{
  auto in_line = utils::get_single_line();
  auto in      = build_bytes(in_line);
  auto in_2    = in;

  defrag(in);
  fmt::print("{}\n", checksum(in));

  defrag_whole(in_2);
  fmt::print("{}\n", checksum(in_2));
}