#include <utils/utils.h>

#include <iostream>
#include <set>
#include <vector>

template <typename T>
class ring_buffer {
public:
  ring_buffer(size_t c)
      : capacity_(c)
      , next_(0)
  {
  }

  bool push(T item)
  {
    if (data_.size() < capacity_) {
      data_.push_back(item);
      return false;
    } else {
      data_[next_] = item;
      next_        = (next_ + 1) % capacity_;
      return true;
    }
  }

  bool marker() const
  {
    auto seen = std::set<T> {};
    for (auto d : data_) {
      seen.insert(d);
    }
    return seen.size() == capacity_;
  }

private:
  size_t         capacity_;
  size_t         next_;
  std::vector<T> data_;
};

int identify(std::string const& data, size_t len)
{
  auto buf = ring_buffer<char>(len);

  for (auto i = 0u; i < data.size(); ++i) {
    buf.push(data[i]);

    if (buf.marker()) {
      return i + 1;
    }
  }

  return -1;
}

int main()
{
  auto input = utils::get_single_line();

  std::cout << identify(input, 4) << '\n';
  std::cout << identify(input, 14) << '\n';
}
