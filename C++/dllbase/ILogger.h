#ifndef __ILOGGER_H__
#define __ILOGGER_H__

#include <string>

enum class LogState
{
    Info,
    Warn,
    Error,
    Success,
    Debug,
};


class ILogger
{
public:
    /**
    * @brief Logging to file
    */
    virtual int Log(const LogState type, const std::string &tag, const std::string &msg) = 0;
};

#endif // !__ILOGGER_H__
