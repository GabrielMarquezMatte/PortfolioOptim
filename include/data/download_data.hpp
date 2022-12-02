#pragma once
#include <curl/curl.h>
#include <string>
#include "date_utils.hpp"
#include <iostream>
#include <format>
namespace portfolio_optimizer::data
{
    class DownloadData
    {
    public:
        DownloadData();
        ~DownloadData();
        const std::string download_yahoo_data(const std::string &symbol,
                                              const std::time_t& start = date_util.add_time(date_util.now(), -5),
                                              const std::time_t& end = date_util.now());

    private:
        CURL *curl;
        CURLcode res;
        size_t WriteCallback(char *contents, size_t size, size_t nmemb, std::string *userp);
        const std::string url = "https://query1.finance.yahoo.com/v7/finance/download/{}?period1={}&period2={}&interval=1d&events=history&includeAdjustedClose=true";
        static datetime date_util;
    };
}