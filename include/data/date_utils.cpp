#include "date_utils.hpp"
namespace portfolio_optimizer::data
{
    datetime::datetime()
    {
        time = std::nullopt;
        time_string = std::nullopt;
    }
    datetime::datetime(const std::time_t &time)
    {
        this->time = time;
        time_string = std::nullopt;
    }
    datetime::datetime(const std::string &time)
    {
        this->time_string = time;
        this->time = std::nullopt;
    }
    datetime::datetime(const datetime &time)
    {
        this->time = time.time;
        this->time_string = time.time_string;
    }
    std::time_t datetime::to_time_t()
    {
        if (time.has_value())
        {
            return time.value();
        }
        else if (time_string.has_value())
        {
            std::tm tm = {};
            std::istringstream ss(time_string.value());
            ss >> std::get_time(&tm, "%Y-%m-%d");
            if (ss.fail())
            {
                throw std::runtime_error("Failed to convert string to time_t");
            }
            return mktime(&tm);
        }
        else
        {
            throw std::runtime_error("No time or time string set");
        }
    }
    std::time_t datetime::to_time_t(const std::string &date)
    {
        std::tm tm = {};
        std::istringstream ss(date);
        ss >> std::get_time(&tm, "%Y-%m-%d");
        if (ss.fail())
        {
            throw std::runtime_error("Failed to convert string to time_t");
        }
        return std::mktime(&tm);
    }
    std::string datetime::to_string(const std::string &format)
    {
        if (time.has_value())
        {
            std::time_t t = time.value();
            std::tm* tm = new std::tm();
            localtime_s(tm, &t);
            char buffer[80];
            std::strftime(buffer, sizeof(buffer), format.c_str(), tm);
            return std::string(buffer);
        }
        else if (time_string.has_value())
        {
            return time_string.value();
        }
        else
        {
            throw std::runtime_error("No time or time string set");
        }
    }
    std::string datetime::to_string(const char *format)
    {
        if (time.has_value())
        {
            std::time_t t = time.value();
            std::tm *tm = new std::tm();
            localtime_s(tm, &(time.value()));
            char buf[80];
            std::strftime(buf, sizeof(buf), format, tm);
            return std::string(buf);
        }
        else if (time_string.has_value())
        {
            return time_string.value();
        }
        else
        {
            throw std::runtime_error("No time or time string set");
        }
    }
    std::string datetime::to_string(const std::time_t &time)
    {
        std::tm *tm = new std::tm();
        localtime_s(tm, &time);
        char buf[80];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm);
        return std::string(buf);
    }
    std::string datetime::to_string(const std::time_t &time, const std::string &format)
    {
        std::tm *tm = new std::tm();
        localtime_s(tm, &time);
        std::unique_ptr<char[]> buf(new char[format.length() + 1]);
        std::strftime(buf.get(), sizeof(buf.get()), format.c_str(), tm);
        return std::string(buf.get());
    }
    std::string datetime::to_string(const std::time_t &time, const char *format)
    {
        std::tm *tm = new std::tm();
        localtime_s(tm, &time);
        char buf[80];
        std::strftime(buf, sizeof(buf), format, tm);
        return std::string(buf);
    }
    std::string datetime::to_string()
    {
        if (time.has_value())
        {
            std::time_t t = time.value();
            std::tm *tm = new std::tm();
            localtime_s(tm, &t);
            char buf[80];
            std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm);
            return std::string(buf);
        }
        else if (time_string.has_value())
        {
            return time_string.value();
        }
        else
        {
            throw std::runtime_error("No time or time string set");
        }
    }
    std::time_t datetime::now()
    {
        return std::time(nullptr);
    }
    std::time_t datetime::add_time(const std::time_t &time, const int &years,
                                   const int &months, const int &days, const int &hours,
                                   const int &minutes, const int &seconds)
    {
        std::tm *tm = new std::tm();
        localtime_s(tm, &time);
        tm->tm_year += years;
        tm->tm_mon += months;
        tm->tm_mday += days;
        tm->tm_hour += hours;
        tm->tm_min += minutes;
        tm->tm_sec += seconds;
        return std::mktime(tm);
    }
    int64_t datetime::to_numeric()
    {
        if (time.has_value())
        {
            return to_numeric(time.value());
        }
        else if (time_string.has_value())
        {
            return to_numeric(time_string.value());
        }
        else
        {
            throw std::runtime_error("No time or time string set");
        }
    }
    int64_t datetime::to_numeric(const std::time_t &time)
    {
        return static_cast<int64_t>(time);
    }
    int64_t datetime::to_numeric(const std::string &time)
    {
        return to_numeric(to_time_t(time));
    }
}