#pragma once

#if defined(__GNUC__) || defined(__GNUG__)
#define KB_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define KB_WARN_UNUSED_RESULT
#endif

namespace kb {

template <typename T>
class Optional {
 public:
  Optional() {
  }

  Optional(T value) : value_(value), has_value_(true) {
  }

  T value() const {
    return value_;
  }

  bool has_value() const {
    return has_value_;
  }

 private:
  T value_;
  bool has_value_ = false;
};

}  // namespace kb
