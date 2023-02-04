#pragma once
#include <fstream>
#include <chrono>

class BaseLog
{
private:
    static void print(std::ofstream& ofs) {}

    template<typename T, typename ...Args>
    static void print(std::ofstream& ofs, const T& first, const Args&... rest) {
        ofs << first;
        print(ofs, rest...);
    }
public:
    template<typename ...Args>
    static void Output(const Args&... all)
    {
        std::ofstream log_ofs;
        log_ofs.open("log.txt", std::ofstream::out | std::ofstream::app);
        print(log_ofs, std::chrono::system_clock::now().time_since_epoch().count() / std::chrono::system_clock::period::den, "\t", all...);
        print(log_ofs, '\n');
        log_ofs.close();
    }

    template<typename ...Args>
    static void JsonOutput(const Args&... all)
    {
        std::ofstream log_ofs;

        tm stTm;
        auto RTC = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        localtime_s(&stTm, &RTC);
        std::string file_name = 
            std::to_string(stTm.tm_year + 1900) + "_" +
            std::to_string(stTm.tm_mon + 1) + "_" +
            std::to_string(stTm.tm_mday) + "_" +
            std::to_string(stTm.tm_hour) + "_" +
            std::to_string(stTm.tm_min) + "_" +
            std::to_string(stTm.tm_sec) +
            ".json";
        log_ofs.open(file_name, std::ofstream::out | std::ofstream::app);
        print(log_ofs, all...);
        print(log_ofs, '\n');
        log_ofs.close();
    }
};
