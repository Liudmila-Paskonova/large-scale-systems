#include <iostream>
#include <map>
#include <print>
#include <chrono>

template <typename Func>
long long
measure(Func f)
{
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int
main()
{
    const int N = 100000;

    // Case 1: sorted insertion end
    {
        std::map<int, int> a, b;

        long long withoutHint = measure([&]() {
            for (int i = 0; i < N; ++i) {
                a.insert({i, i});
            }
        });

        auto withHint = measure([&]() {
            auto hint = b.end();
            for (int i = 0; i < N; ++i) {
                hint = b.insert(hint, {i, i});
            }
        });

        std::println("Sorted insertion end:");
        std::println("Without hint: {} ms", withoutHint);
        std::println("With hint: {} ms", withHint);
        std::println("----------------------------");
    }

    // Case 2: sorted insertion begin
    {
        std::map<int, int> a, b;

        long long withoutHint = measure([&]() {
            for (int i = 0; i < N; ++i) {
                a.insert({i, i});
            }
        });

        auto withHint = measure([&]() {
            auto hint = b.begin();
            for (int i = 0; i < N; ++i) {
                hint = b.insert(hint, {i, i});
            }
        });

        std::println("Sorted insertion begin:");
        std::println("Without hint: {} ms", withoutHint);
        std::println("With hint: {} ms", withHint);
        std::println("----------------------------");
    }

    // Case 3: unsorted insertion
    {
        std::map<int, int> a, b;

        for (int i = 0; i < N; i += 5) {
            a.insert({i, i});
            b.insert({i, i});
        }

        long long withoutHint = measure([&]() {
            for (int i = 1; i < N; i += 5) {
                a.insert({i, i});
            }
        });

        long long withHint = measure([&]() {
            auto hint = b.end();
            for (int i = 1; i < N; i += 5) {
                hint = b.insert(hint, {i, i});
            }
        });

        std::println("Unsorted insertion end:");
        std::println("Without hint: {} ms", withoutHint);
        std::println("With hint: {} ms", withHint);
        std::println("----------------------------");
    }

    // Case 3: unsorted insertion begin
    {
        std::map<int, int> a, b;

        for (int i = 0; i < N; i += 5) {
            a.insert({i, i});
            b.insert({i, i});
        }

        long long withoutHint = measure([&]() {
            for (int i = 1; i < N; i += 5) {
                a.insert({i, i});
            }
        });

        long long withHint = measure([&]() {
            auto hint = b.begin();
            for (int i = 1; i < N; i += 5) {
                hint = b.insert(hint, {i, i});
            }
        });

        std::println("Unsorted insertion begin:");
        std::println("Without hint: {} ms", withoutHint);
        std::println("With hint: {} ms", withHint);
        std::println("----------------------------");
    }

    return 0;
}

/*
CONCLUSIONS:

Sorted insertion end:
Without hint: 27333 ms
With hint: 9180 ms
----------------------------
Sorted insertion begin:
Without hint: 24761 ms
With hint: 7182 ms
----------------------------
Unsorted insertion end:
Without hint: 3878 ms
With hint: 4465 ms
----------------------------
Unsorted insertion begin:
Without hint: 3852 ms
With hint: 4707 ms
----------------------------

If the elements being inserted are already given in some order (for example, sorted), then usage of hint 2-3 times
speeds up the insetion.

*/
