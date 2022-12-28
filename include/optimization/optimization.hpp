#pragma once
#include "matrix.hpp"
#include <unordered_map>
#include <vector>
#include <string>
namespace portfolio_optimizer::optimization
{
    typedef struct{
        double expected_return;
        double volatility;
        double leverage;
        double sharpe_ratio;
        std::unordered_map<std::string, double> weights;
        std::unordered_map<std::string, double> covariance_contributions;
        std::vector<double> lagrange_multipliers;
    } OptimizationResult;
    class Optimization
    {
    private:
        Matrix<double> covariance_matrix;
        std::vector<double> expected_returns;
        double risk_free_rate = 0;
        std::vector<std::string> tickers;
        std::unordered_map<std::string, std::vector<double>> historical_prices;
        double calculate_covariance(const std::vector<double> &x, const std::vector<double> &y);
        double calculate_mean(const std::vector<double> &x);
        void minimize_risk(OptimizationResult& result,const double wanted_return,const std::vector<double>& expected_returns, Matrix<double>& covariance,const std::vector<std::string>& tickers);
    public:
        Optimization(const std::vector<std::string> &tickers, const std::unordered_map<std::string, std::vector<double>> &historical_prices, const std::vector<double> &expected_returns, const double risk_free_rate);
        Optimization(const std::vector<std::string> &tickers, const std::unordered_map<std::string, std::vector<double>> &historical_prices, const std::vector<double> &expected_returns, const double risk_free_rate,const Matrix<double>& covariance_matrix);
        std::vector<OptimizationResult> minimum_risk(const std::vector<double>& expected_returns, bool use_risk_free_rate = false);
    };
    Matrix<double> calculate_covariance_matrix(const std::unordered_map<std::string, std::vector<double>> &historical_prices);
    double calculate_covariance(const std::vector<double> &x, const std::vector<double> &y);
}