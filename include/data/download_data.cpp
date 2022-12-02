#include "download_data.hpp"
namespace portfolio_optimizer::data
{
    DownloadData::DownloadData()
    {
        curl = curl_easy_init();
    }
    DownloadData::~DownloadData()
    {
        curl_easy_cleanup(curl);
    }
    const std::string DownloadData::download_yahoo_data(const std::string& symbol,const std::time_t& start,const std::time_t& end)
    {
        std::string readBuffer;
        if (curl)
        {
            
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &DownloadData::WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
            }
        }
        return readBuffer;
    }
    size_t DownloadData::WriteCallback(char *contents, size_t size, size_t nmemb, std::string *userp)
    {
        userp->append(contents, size * nmemb);
        return size * nmemb;
    }
}