#include "optimization.hpp"
namespace portfolio_optimizer::optimization
{
    double calculate_mean(const std::vector<double> &x)
    {
        double sum = 0;
        for (int i = 0; i < x.size(); i++)
        {
            sum += x[i];
        }
        return sum / x.size();
    }
    double calculate_covariance(const std::vector<double> &x, const std::vector<double> &y)
    {
        double mean_x = calculate_mean(x);
        double mean_y = calculate_mean(y);
        double covariance = 0;
        for (int i = 0; i < x.size(); i++)
        {
            covariance += (x[i] - mean_x) * (y[i] - mean_y);
        }
        return covariance / (x.size() - 1);
    }
    Matrix<double> calculate_covariance_matrix(const std::unordered_map<std::string, std::vector<double>> &historical_prices)
    {
        Matrix<double> covariance_matrix(historical_prices.size(), historical_prices.size());
        for (int i = 0; i < historical_prices.size(); i++)
        {
            for (int j = 0; j < historical_prices.size(); j++)
            {
                covariance_matrix(i, j) = calculate_covariance(historical_prices.at(std::next(historical_prices.begin(), i)->first), historical_prices.at(std::next(historical_prices.begin(), j)->first));
            }
        }
        return covariance_matrix;
    }
    Optimization::Optimization(const std::vector<std::string> &tickers, const std::unordered_map<std::string, std::vector<double>> &historical_prices, const std::vector<double> &expected_returns,
                               const double risk_free_rate)
    {
        this->tickers = tickers;
        this->historical_prices = historical_prices;
        this->expected_returns = expected_returns;
        this->risk_free_rate = risk_free_rate;
        this->covariance_matrix = calculate_covariance_matrix(historical_prices);
    }
    Optimization::Optimization(const std::vector<std::string> &tickers, const std::unordered_map<std::string, std::vector<double>> &historical_prices, const std::vector<double> &expected_returns,
                               const double risk_free_rate,const Matrix<double>& covariance_matrix)
    {
        this->tickers = tickers;
        this->historical_prices = historical_prices;
        this->expected_returns = expected_returns;
        this->risk_free_rate = risk_free_rate;
        this->covariance_matrix = covariance_matrix;
    }
    std::vector<OptimizationResult> Optimization::minimum_risk(const std::vector<double> &wanted_returns, bool use_risk_free_rate)
    {
        Matrix<double> cov = covariance_matrix;
        std::vector<std::string> tickers = this->tickers;
        std::vector<double> expected_returns = this->expected_returns;
        if (use_risk_free_rate)
        {
            cov.cbind(0);
            cov.rbind(0);
            cov(cov.rows - 1, cov.cols - 1) = 1e-8;
            tickers.push_back("rf");
            expected_returns.push_back(risk_free_rate);
        }
        cov *= 2;
        cov.rbind(expected_returns);
        cov.rbind(1);
        std::vector<double> constraints(expected_returns.size() + 2);
        for (size_t i = 0; i < expected_returns.size()+use_risk_free_rate; i++)
        {
            if (i < expected_returns.size())
            {
                constraints[i] = expected_returns[i];
            }
            else
            {
                constraints[i] = this->risk_free_rate;
            }
        }
        constraints[expected_returns.size()+use_risk_free_rate] = 0;
        constraints[expected_returns.size() + 1+use_risk_free_rate] = 0;
        cov.cbind(constraints);
        constraints.clear();
        constraints.resize(cov.rows);
        for (size_t i = 0; i < cov.rows; i++)
        {
            constraints[i] = 1;
        }
        constraints[cov.rows - 1] = 0;
        constraints[cov.rows - 2] = 0;
        cov.cbind(constraints);
        constraints.clear();
        std::vector<OptimizationResult> results(wanted_returns.size());
        std::vector<std::thread> threads(wanted_returns.size());
        for(size_t i = 0; i < wanted_returns.size(); i++)
        {
            threads[i] = std::thread(&Optimization::minimize_risk,this, std::ref(results[i]), wanted_returns[i], std::ref(expected_returns), std::ref(cov), std::ref(tickers));
        }
        for(size_t i = 0; i < wanted_returns.size(); i++)
        {
            threads[i].join();
        }
        return results;
    }
    void Optimization::minimize_risk(OptimizationResult &result, const double wanted_return, const std::vector<double> &expected_returns, Matrix<double> &covariance, const std::vector<std::string> &tickers)
    {
        std::vector<double> constraints(covariance.rows);
        for (size_t i = 0; i < expected_returns.size(); i++)
        {
            constraints[i] = 0;
        }
        constraints[expected_returns.size()] = wanted_return;
        constraints[expected_returns.size() + 1] = 1;
        Matrix<double> solution = covariance.inverse() * constraints;
        result.weights = std::unordered_map<std::string, double>();
        Matrix<double> weights = solution.submatrix(0, 0, solution.rows - 2, 1);
        result.leverage = 0;
        for (size_t i = 0; i < expected_returns.size(); i++)
        {
            result.weights[tickers[i]] = weights(i, 0);
            result.leverage += abs(weights(i, 0));
        }
        result.expected_return = wanted_return;
        result.volatility = sqrt((weights.transpose() * covariance.submatrix(0, 0, weights.rows, weights.rows) * weights)(0, 0));
        result.lagrange_multipliers = solution.submatrix(solution.rows - 2, 0, 2, 1).as_vector();
        result.sharpe_ratio = (wanted_return - risk_free_rate) / result.volatility;
    }
}