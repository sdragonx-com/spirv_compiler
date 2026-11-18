#pragma once

#include "pch.h"

#include <TextEditor.h>

void init_imgui(GLFWwindow* window, const char* iniSettings = "imgui.ini");
void imgui_shutdown();

void imgui_begin();
void imgui_end();

// string format
const char* imgui_format(const char* param, ...);

// 主菜单
void display_main_menu(GLFWwindow* window);

// 编译菜单
void display_compile_menu();

// 获取 consolas 字体
ImFont* consolas_font();

// 获取停靠空间 ID
ImGuiID dockspace();

/*
 * 停靠节点属性：
 *   ImGuiDockNodeFlags_KeepAliveOnly               停用停靠，但保持停靠空间存在
 *   ImGuiDockNodeFlags_NoDockingOverCentralNode    没有中间的停靠节点
 *   ImGuiDockNodeFlags_PassthruCentralNode         停靠空间不显示背景，但停靠的窗口 NoBackground 属性失效
 */
ImGuiID display_dock_space(
    bool* open = nullptr,
    // PFN_BUILD_LAYOUT pfnBuildLayout = build_docspace_layout,
    std::function<void(ImGuiID)> pfnBuildLayout = nullptr,
    int dockNodeFlags                           = ImGuiDockNodeFlags_None
);
