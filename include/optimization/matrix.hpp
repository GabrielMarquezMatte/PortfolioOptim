#include <vector>
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
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
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
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
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
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
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
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < other.cols; j++)
                {
                    T sum = 0;
                    for (int k = 0; k < cols; k++)
                    {
                        sum += data[i * cols + k] * other.data[k * other.cols + j];
                    }
                    result(i, j) = sum;
                }
            }
            return result;
        }
        Matrix<T> operator*(const T &scalar)
        {
            Matrix<T> result(rows, cols);
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    result(i, j) = (*this)(i, j) * scalar;
                }
            }
            return result;
        }
        Matrix<T> operator/(const T &scalar)
        {
            Matrix<T> result(rows, cols);
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
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
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    result(j, i) = (*this)(i, j);
                }
            }
            return result;
        }
        T determinant()
        {
            if (!is_square())
            {
                throw std::invalid_argument("Matrix must be square.");
            }
            if (rows == 1)
            {
                return (*this)(0, 0);
            }
            if(rows == 2)
            {
                return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
            }
            T det = 0;
            for (int i = 0; i < cols; i++)
            {
                Matrix<T> submatrix(rows - 1, cols - 1);
                for (int j = 1; j < rows; j++)
                {
                    for (int k = 0; k < cols; k++)
                    {
                        if (k < i)
                        {
                            submatrix(j - 1, k) = (*this)(j, k);
                        }
                        else if (k > i)
                        {
                            submatrix(j - 1, k - 1) = (*this)(j, k);
                        }
                    }
                }
                det += pow(-1, i) * (*this)(0, i) * submatrix.determinant();
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
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    Matrix<T> submatrix(rows - 1, cols - 1);
                    for (int k = 0; k < rows; k++)
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
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
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
    };
}