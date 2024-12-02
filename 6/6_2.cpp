#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <gtest/gtest.h>

class TMyException : public std::runtime_error
{
  public:
    explicit TMyException(const std::string &msg = "") : std::runtime_error(msg), msg_(msg) {}

    template <typename T>
    TMyException &
    operator<<(const T &additionalInfo)
    {
        std::ostringstream oss;
        oss << msg_ << additionalInfo;
        msg_ = oss.str();
        return *this;
    }

    const char *
    what() const noexcept override
    {
        return msg_.c_str();
    }

  private:
    std::string msg_;
};

class A : public TMyException
{
  public:
    explicit A(const std::string &msg = "") : TMyException(msg) {}
};

class B : public TMyException
{
  public:
    explicit B(const std::string &msg = "") : TMyException(msg) {}
};

/////////////////////// TEST ///////////////////////////////////
TEST(TMyExceptionTest, Test)
{
    try {
        TMyException ex("TMyException");
        ex << "additional info";
        throw ex;
    } catch (const TMyException &e) {
        EXPECT_STREQ(e.what(), "TMyException: additional info");
    }
}

TEST(TMyExceptionTest, ATest) { EXPECT_THROW(throw A("A"), A); }

TEST(TMyExceptionTest, BTest) { EXPECT_THROW(throw B("B"), B); }
