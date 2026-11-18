#include "ui.h"

static const char* UI_DOCK_SPACE  = "##ui.dock_space";
static const char* UI_DOCK_WINDOW = "##ui.dock_window";

static ImFont* _m_fontConsolas = nullptr;
static ImFont* _m_fontMSYH     = nullptr;

static bool _m_aboutWindow = false;

void display_about(GLFWwindow* window);

// string format
const char* imgui_format(const char* param, ...)
{
    static char buf[256] = { 0 };

    va_list body;
    va_start(body, param);
    std::vsnprintf(buf, sizeof(buf), param, body);
    va_end(body);

    return buf;
}

void init_imgui(GLFWwindow* window, const char* iniSettings)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    // 配置文件
    if (iniSettings) {
        io.WantSaveIniSettings = true;
        io.IniFilename         = iniSettings;
    }
    else {
        io.WantSaveIniSettings = false;
        io.IniFilename         = nullptr;
    }

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    // io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;  // Disable Mouse Cursor

    // io.Fonts->AddFontFromFileTTF("c:/windows/fonts/msyh.ttc", 20.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    _m_fontMSYH = io.Fonts->AddFontFromFileTTF("c:/windows/fonts/msyh.ttc", 20.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    _m_fontConsolas = io.Fonts->AddFontFromFileTTF("c:/windows/fonts/consola.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());

    // For Microsoft IME, pass your HWND to enable IME positioning:
    // io.ImeWindowHandle = my_hwnd;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();
    // ImGui::StyleColorsClassic();
    ImGui::GetStyle().FrameBorderSize = 1.0f;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void imgui_shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void imgui_begin()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imgui_end()
{
    // Rendering
    ImGui::Render();

    // GLint last_program;
    // glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    // glUseProgram(0);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // glUseProgram(last_program);
}

// 主菜单
void display_main_menu(GLFWwindow* window)
{
    if (ImGui::BeginMainMenuBar()) {
        ImVec2 size = ImGui::GetItemRectSize();

        // 文件
        if (ImGui::BeginMenu(u8"文件")) {
            if (ImGui::MenuItem(u8"新建\tCtrl+N")) {
            }
            if (ImGui::MenuItem(u8"打开")) {
                // ui_open_dialog.state = glui_state::show;

                // 打开模型
                // GLFWwindow* window = glfwGetCurrentContext();
                // HWND hwnd = glfwGetWin32Window(window);
                // std::string filename = show_open(hwnd, ".", model_filter);
                // if (!filename.empty()) {
                //    //singleton<assets>::instance.
                //    hero.dispose();
                //    hero.load(filename);
                //}
            }
            if (ImGui::MenuItem(u8"保存")) {
            }
            if (ImGui::MenuItem(u8"另存为...")) {
            }
            if (ImGui::MenuItem(u8"关闭")) {
            }

            ImGui::Separator();

            if (ImGui::MenuItem(u8"退出")) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }

            ImGui::EndMenu();
        }

        // 渲染
        /*
        if (ImGui::BeginMenu(u8"渲染")) {
            ImGui::Checkbox(u8"填充", &state.fill);
            ImGui::Checkbox(u8"背面剔除", &state.cullFace);
            ImGui::Checkbox(u8"深度测试", &state.depthTest);
            ImGui::Checkbox(u8"Alpha测试", &state.alphaTest);
            ImGui::Checkbox(u8"纹理映射", &state.textureMap);
            ImGui::Checkbox(u8"光照", &state.lighting);
            ImGui::Checkbox(u8"阴影", &state.shadowMap);
            ImGui::Separator();
            if (ImGui::Checkbox(u8"垂直同步", &state.vsync)) {
                glfwSwapInterval(state.vsync ? 1 : 0);
            }

            ImGui::EndMenu();
        }
        */

        if (ImGui::BeginMenu(u8"编译")) {
            display_compile_menu();
            ImGui::EndMenu();
        }

        // help
        if (ImGui::BeginMenu(u8"帮助")) {
            if (ImGui::MenuItem(u8"关于")) {
                _m_aboutWindow = true;
            }

            ImGui::EndMenu();
        }

        // mouse
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        ImGui::Indent(size.x - 320.0f);
        ImGui::Text(u8"mouse: %0.2lf, %0.2lf", x, y);

        // fps
        float fps = ImGui::GetIO().Framerate;
        ImVec4 color;
        if (fps < 20.0f) {
            color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        }
        else if (fps < 40.0f) {
            color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        }
        else {
            color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        }
        ImGui::Indent(200.0f);
        ImGui::TextColored(color, "fps: %0.2f", fps);
    }
    ImGui::EndMainMenuBar();

    // about
    display_about(window);
}

void display_about(GLFWwindow* window)
{
    __cgl_unused(window);

    if (_m_aboutWindow) {
        if (ImGui::Begin(u8"About", &_m_aboutWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {
            ImGui::SetWindowSize(ImVec2(400.0f, 300.0f));
            ImGui::Text(u8"OpenGL Application...");
        }
        ImGui::End();
    }
}

// 获取 consolas 字体
ImFont* consolas_font()
{
    return _m_fontConsolas;
}

// 获取停靠空间 ID
ImGuiID dockspace()
{
    static ImGuiID id = ImGui::GetID(UI_DOCK_SPACE);
    return id;
}

/*
 * 停靠节点属性：
 *   ImGuiDockNodeFlags_KeepAliveOnly               停用停靠，但保持停靠空间存在
 *   ImGuiDockNodeFlags_NoDockingOverCentralNode    没有中间的停靠节点
 *   ImGuiDockNodeFlags_PassthruCentralNode         停靠空间不显示背景，但停靠的窗口 NoBackground 属性失效
 */
ImGuiID display_dock_space(
    bool* open,
    std::function<void(ImGuiID)> pfnBuildLayout,
    int dockNodeFlags
)
{
    __cgl_unused(open);

    // 获取视口大小
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    // 设置窗口参数
    int windowFlags = ImGuiWindowFlags_NoDecoration   // 无标题栏、不可改变大小、无滚动条、不可折叠
                      | ImGuiWindowFlags_NoMove       // 不可移动
                      | ImGuiWindowFlags_NoBackground // 无背景（背景透明）
                      // | ImGuiWindowFlags_MenuBar               // 菜单栏
                      | ImGuiWindowFlags_NoDocking             // 不可停靠
                      | ImGuiWindowFlags_NoBringToFrontOnFocus // 无法设置前台和聚焦
                      | ImGuiWindowFlags_NoNavFocus            // 无法通过键盘和手柄聚焦
        ;

    // 压入样式设置
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);            // 无边框
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // 无边界
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);              // 无圆角
    // ImGui::SetNextWindowBgAlpha(0.0f); // 窗口 alpha 为 0，同样可以不显示背景

    ImGui::Begin(UI_DOCK_WINDOW, 0, windowFlags); // 开始停靠窗口
    ImGui::PopStyleVar(3);                        // 弹出样式设置

    ImGuiID dockspaceID = dockspace();

    // 创建停靠空间
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) { // 判断是否开启停靠
        // 重建布局
        if (pfnBuildLayout) {
            pfnBuildLayout(dockspaceID);
        }

        // 创建停靠空间
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockNodeFlags);
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }
    ImGui::End(); // 结束停靠窗口

    return dockspaceID;
}
