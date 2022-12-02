#include <iostream>
#include "data/download_data.hpp"
namespace data = portfolio_optimizer::data;

int main() {
    std::cout << "Hello, World!" << std::endl;
    data::DownloadData data = portfolio_optimizer::data::DownloadData();
    data::datetime date_util = data::datetime();
    std::time_t time = date_util.now();
    std::string time_string = date_util.to_string(time, "%d/%m/%Y %H:%M:%S");
    std::cout << time_string << "\n";
    return 0;
}