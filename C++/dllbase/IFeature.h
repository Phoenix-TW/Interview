#ifndef __IFEATURE_H__
#define __IFEATURE_H__

#include <windows.h>
#include <iostream>
#include <sstream>

#include "ILoggerRewrite.h"
#include "../../config/TriggerEventDef.h"
#include "../../config/SystemInfoDef.h"

enum class DllState
{
    UNKNOWN,
    INIT,
    IN_SERVICE,
    OUT_OF_SERVICE,
};

class IFeature : public ILoggerRewrite
{
private:
    DllState mState;
    std::string m_featureName;
    void AddHandler(HWND handler){m_mainHandler = handler;};
    void AddEventMessage(UINT WINDOW_MESSAGE_DLL_EVENT_MESSAGE){m_WINDOW_MESSAGE_DLL_EVENT_MESSAGE = WINDOW_MESSAGE_DLL_EVENT_MESSAGE;};

public:
    IFeature(std::string name)
    {
        m_featureName = name;
    };
    ~IFeature(){};

    void Append(ILogger* logger, HWND handler, UINT WINDOW_MESSAGE_DLL_EVENT_MESSAGE)
    {
        setLogger(logger);
        AddHandler(handler);
        AddEventMessage(WINDOW_MESSAGE_DLL_EVENT_MESSAGE);
    };

    inline std::string GetStateString()
    {
        return GetStateString(mState);
    }

    const std::string GetStateString(DllState thisState)
    {
        static const char * stateString[] = 
        {
            "UNKNOWN",
            "INIT",
            "IN_SERVICE",
            "OUT_OF_SERVICE"
        };

        if (thisState >= DllState::UNKNOWN && thisState <= DllState::OUT_OF_SERVICE)
            return stateString[int(thisState)];
        else
            return "UNKNOWN";
    }

    inline char * GetName()
    {
        static char charName[32];
        memset(charName, '\0', sizeof(charName));
        strcpy_s(charName, m_featureName.c_str());
        return charName;
    };

    inline DllState GetState() { return mState; };

    virtual bool Initial(SystemInfo *args) = 0;
    virtual bool Run() = 0;
    virtual bool Heartbeat() = 0;
    virtual bool Reset() = 0;
    virtual bool Stop() = 0;
    virtual void Release() = 0;
    virtual void onEvent(TriggerEventType evt) = 0;

protected:

    HWND m_mainHandler = NULL;
    UINT m_WINDOW_MESSAGE_DLL_EVENT_MESSAGE;

    void SetState(DllState newState)
    {
        if (newState != mState)
        {
            DllState previousState = mState;
            mState = newState;
            OnStateChange(previousState);
        }
    }

    virtual void OnStateChange(DllState previousState) = 0;
};


#endif