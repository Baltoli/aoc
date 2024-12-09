#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <list>
#include <map>

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

void defrag_whole(std::vector<std::int64_t>& bs)
{
  auto empty_slots = std::map<std::size_t, std::size_t> {};
  auto block_slots
      = std::map<std::int64_t, std::pair<std::size_t, std::size_t>> {};

  for (auto i = 0u; i < bs.size(); ++i) {
    auto slot_begin = i;
    while (bs.at(i) == empty_id) {
      empty_slots.try_emplace(slot_begin, 0);
      empty_slots.at(slot_begin)++;
      ++i;
    }
  }

  auto first_fit = [&empty_slots](auto size) {
    return std::find_if(empty_slots.begin(), empty_slots.end(), [size](auto p) {
      return p.second >= size;
    });
  };

  for (auto i = 0u; i < bs.size();) {
    auto slot_begin = i;
    auto id         = bs.at(i);

    if (id == empty_id) {
      ++i;
      continue;
    }

    while (i < bs.size() && bs.at(i) == id) {
      block_slots.try_emplace(id, std::pair {slot_begin, 0});
      block_slots.at(id).second++;
      ++i;
    }
  }

  for (auto it = block_slots.rbegin(); it != block_slots.rend(); ++it) {
    auto req_size = it->second.second;
    auto slot     = first_fit(req_size);

    if (slot != empty_slots.end()) {
      assert(slot->second >= req_size);

      if (it->second.first > slot->first) {
        for (auto i = 0u; i < req_size; ++i) {
          bs[slot->first + i]      = it->first;
          bs[it->second.first + i] = empty_id;
        }

        empty_slots.emplace(slot->first + req_size, slot->second - req_size);
        empty_slots.erase(slot);
      }
    }
  }
}

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
  auto in_2    = build_bytes(in_line);

  defrag(in);
  fmt::print("{}\n", checksum(in));

  defrag_whole(in_2);
  fmt::print("{}\n", checksum(in_2));
}