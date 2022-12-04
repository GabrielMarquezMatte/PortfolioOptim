#include <iostream>
#include "data/download_data.hpp"
#include "optimization/matrix.hpp"
namespace data = portfolio_optimizer::data;
namespace optimization = portfolio_optimizer::optimization;
void DownloadTest()
{
    std::cout << "DownloadTest:\n";
    auto data = data::download_yahoo_data("AAPL");
    std::cout << data.to_string()+"\n";
}
void MatrixTest()
{
    std::string result;
    result += "MatrixTest:\n";
    optimization::Matrix<double> m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(1, 0) = 3;
    m(1, 1) = 4;
    result += m.to_string()+"\n";
    result += m.transpose().to_string()+"\n";
    result += std::to_string(m.determinant())+"\n";
    result += m.inverse().to_string()+"\n";
    std::cout << result;
}
int main() {
    // DownloadTest();
    MatrixTest();
    return 0;
}