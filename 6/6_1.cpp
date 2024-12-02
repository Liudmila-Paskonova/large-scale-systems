#include <utility>
#include <type_traits>
#include <iostream>
#include <cstring>

template <char... c> class TString
{
    char data_[100]{};
    size_t size_{};

  public:
    constexpr TString() = default;

    constexpr TString(const char *str)
    {
        for (size_t i = 0; str[i] != '\0'; ++i) {
            data_[i] = str[i];
            ++size_;
        }
    }

    constexpr TString
    operator+(const TString &other) const
    {
        TString result;
        result.size_ = size_ + other.size_;
        for (size_t i = 0; i < size_; ++i) {
            result.data_[i] = data_[i];
        }
        for (size_t i = 0; i < other.size_; ++i) {
            result.data_[size_ + i] = other.data_[i];
        }
        return result;
    }

    constexpr bool
    operator==(const TString &other) const
    {
        if (size_ != other.size_) {
            return false;
        }
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != other.data_[i]) {
                return false;
            }
        }
        return true;
    }

    friend std::ostream &
    operator<<(std::ostream &os, const TString &p)
    {
        return os << p.data_;
    }
};

constexpr TString<> operator"" _s(const char *val, size_t) { return TString<>(val); }

int
main()
{
    constexpr auto hello = "hello"_s + " world"_s;
    static_assert(hello == "hello world"_s);
    std::cout << hello;
    return 0;
}
