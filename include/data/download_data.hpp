#pragma once
#include "date_utils.hpp"
#include <vector>
#include <string>
#include <ctime>
namespace portfolio_optimizer::data
{
    static datetime date_util;
    class YahooStockData
    {
    public:
        std::string symbol;
        std::vector<std::time_t> date;
        std::vector<double> open;
        std::vector<double> high;
        std::vector<double> low;
        std::vector<double> close;
        std::vector<double> adj_close;
        std::vector<double> volume;
        enum class ReturnColumn
        {
            Open,
            High,
            Low,
            Close,
            AdjClose,
            Volume
        };
        std::vector<double> get_return(ReturnColumn column);
        std::string to_string();
        void reserve(size_t size);
    };
    YahooStockData download_yahoo_data(const std::string &symbol,
                                       const std::time_t &start = date_util.add_time(date_util.now(), -5),
                                       const std::time_t &end = date_util.now(),
                                       const bool verbose = false);
}