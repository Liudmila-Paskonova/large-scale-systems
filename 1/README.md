# Задание
Реализуйте ```constexpr```-функцию, вычисляющую определитель целочисленной квадратной мат-
рицы во время компиляции. Реализуйте ваш код так, чтобы он компилировался на **C++14**.
```cpp
#include <array>
#include <iostream>
template <int N>
constexpr int Det(const std::array<std::array<int, N>, N>& matrix) {
// ...
}

int main() {
constexpr std::array<std::array<int, 3>, 3> matrix = {{
}};
constexpr int result = Det<3>(matrix);
std::cout << result << std::endl;
return 0;
}
```
