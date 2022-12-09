#include <iostream>
#include "optimization/optimization.hpp"
#include "data/download_data.hpp"
#include <thread>
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
                                               const std::time_t start_date = data::date_util.add_time(data::date_util.now(), -4),
                                               const std::time_t end_date = data::date_util.now())
{
    std::vector<data::YahooStockData> result(tickers.size());
    std::vector<std::future<void>> futures(tickers.size());
    for (int i = 0; i < tickers.size(); i++)
    {
        futures[i] = std::async(std::launch::async, [&result, &tickers, i, start_date, end_date]() {
            result[i] = data::download_yahoo_data(tickers[i], start_date, end_date);
        });
    }
    for (int i = 0; i < futures.size(); i++)
    {
        futures[i].wait();
    }
    return result;
}
void OptimizationTest()
{
    std::cout << "OptimizationTest:\n";
    std::vector<std::string> tickers = {"MSFT", "AMZN", "AAPL", "TSLA"};
    std::vector<data::YahooStockData> data = DownloadData(tickers);
    std::unordered_map<std::string, std::vector<double>> historical_prices(tickers.size());
    std::vector<double> expected_returns(tickers.size());
    for (int i = 0; i < tickers.size(); i++)
    {
        historical_prices[tickers[i]] = data[i].get_return(data::YahooStockData::ReturnColumn::AdjClose);
        double mean = 0;
        for (int j = 0; j < historical_prices[tickers[i]].size(); j++)
        {
            mean += log(historical_prices[tickers[i]][j]+1);
        }
        expected_returns[i] = exp((mean / historical_prices[tickers[i]].size())*252)-1;
    }
    optimization::Matrix covariance_matrix = optimization::calculate_covariance_matrix(historical_prices)*252;
    optimization::Optimization optimization(tickers, historical_prices, expected_returns, 0, covariance_matrix);
    auto results = optimization.minimum_risk({0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0});
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