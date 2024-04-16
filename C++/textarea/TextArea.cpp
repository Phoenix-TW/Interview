#include <sstream>
#include <ctime>
#include <GLFW/glfw3.h>

#include "TextArea.h"

using namespace std;

HANDLE showTextAreaThread = NULL;
clock_t startTime = 0,endTime = 0;

// IgnoreHideEvent is avoid continue called glfwHideWindow or glfwShowWindow when moving the MasterDuel Window
bool g_IgnoreHideEvent = false;
HANDLE ignoreHideEventThread = NULL;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

DWORD WINAPI StartShowTextArea(void* data)
{
    TextArea * ta = (TextArea *)data;
    if(!ta->RunRutine())
        ta->WalkAroundOpenGLProblem();
    return 0;
};

void TextArea::WalkAroundOpenGLProblem()
{
    Sleep(1000);
    SendMessage(m_mainHandler, m_WM_DLL_EVENT_MESSAGE, (WPARAM)(TriggerEventType::PrepareToClose), (LPARAM)(NULL));
}

DWORD WINAPI StartResizeWindow(void* data)
{
    TextArea * ta = (TextArea *)data;
    ta->RerunRutine();
    return 0;
}


char StringTitle[1024] = { 0 };
BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam)
{
	TCHAR szClass[256] = { 0 };
	TCHAR szString[1024] = { 0 };
	::GetWindowText(hWnd, szString, 255); //獲取窗口標題
	::GetClassName(hWnd, szClass, 255); //獲取窗口類名稱
	if (::IsWindow(hWnd) &&   //判斷是否為一個窗口
		(::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW )
	{
		
        if(!strcmp("ConsoleWindowClass",(char *)szClass))
            if(strstr((char *)szString,"MasterDuelHelper.exe"))
                strcpy(StringTitle,(char *)szString);
	}
 
	return TRUE;
}

DWORD WINAPI StartIgnoreHideEventTimer(void* data)
{
    TextArea * ta = (TextArea *)data;
    ta->IgnoreHideEventTimer();
    return 0;
};

void TextArea::IgnoreHideEventTimer()
{
    clock_t currentTime = clock();
    while(currentTime < endTime)
    {
        currentTime = clock();
    };

    /*
    // Focus On Console after move window to avoid bugs
    // codes below are finding MasterDuelHelper's console HWND and focus on it
    EnumWindows(EnumWindowProc, NULL);
    HWND MDHConsoleHandler = FindWindowA("ConsoleWindowClass",(char *)StringTitle);
    // focus on MasterDuelHelper Console start//
    SetWindowPos(MDHConsoleHandler, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOMOVE);
	PostMessage(MDHConsoleHandler, WM_SYSCOMMAND, SC_RESTORE,  NULL);
	HWND hForeWnd = ::GetForegroundWindow();
	DWORD dwForeID = ::GetWindowThreadProcessId(hForeWnd, NULL);
	DWORD dwCurID = ::GetCurrentThreadId();
	::AttachThreadInput(dwCurID, dwForeID, TRUE);
	SetForegroundWindow(MDHConsoleHandler);
	PostMessage(MDHConsoleHandler, WM_SETFOCUS, NULL, NULL);
    // focus on MasterDuelHelper Console end//
    */

    HWND MDHTextAreaHandler = FindWindowA("GLFW30","MasterDuelHelperMainTextArea");
    if(showText && !IsWindowVisible(MDHTextAreaHandler))
    {
        showOrHideisDone = false;
        warn("<Synchro Problem> showText is TRUE but isVisible is FALSE! Walkaround!");
    }

    g_IgnoreHideEvent = false;
    printf("[Debug] Focus On Console after move window to avoid bugs.\n");
}

TextArea::TextArea()
    : IFeature(__func__)
{

}

TextArea::~TextArea()
{
    // clean up

}

bool TextArea::Initial(struct SystemInfo * arguments)
{
    dbg(__FUNCTION__);
    SetState(DllState::INIT);

    m_arg = arguments;
    showText = true;
    reloadText = true;
    run_flag = false;
    needToReloadPosition = false;
    showOrHideisDone = true;

    if(!m_arg->usePotPlayer64ToDebug)
        resolutionConvertDataMap.insert(std::pair<GameResolutionType,std::tuple<int,int,int,int>>(GameResolutionType::Resolution720P,{260, 237, 17, 335}));
    else
        resolutionConvertDataMap.insert(std::pair<GameResolutionType,std::tuple<int,int,int,int>>(GameResolutionType::Resolution720P,{260+(3-1), 237+(34-38), 17+(3-1), 335+(34-38)}));
    resolutionConvertDataMap.insert(std::pair<GameResolutionType,std::tuple<int,int,int,int>>(GameResolutionType::Resolution900P,{325, 296, 22, 418}));
    resolutionConvertDataMap.insert(std::pair<GameResolutionType,std::tuple<int,int,int,int>>(GameResolutionType::Resolution1080P,{390, 355, 26, 502}));
    resolutionConvertDataMap.insert(std::pair<GameResolutionType,std::tuple<int,int,int,int>>(GameResolutionType::Resolution1440P,{520, 473, 35, 670}));
    resolutionConvertDataMap.insert(std::pair<GameResolutionType,std::tuple<int,int,int,int>>(GameResolutionType::Resolution2160P,{780, 710, 52, 1004}));
    return true;
}

bool TextArea::RunRutine()
{
    GameResolutionType currentType = m_arg->resolutionType;

    /*
    printf("PosX = %d, PosY = %d, shiftX = %d, shiftY = %d, rectX = %d, rectY = %d\n",m_arg->coordinateX,m_arg->coordinateY,
    get<2>(resolutionConvertDataMap[currentType]),get<3>(resolutionConvertDataMap[currentType]),
    get<0>(resolutionConvertDataMap[currentType]),get<1>(resolutionConvertDataMap[currentType]));
    */

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return false;
    
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //隱藏OpenGL做的TitleBar
    glfwWindowHint(GLFW_DECORATED, false);
    glfwWindowHint(GLFW_FLOATING, true);

    // Create window with graphics context
    window = glfwCreateWindow( get<0>(resolutionConvertDataMap[currentType]), 
     get<1>(resolutionConvertDataMap[currentType]), "MasterDuelHelperMainTextArea", NULL, NULL);

    if (window == NULL)
        return false;

    glfwHideWindow(window);
    glfwSetWindowPos(window, m_arg->coordinateX + get<2>(resolutionConvertDataMap[currentType]), 
    m_arg->coordinateY + get<3>(resolutionConvertDataMap[currentType]));
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImFont* font = io.Fonts->AddFontFromFileTTF("DroidSansFallback.ttf", 26.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    // For publish
    // ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msjh.ttc", 30.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

    //set window properties
    static bool no_titlebar = true;
    static bool no_resize = true;
    static bool no_move = true;
    static bool no_scrollbar = false;
    static bool no_collapse = false;
    static bool no_menu = true;

    // Demonstrate the various window flags. 
    // Typically you would just use the default.
    window_flags = 0;
    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;

    ImVec4 clear_color = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);

    glfwShowWindow(window);
    // Main loop
    while (!glfwWindowShouldClose(window) && run_flag)
    {
        if(reloadText)
        {
            text = m_arg->CardDescription;
            (m_arg->CardDescription).clear();
            reloadText = false;
        }

        //printf("[Debug] [TextArea::RunRutine()] showText = %d, showOrHideisDone = %d\n",showText, showOrHideisDone);

        if(!g_IgnoreHideEvent && !showOrHideisDone)
        {
            if(showText)
            {
                glfwShowWindow(window);
                showOrHideisDone = true;
            }
            else
            {
                glfwHideWindow(window);
                showOrHideisDone = true;
            }
        }

        if(needToReloadPosition)
        {
            glfwSetWindowPos(window, m_arg->coordinateX + get<2>(resolutionConvertDataMap[currentType]),
            m_arg->coordinateY + get<3>(resolutionConvertDataMap[currentType]));
            needToReloadPosition = false;
        }

        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(currentType == GameResolutionType::Resolution1080P)
            ImGui::Begin("TextArea1080p", &showText,  window_flags);
        if(currentType == GameResolutionType::Resolution720P)
            ImGui::Begin("TextArea720p", &showText,  window_flags);
        if(showText)
            ImGui::TextWrapped(text.c_str());
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return true;
};

bool TextArea::Run()
{
    dbg(__FUNCTION__);
    SetState(DllState::IN_SERVICE);

    run_flag = true;
    showTextAreaThread = CreateThread(NULL, 0, StartShowTextArea, (void *)this, 0, NULL);
    return true;
}

bool TextArea::Heartbeat()
{
    printf("%s: %s\n", GetName(),__FUNCTION__);
    return true;
}

bool TextArea::Reset()
{
    dbg(__FUNCTION__);
    SetState(DllState::INIT);
    return true;
}

bool TextArea::Stop()
{
    dbg(__FUNCTION__);
    SetState(DllState::OUT_OF_SERVICE);
    run_flag = false;
    return true;
}

void TextArea::Release()
{
    dbg(__FUNCTION__);
    SetState(DllState::UNKNOWN);
}

void TextArea::onEvent(TriggerEventType evt)
{

    if(evt == TriggerEventType::ShowTextArea)
    {
        //dbg(__FUNCTION__);
        cout << "\n[debug] " << GetName() << "::" << __FUNCTION__ << " (ShowTextArea)"  << endl;
        showText = true;
        showOrHideisDone = false;
    }
    if(evt == TriggerEventType::HideTextArea && !g_IgnoreHideEvent)
    {
        //dbg(__FUNCTION__);
        cout << "\n[debug] " << GetName() << "::" << __FUNCTION__ << " (HideTextArea)"  << endl;
        showText = false;
        showOrHideisDone = false;
    }
    if(evt == TriggerEventType::PrintOnGUI)
    {
        //dbg(__FUNCTION__);
        cout << "\n[debug] " << GetName() << "::" << __FUNCTION__ << " (PrintOnGUI)"  << endl;
        reloadText = true;
        if(!showText)
            showOrHideisDone = false;
        showText = true;
    }
    if(evt == TriggerEventType::PrepareToClose)
    {
        //dbg(__FUNCTION__);
        cout << "\n[debug] " << GetName() << "::" << __FUNCTION__ << " (PrepareToClose)"  << endl;
        reloadText = false;
        showText = false;
        run_flag = false;
    }
    if(evt == TriggerEventType::MoveWindow)
    {
        //dbg(__FUNCTION__);
        cout << "\n[debug] " << GetName() << "::" << __FUNCTION__ << " (MoveWindow)"  << endl;

        startTime = clock();
        endTime = startTime + 300;
        if(!g_IgnoreHideEvent)
        {
            g_IgnoreHideEvent = true;
            if(ignoreHideEventThread)
                TerminateThread(ignoreHideEventThread,0);
            ignoreHideEventThread = CreateThread(NULL, 0, StartIgnoreHideEventTimer, this, 0, NULL);
        }
            
        reloadText = false;
        needToReloadPosition = true;
    }
    if(evt == TriggerEventType::ResizeWindow)
    {
        //dbg(__FUNCTION__);
        cout << "\n[debug] " << GetName() << "::" << __FUNCTION__ << " (ResizeWindow)"  << endl;
        
        CreateThread(NULL, 0, StartResizeWindow, (void *)this, 0, NULL);
    }
}

bool TextArea::RerunRutine()
{
    run_flag = false;
    WaitForSingleObject(showTextAreaThread,INFINITE);
    showText = true;
    reloadText = true;
    showOrHideisDone = true;
    needToReloadPosition = false;
    Run();
    return 0;
}

void TextArea::OnStateChange(DllState previousState)
{
    info(string("state change ") + GetStateString(previousState) + " -> " + GetStateString());
}
