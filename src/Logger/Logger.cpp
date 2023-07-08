#include <string>
#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

// everything we *declare* in .h needs to be *defined* here.
std::vector<LogEntry> Logger::messages;

string CurrentDataTimeToString()
{
    static time_t givemetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    string timeStr = ctime(&givemetime);
    timeStr.erase(timeStr.length() - 1);
    return timeStr;
}

void Logger::Log(const std::string &message)
{
    LogEntry LogEntry;
    LogEntry.type = LOG_INFO;
    LogEntry.message = message;
    cout << "\033[0;32m"
         << "LOG | " << CurrentDataTimeToString() << " - " << message << "\033[0m" << endl;
    messages.push_back(LogEntry);
}

void Logger::Err(const std::string &message)
{
    LogEntry LogEntry;
    LogEntry.type = LOG_ERROR;
    LogEntry.message = message;
    cerr << "\033[0;31m"
         << "ERR | " << CurrentDataTimeToString() << " - " << message << "\033[0m" << endl;
    messages.push_back(LogEntry);
}
