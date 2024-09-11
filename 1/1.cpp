#include <array>
#include <iostream>

// Get the M_0_j = minor, the submatrix (N-1) * (N-1), which 0 line and j column are removes
template <int N>
constexpr std::array<std::array<int, N - 1>, N - 1>
Submatrix(const std::array<std::array<int, N>, N> &matrix, int j)
{
    std::array<std::array<int, N - 1>, N - 1> newMatrix{};
    for (int line = 1; line < N; ++line) {
        int k = 0;
        for (int row = 0; row < N; ++row) {
            if (row != j) {
                (&std::get<0>(((&std::get<0>(newMatrix))[line - 1])))[k] = matrix[line][row];
                ++k;
            }
        }
    }

    return newMatrix;
}

// Recursively compute the determinant using the Laplace expansion each time by first line
// det(matrix) = sum_[0..N) { (-1)^j * matrix_0_j * M_0_j }
// M_0_j = minor
template <int N>
constexpr int
Det(const std::array<std::array<int, N>, N> &matrix)
{
    int d = 0;
    int pow = 1;
    for (int j = 0; j < N; ++j) {
        d += pow * matrix[0][j] * Det<N - 1>(Submatrix<N>(matrix, j));
        pow *= -1;
    }
    return d;
}

// Template instantiation for cases N = 0
template <>
constexpr int
Det<0>(const std::array<std::array<int, 0>, 0> &matrix)
{
    return 0;
}

// and N = 1
template <>
constexpr int
Det<1>(const std::array<std::array<int, 1>, 1> &matrix)
{
    return matrix[0][0];
}

int
main()
{
    constexpr std::array<std::array<int, 3>, 3> matrix = {{{0, 1, 2}, {1, 2, 3}, {2, 3, 7}}};
    constexpr int result = Det<3>(matrix);
    std::cout << result << std::endl;
    return 0;
}
