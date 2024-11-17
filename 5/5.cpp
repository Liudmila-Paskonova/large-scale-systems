#include <iostream>
#include <memory>

class Any
{

    struct Base {
        virtual std::unique_ptr<Base> copy() const = 0;
        virtual ~Base() = default;
    };

    template <typename T> struct TypedBase : Base {
        T value;

        explicit TypedBase(const T &value) : value(value) {}

        std::unique_ptr<Base>
        copy() const override
        {
            return std::make_unique<TypedBase<T>>(*this);
        }
    };

    std::unique_ptr<Base> typed = nullptr;

  public:
    template <typename T> Any(const T &value) : typed(std::make_unique<TypedBase<T>>(value)) {}

    Any(const Any &other) : typed(other.typed ? other.typed->copy() : nullptr) {}

    Any &
    operator=(const Any &other)
    {
        if (this != &other) {
            typed = other.typed ? other.typed->copy() : nullptr;
        }
        return *this;
    }

    template <typename T>
    T &
    get()
    {
        if (!typed) {
            throw "Empty pointer";
        }
        auto derived = dynamic_cast<TypedBase<T> *>(typed.get());
        if (!derived) {
            throw "Wrong type!";
        }
        return derived->value;
    }
};

int
main()
{
    Any a = 5;
    try {
        std::cout << a.get<int>() << std::endl;
    } catch (char const *s) {
        std::cout << s << std::endl;
    }
    try {
        std::cout << a.get<std::string>() << std::endl;
    } catch (char const *s) {
        std::cout << s << std::endl;
    }
}
