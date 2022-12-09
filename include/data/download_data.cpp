#include "download_data.hpp"
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <format>
#include <vector>
namespace portfolio_optimizer::data
{
    std::vector<double> YahooStockData::get_return(ReturnColumn column)
    {
        std::vector<double> result;
        result.reserve(date.size());
        for (int i = 1; i < date.size(); i++)
        {
            double r = 0;
            switch (column)
            {
            case ReturnColumn::Open:
                r = (open[i] - open[i - 1]) / open[i - 1];
                break;
            case ReturnColumn::High:
                r = (high[i] - high[i - 1]) / high[i - 1];
                break;
            case ReturnColumn::Low:
                r = (low[i] - low[i - 1]) / low[i - 1];
                break;
            case ReturnColumn::Close:
                r = (close[i] - close[i - 1]) / close[i - 1];
                break;
            case ReturnColumn::AdjClose:
                r = (adj_close[i] - adj_close[i - 1]) / adj_close[i - 1];
                break;
            case ReturnColumn::Volume:
                r = (volume[i] - volume[i - 1]) / volume[i - 1];
                break;
            }
            result.push_back(r);
        }
        return result;
    }
    std::string YahooStockData::to_string()
    {
        std::string result;
        for (int i = 0; i < date.size(); i++)
        {
            result += std::format("{} {} {} {} {} {} {} {}\n", symbol, date_util.to_string(date[i]), open[i], high[i], low[i], close[i], adj_close[i], volume[i]);
        }
        return result;
    }
    void YahooStockData::reserve(size_t size)
    {
        date.reserve(size);
        open.reserve(size);
        high.reserve(size);
        low.reserve(size);
        close.reserve(size);
        adj_close.reserve(size);
        volume.reserve(size);
    }
    double fast_stod(const std::string &str)
    {
        const char *p = str.c_str();
        double r = 0.0;
        bool neg = false;
        if (*p == '-')
        {
            neg = true;
            ++p;
        }
        while (*p >= '0' && *p <= '9')
        {
            r = (r * 10.0) + (*p - '0');
            ++p;
        }
        if (*p == '.')
        {
            double f = 0.0;
            int n = 0;
            ++p;
            while (*p >= '0' && *p <= '9')
            {
                f = (f * 10.0) + (*p - '0');
                ++p;
                ++n;
            }
            r += f / std::pow(10.0, n);
        }
        if (neg)
        {
            r = -r;
        }
        return r;
    }
    size_t WriteCallback(char *contents, size_t size, size_t nmemb, std::string *userp)
    {
        userp->append(contents, size * nmemb);
        return size * nmemb;
    }
    YahooStockData download_yahoo_data(const std::string &symbol, const std::time_t &start, const std::time_t &end,const bool verbose)
    {
        std::string readBuffer;
        CURL *curl = curl_easy_init();
        if (curl)
        {
            CURLcode res;
            std::string url_formatted = std::format("https://query1.finance.yahoo.com/v7/finance/download/{}?period1={}&period2={}&interval=1d&events=history&includeAdjustedClose=true", symbol, start, end);
            curl_easy_setopt(curl, CURLOPT_URL, url_formatted.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            int http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            if (res != CURLE_OK || http_code != 200)
            {
                std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
                std::cout << "http_code: " << http_code << "\n";
                std::cout << "url: " << url_formatted << "\n";
                throw std::runtime_error("curl_easy_perform() failed");
            }
            if(verbose)
            {
                std::cout << "Downloaded data for " << symbol << " from " << date_util.to_string(start) << " to " << date_util.to_string(end) << "\n";
            }
            curl_easy_cleanup(curl);
        }
        YahooStockData result;
        result.reserve(std::count(readBuffer.begin(), readBuffer.end(), '\n'));
        result.symbol = symbol;
        std::string line;
        std::istringstream f(readBuffer);
        int i = 0;
        while (std::getline(f, line))
        {
            if (i == 0)
            {
                i++;
                continue;
            }
            std::vector<std::string> tokens(7);
            std::string token;
            std::istringstream f(line);
            int j = 0;
            while (std::getline(f, token, ','))
            {
                tokens[j] = token;
                j++;
            }
            result.date.push_back(date_util.to_time_t(tokens[0]));
            result.open.push_back(stod(tokens[1]));
            result.high.push_back(stod(tokens[2]));
            result.low.push_back(stod(tokens[3]));
            result.close.push_back(stod(tokens[4]));
            result.adj_close.push_back(stod(tokens[5]));
            result.volume.push_back(stod(tokens[6]));
        }
        return result;
    }
}