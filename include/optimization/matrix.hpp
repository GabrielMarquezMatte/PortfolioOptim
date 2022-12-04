#pragma once
#include <vector>
#include <string>
#include <stdexcept>
namespace portfolio_optimizer::optimization
{
    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix
    {
    private:
        std::vector<T> data;
    public:
        size_t rows;
        size_t cols;
        Matrix(const size_t rows, const size_t cols)
        {
            this->rows = rows;
            this->cols = cols;
            data.resize(rows * cols);
        }
        Matrix(const std::vector<std::vector<T>> &data)
        {
            this->rows = data.size();
            this->cols = data[0].size();
            this->data.resize(rows * cols);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    this->data[i * cols + j] = data[i][j];
                }
            }
        }
        Matrix()
        {
            this->rows = 0;
            this->cols = 0;
        }
        Matrix(const std::vector<T> &data, const size_t rows, const size_t cols)
        {
            this->rows = rows;
            this->cols = cols;
            this->data = data;
        }
        Matrix<T> operator+(const Matrix<T> &other)
        {
            if (rows != other.rows || cols != other.cols)
            {
                throw std::invalid_argument("Matrix dimensions must agree.");
            }
            Matrix<T> result(rows, cols);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    result(i, j) = (*this)(i, j) + other(i, j);
                }
            }
            return result;
        }
        Matrix<T> operator-(const Matrix<T> &other)
        {
            if (rows != other.rows || cols != other.cols)
            {
                throw std::invalid_argument("Matrix dimensions must agree.");
            }
            Matrix<T> result(rows, cols);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    result(i, j) = (*this)(i, j) - other(i, j);
                }
            }
            return result;
        }
        Matrix<T> operator*(const Matrix<T> &other)
        {
            if (cols != other.rows)
            {
                throw std::invalid_argument("Matrix dimensions must agree.");
            }
            Matrix<T> result(rows, other.cols);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < other.cols; j++)
                {
                    T sum = 0;
                    for (size_t k = 0; k < cols; k++)
                    {
                        sum += data[i * cols + k] * other.data[k * other.cols + j];
                    }
                    result(i, j) = sum;
                }
            }
            return result;
        }
        Matrix<T> operator*(const std::vector<T> &other)
        {
            if (cols != other.size())
            {
                throw std::invalid_argument("Matrix dimensions must agree.");
            }
            Matrix<T> result(rows, 1);
            for (size_t i = 0; i < rows; i++)
            {
                T sum = 0;
                for (size_t k = 0; k < cols; k++)
                {
                    sum += data[i * cols + k] * other[k];
                }
                result(i, 0) = sum;
            }
            return result;
        }
        void operator*=(const Matrix<T> &other)
        {
            if (cols != other.rows)
            {
                throw std::invalid_argument("Matrix dimensions must agree.");
            }
            Matrix<T> result(rows, other.cols);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < other.cols; j++)
                {
                    T sum = 0;
                    for (size_t k = 0; k < cols; k++)
                    {
                        sum += data[i * cols + k] * other.data[k * other.cols + j];
                    }
                    result(i, j) = sum;
                }
            }
            *this = result;
        }
        void operator*=(const std::vector<T> &other)
        {
            if (cols != other.size())
            {
                throw std::invalid_argument("Matrix dimensions must agree.");
            }
            Matrix<T> result(rows, 1);
            for (size_t i = 0; i < rows; i++)
            {
                T sum = 0;
                for (size_t k = 0; k < cols; k++)
                {
                    sum += data[i * cols + k] * other[k];
                }
                result(i, 0) = sum;
            }
            *this = result;
        }
        Matrix<T> operator*(const T &scalar)
        {
            Matrix<T> result(rows, cols);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    result(i, j) = (*this)(i, j) * scalar;
                }
            }
            return result;
        }
        void operator*= (const T &scalar)
        {
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    (*this)(i, j) *= scalar;
                }
            }
        }
        Matrix<T> operator/(const T &scalar)
        {
            Matrix<T> result(rows, cols);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    result(i, j) = (*this)(i, j) / scalar;
                }
            }
            return result;
        }
        bool is_square()
        {
            return rows == cols;
        }
        Matrix<T> transpose()
        {
            Matrix<T> result(cols, rows);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    result(j, i) = (*this)(i, j);
                }
            }
            return result;
        }
        T determinant()
        {
            //Algorithm to calculate the determinant of a matrix using LU decomposition
            //https://www.geeksforgeeks.org/determinant-of-a-matrix/
            if (!is_square())
            {
                throw std::invalid_argument("Matrix must be square.");
            }
            Matrix<T> L(rows, cols);
            Matrix<T> U(rows, cols);
            for (size_t i = 0; i < rows; i++)
            {
                for (int k = i; k < cols; k++)
                {
                    T sum = 0;
                    for (size_t j = 0; j < i; j++)
                    {
                        sum += (L(i, j) * U(j, k));
                    }
                    U(i, k) = data[i * cols + k] - sum;
                }
                for (int k = i; k < rows; k++)
                {
                    if (i == k)
                    {
                        L(i, i) = 1;
                    }
                    else
                    {
                        T sum = 0;
                        for (size_t j = 0; j < i; j++)
                        {
                            sum += (L(k, j) * U(j, i));
                        }
                        L(k, i) = (data[k * cols + i] - sum) / U(i, i);
                    }
                }
            }
            T det = 1;
            for (size_t i = 0; i < rows; i++)
            {
                if(U(i, i) == 0)
                {
                    return 0;
                }
                det *= U(i, i);
            }
            return det;
        }
        Matrix<T> cofactor()
        {
            if (!is_square())
            {
                throw std::invalid_argument("Matrix must be square.");
            }
            Matrix<T> result(rows, cols);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    Matrix<T> submatrix(rows - 1, cols - 1);
                    for (size_t k = 0; k < rows; k++)
                    {
                        for (int l = 0; l < cols; l++)
                        {
                            if (k != i && l != j)
                            {
                                submatrix(k > i ? k - 1 : k, l > j ? l - 1 : l) = (*this)(k, l);
                            }
                        }
                    }
                    result(i, j) = submatrix.determinant() * pow(-1, i + j);
                }
            }
            return result;
        }
        Matrix<T> inverse()
        {
            if (!is_square())
            {
                throw std::invalid_argument("Matrix must be square.");
            }
            T det = determinant();
            if (det == 0)
            {
                throw std::invalid_argument("Matrix must be invertible.");
            }
            return cofactor().transpose() / det;
        }
        const std::string to_string()
        {
            std::string result = "";
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    std::string value = std::to_string(data[i * cols + j]);
                    result += value + std::string(10 - value.length(), ' ');
                }
                result += "\n";
            }
            return result;
        }
        T get(int row, int col)
        {
            return data[row * cols + col];
        }
        //Operator to set value of matrix with matrix notation, like matrix(1,2) = 5
        T& operator()(const size_t row,const size_t col)
        {
            return data[row * cols + col];
        }
        void cbind(const T& value){
            Matrix<T> result(rows, cols + 1);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    result(i, j) = (*this)(i, j);
                }
                result(i, cols) = value;
            }
            *this = result;
        }
        void cbind(const std::vector<T>& values){
            if(values.size() != rows){
                throw std::invalid_argument("Vector size must be equal to number of rows.");
            }
            Matrix<T> result(rows, cols + 1);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    result(i, j) = (*this)(i, j);
                }
                result(i, cols) = values[i];
            }
            *this = result;
        }
        void rbind(const T& value){
            Matrix<T> result(rows + 1, cols);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    result(i, j) = (*this)(i, j);
                }
            }
            for (size_t j = 0; j < cols; j++)
            {
                result(rows, j) = value;
            }
            *this = result;
        }
        void rbind(const std::vector<T>& values){
            if(values.size() != cols){
                throw std::invalid_argument("Vector size must be equal to number of columns.");
            }
            Matrix<T> result(rows + 1, cols);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    result(i, j) = (*this)(i, j);
                }
            }
            for (size_t j = 0; j < cols; j++)
            {
                result(rows, j) = values[j];
            }
            *this = result;
        }
        Matrix<T> submatrix(size_t row, size_t col)
        {
            if (row >= rows || col >= cols)
            {
                throw std::invalid_argument("Row or column index out of bounds.");
            }
            Matrix<T> result(rows - 1, cols - 1);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    if (i != row && j != col)
                    {
                        result(i > row ? i - 1 : i, j > col ? j - 1 : j) = (*this)(i, j);
                    }
                }
            }
            return result;
        }
        Matrix<T> submatrix(size_t row, size_t col, size_t row_count, size_t col_count)
        {
            if (row >= rows || col >= cols)
            {
                throw std::invalid_argument("Row or column index out of bounds.");
            }
            if (row + row_count > rows || col + col_count > cols)
            {
                throw std::invalid_argument("Submatrix out of bounds.");
            }
            Matrix<T> result(row_count, col_count);
            for (size_t i = 0; i < row_count; i++)
            {
                for (size_t j = 0; j < col_count; j++)
                {
                    result(i, j) = (*this)(row + i, col + j);
                }
            }
            return result;
        }
        std::vector<T> as_vector()
        {
            std::vector<T> result(rows * cols);
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    result[i * cols + j] = (*this)(i, j);
                }
            }
            return result;
        }
    };
}