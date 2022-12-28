#pragma once
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <iosfwd>
#include <iomanip>
#include <optional>
namespace portfolio_optimizer::data
{
    class datetime
    {
    public:
        datetime();
        datetime(const std::time_t &time);
        datetime(const std::string &time);
        datetime(const datetime &time);
        std::time_t to_time_t();
        std::time_t to_time_t(const std::string &date);
        std::string to_string(const std::string &format);
        std::string to_string(const char *format);
        std::string to_string(const std::time_t &time);
        std::string to_string(const std::time_t &time, const std::string &format);
        std::string to_string(const std::time_t &time, const char *format);
        std::string to_string();
        std::time_t now();
        std::time_t add_time(const std::time_t &time, const int &years = 0, const int &months = 0,
                             const int &days = 0, const int &hours = 0, const int &minutes = 0,
                             const int &seconds = 0);
        int64_t to_numeric();
        int64_t to_numeric(const std::time_t &time);
        int64_t to_numeric(const std::string &date);

    private:
        std::optional<std::time_t> time;
        std::optional<std::string> time_string;
    };
}