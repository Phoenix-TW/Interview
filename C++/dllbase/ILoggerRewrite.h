#ifndef __ILOGGERREWRITE_H__
#define __ILOGGERREWRITE_H__

#include <string>

#include "ILogger.h"

class ILoggerRewrite
{
public:
    ILogger *getLogger() { return m_pLogger; };
    inline void setLogger(ILogger *logger) { m_pLogger = logger; };

    void dbg(std::string msg)
    {
        if (!m_pLogger)
            return;
        m_pLogger->Log(LogState::Debug, GetName(), msg);
    };

    void info(std::string msg)
    {
        if (!m_pLogger)
            return;
        m_pLogger->Log(LogState::Info, GetName(), msg);
    }

    void warn(std::string msg)
    {
        if (!m_pLogger)
            return;
        m_pLogger->Log(LogState::Warn, GetName(), msg);
    }

    void err(std::string msg)
    {
        if (!m_pLogger)
            return;
        m_pLogger->Log(LogState::Error, GetName(), msg);
    }

    virtual char * GetName() = 0;

protected:
    ILogger *m_pLogger;
};

#endif
