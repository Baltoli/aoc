#include <utils/utils.h>

#include <cassert>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <stack>

class sf_number {
public:
  using sf_ptr = std::unique_ptr<sf_number>;

  virtual void   print(std::ostream&) const = 0;
  virtual int    magnitude() const          = 0;
  virtual void   set_depths(int base)       = 0;
  virtual void   renumber(int& counter)     = 0;
  virtual sf_ptr clone() const              = 0;

  virtual ~sf_number() = default;

  static std::unique_ptr<sf_number>
  add(std::unique_ptr<sf_number> l, std::unique_ptr<sf_number> r);

  int depth_ = 0;
  int order_ = 0;
};

std::ostream& operator<<(std::ostream& os, sf_number const& n)
{
  n.print(os);
  return os;
}

class regular : public sf_number {
public:
  regular(int d)
      : data_(d)
  {
  }

  void print(std::ostream& os) const override
  {
    /* os << data_ << '{' << order_ << '}'; */
    os << data_;
  }

  void set_depths(int base) override { depth_ = base; }

  void renumber(int& c) override { order_ = c++; }

  sf_ptr clone() const override { return std::make_unique<regular>(data_); }

  int magnitude() const override { return data_; }

  int data_;
};

class pair : public sf_number {
public:
  pair(std::unique_ptr<sf_number> l, std::unique_ptr<sf_number> r)
      : left_(std::move(l))
      , right_(std::move(r))
  {
  }

  pair()
      : left_(nullptr)
      , right_(nullptr)
  {
  }

  static std::unique_ptr<pair> parse(std::string_view s)
  {
    auto stack = std::stack<std::unique_ptr<pair>> {};

    std::unique_ptr<sf_number>* target = nullptr;

    for (auto c : s) {
      if (c == '[') {
        stack.push(std::make_unique<pair>());
        target = &stack.top()->left_;
      } else if (std::isdigit(c)) {
        *target = std::make_unique<regular>(utils::ctoi(c));
      } else if (c == ',') {
        target = &stack.top()->right_;
      } else if (c == ']') {
        auto complete = std::move(stack.top());
        stack.pop();

        if (stack.empty()) {
          return complete;
        }

        if (!stack.top()->left_) {
          stack.top()->left_ = std::move(complete);
        } else if (!stack.top()->right_) {
          stack.top()->right_ = std::move(complete);
        } else {
          assert(false && "Bad parse");
        }
      } else {
        assert(false && "Bad char");
      }
    }

    assert(false && "Bad parse");
  }

  void set_depths(int base) override
  {
    depth_ = base;
    left_->set_depths(base + 1);
    right_->set_depths(base + 1);
  }

  void renumber(int& c) override
  {
    left_->renumber(c);
    order_ = c++;
    right_->renumber(c);
  }

  sf_ptr clone() const override
  {
    return std::make_unique<pair>(left_->clone(), right_->clone());
  }

  void reduce()
  {
    bool action_performed;
    do {
      action_performed = false;

      set_depths(0);
      int counter = 0;
      renumber(counter);

      auto nodes = std::vector<sf_ptr*> {};
      auto queue = std::stack<sf_ptr*> {};

      queue.push(&left_);
      queue.push(&right_);

      while (!queue.empty()) {
        auto next = queue.top();
        queue.pop();

        nodes.push_back(next);

        if (auto p = dynamic_cast<pair*>(next->get())) {
          queue.push(&p->left_);
          queue.push(&p->right_);
        }
      }

      std::sort(nodes.begin(), nodes.end(), [](auto a, auto b) {
        return (*a)->order_ < (*b)->order_;
      });

      for (auto i = 0; i < nodes.size(); ++i) {
        if (auto this_pair = dynamic_cast<pair*>(nodes[i]->get())) {
          if ((*nodes[i])->depth_ >= 4) {
            for (auto left = i - 2; left >= 0; --left) {
              if (auto reg = dynamic_cast<regular*>(nodes[left]->get())) {
                auto left_part = dynamic_cast<regular*>(this_pair->left_.get());
                reg->data_ += left_part->data_;
                break;
              }
            }

            for (auto right = i + 2; right < nodes.size(); ++right) {
              if (auto reg = dynamic_cast<regular*>(nodes[right]->get())) {
                auto right_part
                    = dynamic_cast<regular*>(this_pair->right_.get());
                reg->data_ += right_part->data_;
                break;
              }
            }

            *nodes[i] = std::make_unique<regular>(0);

            action_performed = true;
            break;
          }
        }
      }

      if (action_performed) {
        continue;
      }

      for (auto i = 0; i < nodes.size(); ++i) {
        if (auto reg = dynamic_cast<regular*>(nodes[i]->get())) {
          if (reg->data_ > 9) {
            *nodes[i] = std::make_unique<pair>(
                std::make_unique<regular>(reg->data_ / 2),
                std::make_unique<regular>((reg->data_ / 2) + reg->data_ % 2));
            action_performed = true;
            break;
          }
        }
      }

    } while (action_performed);
  }

  virtual void print(std::ostream& os) const override
  {
    os << '[';
    left_->print(os);
    os << ',';
    /* os << '{' << order_ << '}'; */
    right_->print(os);
    os << ']';
  }

  int magnitude() const override
  {
    return 3 * left_->magnitude() + 2 * right_->magnitude();
  }

private:
  std::unique_ptr<sf_number> left_;
  std::unique_ptr<sf_number> right_;
};

std::unique_ptr<sf_number>
sf_number::add(std::unique_ptr<sf_number> l, std::unique_ptr<sf_number> r)
{
  auto new_pair = std::make_unique<pair>(std::move(l), std::move(r));
  new_pair->reduce();
  return new_pair;
}

int part_1(std::vector<std::string> const& lines)
{
  std::unique_ptr<sf_number> current = pair::parse(lines[0]);

  std::for_each(lines.begin() + 1, lines.end(), [&current](auto const& line) {
    current = sf_number::add(std::move(current), pair::parse(line));
  });

  return current->magnitude();
}

int part_2(std::vector<std::string> const& lines)
{
  auto max_sum = std::numeric_limits<int>::min();

  auto numbers = std::vector<std::unique_ptr<sf_number>> {};
  for (auto const& line : lines) {
    numbers.push_back(pair::parse(line));
  }

  for (auto i = 0; i < numbers.size(); ++i) {
    for (auto j = 0; j < numbers.size(); ++j) {
      if (i != j) {
        auto a     = sf_number::add(numbers[i]->clone(), numbers[j]->clone());
        auto a_mag = a->magnitude();

        auto b     = sf_number::add(numbers[j]->clone(), numbers[i]->clone());
        auto b_mag = b->magnitude();

        max_sum = std::max(max_sum, std::max(a_mag, b_mag));
      }
    }
  }

  return max_sum;
}

int main()
{
  auto lines = utils::get_lines();

  std::cout << part_1(lines) << '\n';
  std::cout << part_2(lines) << '\n';
}
