#pragma once
#include <string>

class Logger {
public:
    static Logger& Instance();
    void LogError(const std::string& text);
    void LogInfo(const std::string& text);

private:
    Logger() = default;
};