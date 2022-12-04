#include <iostream>
#include "optimization/optimization.hpp"
#include "data/download_data.hpp"
#include <future>
namespace data = portfolio_optimizer::data;
namespace optimization = portfolio_optimizer::optimization;
void DownloadTest()
{
    std::cout << "DownloadTest:\n";
    auto data = data::download_yahoo_data("AAPL");
    std::cout << data.to_string() + "\n";
}
std::vector<data::YahooStockData> DownloadData(const std::vector<std::string> &tickers,
                                               std::time_t start_date = data::date_util.add_time(data::date_util.now(), -4),
                                               std::time_t end_date = data::date_util.now())
{
    std::vector<std::future<data::YahooStockData>> futures(tickers.size());
    for (int i = 0; i < tickers.size(); i++)
    {
        futures[i] = std::async(std::launch::async, data::download_yahoo_data, std::ref(tickers[i]), std::ref(start_date), std::ref(end_date),false);
    }
    std::vector<data::YahooStockData> data_result(tickers.size());
    for (int i = 0; i < tickers.size(); i++)
    {
        data_result[i] = futures[i].get();
    }
    return data_result;
}
void OptimizationTest()
{
    std::cout << "OptimizationTest:\n";
    std::vector<std::string> tickers = {"MSFT","AMZN","AAPL","TSLA"};
    std::vector<data::YahooStockData> data = DownloadData(tickers);
    std::unordered_map<std::string, std::vector<double>> historical_prices(tickers.size());
    std::vector<double> expected_returns(tickers.size());
    for (int i = 0; i < tickers.size(); i++)
    {
        historical_prices[tickers[i]] = data[i].get_return(data::YahooStockData::ReturnColumn::AdjClose);
        double mean = 0;
        for (int j = 0; j < historical_prices[tickers[i]].size(); j++)
        {
            mean += historical_prices[tickers[i]][j];
        }
        expected_returns[i] = mean / historical_prices[tickers[i]].size();
    }
    optimization::Optimization optimization(tickers, historical_prices, expected_returns, 0);
    auto results = optimization.minimum_risk({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7});
    std::string result_string = "";
    for (int i = 0; i < results.size(); i++)
    {
        result_string += "Expected return: " + std::to_string(results[i].expected_return) + "\n";
        result_string += "Volatility: " + std::to_string(results[i].volatility) + "\n";
        result_string += "Leverage: " + std::to_string(results[i].leverage) + "\n";
        result_string += "Sharpe ratio: " + std::to_string(results[i].sharpe_ratio) + "\n";
        result_string += "Weights:\n";
        for (auto const &x : results[i].weights)
        {
            result_string += x.first + ": " + std::to_string(x.second) + "\n";
        }
        result_string += "\n";
    }
    std::cout << result_string;
}
int main()
{
    OptimizationTest();
    return 0;
}