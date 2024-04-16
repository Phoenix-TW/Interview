#ifndef __IMAGERECOGNITION_H__
#define __IMAGERECOGNITION_H__

#if defined(DLLLIBRARY_IMPORT)
    #define DLLAPI __declspec(dllimport)
#else
    #define DLLAPI __declspec(dllexport)
#endif

#include <map>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../dllbase/IFeature.h"

class TextArea : public IFeature
{
public:
    TextArea();
    ~TextArea();

    bool Initial(struct SystemInfo * args);
    bool Run();
    bool Heartbeat();
    bool Reset();
    bool Stop();
    void Release();
    void onEvent(TriggerEventType evt);
    bool RunRutine();
    bool RerunRutine();
    void IgnoreHideEventTimer();
    void WalkAroundOpenGLProblem();

private:
    struct SystemInfo * m_arg = NULL;
    ImGuiWindowFlags window_flags;
    bool showText;
    bool reloadText;
    bool run_flag;
    bool needToReloadPosition;
    bool showOrHideisDone;
    std::string text;
    GLFWwindow* window;
    std::map<GameResolutionType,std::tuple<int,int,int,int>> resolutionConvertDataMap;
protected:
    void OnStateChange(DllState previousState);
};

extern "C" DLLAPI TextArea * __stdcall GetControlBlock();

#endif