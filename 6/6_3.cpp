
#include <atomic>
#include <cassert>
#include <utility>
#include <cstddef>
#include <iostream>

template <typename T> class TBasePtr
{

  public:
    T *Ptr = nullptr;

    TBasePtr() = default;

    explicit TBasePtr(T *ptr) : Ptr(ptr) {}

    TBasePtr(const TBasePtr &other) = default;

    TBasePtr &
    operator=(T *ptr)
    {
        Ptr = ptr;
        return *this;
    }

    TBasePtr &operator=(const TBasePtr &other) = default;

    ~TBasePtr() = default;
    // операторы ->, *, ==, !=, bool
    T *
    operator->() const
    {
        return Ptr;
    }

    T &
    operator*() const
    {
        return *Ptr;
    }

    explicit
    operator bool() const
    {
        return Ptr != nullptr;
    }

    bool
    operator==(const TBasePtr &other) const
    {
        return Ptr == other.Ptr;
    }

    bool
    operator!=(const TBasePtr &other) const
    {
        return Ptr != other.Ptr;
    }
};

template <typename T> class TRefCounter
{
    mutable std::atomic<size_t> Counter{0};

  public:
    void
    AddRef() const
    {
        ++Counter;
    }

    void
    ReleaseRef() const
    {
        --Counter;
    }

    size_t
    RefCount() const
    {
        return Counter.load();
    }
};

template <typename T> class TIntrusivePtr
{
  private:
    TBasePtr<T> Base;

  public:
    TIntrusivePtr() = default;

    TIntrusivePtr(std::nullptr_t) : Base(nullptr) {}

    T *
    Get() const
    {
        return Base.Ptr;
    }

    explicit TIntrusivePtr(T *ptr) : Base(ptr)
    {
        if (Get()) {
            Get()->AddRef();
        }
    }

    TIntrusivePtr(const TIntrusivePtr &other) : Base(other.Get())
    {
        if (Get()) {
            Get()->AddRef();
        }
    }

    TIntrusivePtr(TIntrusivePtr &&other) noexcept : Base(other.Get()) { other.Base = nullptr; }

    ~TIntrusivePtr() { Reset(); }

    // основные копирующие и перемещающие операции,
    TIntrusivePtr &
    operator=(const TIntrusivePtr &other)
    {
        if (this != &other) {
            Reset(other.Get());
        }
        return *this;
    }

    TIntrusivePtr &
    operator=(TIntrusivePtr &&other) noexcept
    {
        if (this != &other) {
            Reset();
            Base = other.Get();
            other.Base = nullptr;
        }
        return *this;
    }

    TIntrusivePtr &
    operator=(std::nullptr_t)
    {
        Reset();
        return *this;
    }
    // методы UseCount, Get, Reset, Release,

    size_t
    RefCount() const
    {
        return Get() ? Get()->RefCount() : 0;
    }

    size_t
    UseCount() const
    {
        return RefCount();
    }

    void
    Reset(T *ptr = nullptr)
    {
        if (Get()) {
            Get()->ReleaseRef();
        }
        Base = TBasePtr<T>(ptr);
        if (Get()) {
            Get()->AddRef();
        }
    }

    void
    Reset(const TIntrusivePtr &other)
    {
        Reset(other.Get());
    }

    void
    Reset(TIntrusivePtr &&other) noexcept
    {
        if (this != &other) {
            Reset();
            Base = other.Get();
            other.Base = nullptr;
        }
    }

    void
    Release()
    {
        Reset();
    }

    T *
    operator->() const
    {
        return Base.operator->();
    }

    T &
    operator*() const
    {
        return Base.operator*();
    }

    explicit
    operator bool() const
    {
        return Base.operator bool();
    }

    bool
    operator==(const TIntrusivePtr &other) const
    {
        return Base == other.Base;
    }

    bool
    operator!=(const TIntrusivePtr &other) const
    {
        return Base != other.Base;
    }
};

template <typename T, typename... Args>
TIntrusivePtr<T>
MakeIntrusive(Args &&...args)
{
    return TIntrusivePtr<T>(new T(std::forward<Args>(args)...));
}

class TDoc : public TRefCounter<TDoc>
{
  public:
    TDoc() { std::cout << __PRETTY_FUNCTION__ << std::endl; }

    ~TDoc() { std::cout << __PRETTY_FUNCTION__ << std::endl; }
};

int
main()
{
    TIntrusivePtr<TDoc> ptr = nullptr;
    ptr = MakeIntrusive<TDoc>();
    // ptr.RefCount() == 1
    assert(ptr.RefCount() == 1);
    TIntrusivePtr<TDoc> ptr2 = ptr;
    // ptr.RefCount() == 2
    assert(ptr2.RefCount() == 2);
    TIntrusivePtr<TDoc> ptr3 = MakeIntrusive<TDoc>();
    ptr3.Reset(ptr2);
    // ptr.RefCount() == ptr3.RefCount() == 3
    assert(ptr.RefCount() == 3 && ptr3.RefCount() == 3);
    ptr3.Reset();
    // ptr.RefCount() ==  ptr2.RefCount() == 2
    assert(ptr.RefCount() == 2 && ptr2.RefCount() == 2);
    ptr3.Reset(std::move(ptr2));
    // ptr.RefCount() == ptr3.RefCount() == 2
    assert(ptr.RefCount() == 2 && ptr3.RefCount() == 2);
    static_assert(sizeof(TDoc *) == sizeof(ptr));
    return 0;
}
