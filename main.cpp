#include "pch.h"
#include "ui.h"

#include <glslang_loader.h>

#include "spirv_exsample.h" // 示例代码

#include "spirv_compiler.h"
#include "spirv_disassemble.h"

#include "spirv_tools.h"

using namespace std;
using namespace cgl;

// 显示 SPIR-V 代码反汇编
void display_spirv(const uint32_t* codes, size_t size);

// 示例 GLSL 顶点着色器
std::string vertCode = R"(
        #version 450
        #extension GL_ARB_separate_shader_objects : enable

        layout(location = 0) in vec3 inPosition;
        layout(location = 1) in vec3 inColor;

        layout(location = 0) out vec3 fragColor;

        layout(binding = 0) uniform UniformBufferObject {
            mat4 model;
            mat4 view;
            mat4 proj;
        } ubo;

        void main() {
            gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
            fragColor = inColor;
        }
    )";

const char* fragCode = R"(
        #version 450

        layout(location = 2) in vec4 frag_color;

        layout(location = 0) out vec4 FragColor;

        void main()
        {
            //mat4 m = mat4(1.0);
            //m[0][0] = sin(10.0);


            FragColor = frag_color;
        }

        )";

/*
; SPIR-V
; Version: 1.0
; Generator: Khronos Glslang Reference Front End; 11
; Bound: 13
; Schema: 0
OpCapability Shader
%1 = OpExtInstImport "GLSL.std.450"
OpMemoryModel Logical GLSL450
OpEntryPoint Fragment %4 "main" %9 %11
OpExecutionMode %4 OriginUpperLeft
OpSource GLSL 450
OpName %4 "main"
OpName %9 "FragColor"
OpName %11 "frag_color"
OpDecorate %9 Location 0
OpDecorate %11 Location 2
%2 = OpTypeVoid
%3 = OpTypeFunction %2
%6 = OpTypeFloat 32
%7 = OpTypeVector %6 4
%8 = OpTypePointer Output %7
%9 = OpVariable %8 Output
%10 = OpTypePointer Input %7
%11 = OpVariable %10 Input
%4 = OpFunction %2 None %3
%5 = OpLabel
%12 = OpLoad %7 %11
OpStore %9 %12
OpReturn
OpFunctionEnd


*/
uint32_t color_frag[] = {
    // 1114.3.0
    0x07230203, 0x00010000, 0x0008000b, 0x0000000d, 0x00000000, 0x00020011, 0x00000001, 0x0006000b,
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e, 0x00000000, 0x0003000e, 0x00000000, 0x00000001,
    0x0007000f, 0x00000004, 0x00000004, 0x6e69616d, 0x00000000, 0x00000009, 0x0000000b, 0x00030010,
    0x00000004, 0x00000007, 0x00030003, 0x00000002, 0x000001c2, 0x00040005, 0x00000004, 0x6e69616d,
    0x00000000, 0x00050005, 0x00000009, 0x67617246, 0x6f6c6f43, 0x00000072, 0x00050005, 0x0000000b,
    0x67617266, 0x6c6f635f, 0x0000726f, 0x00040047, 0x00000009, 0x0000001e, 0x00000000, 0x00040047,
    0x0000000b, 0x0000001e, 0x00000002, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002,
    0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00040020,
    0x00000008, 0x00000003, 0x00000007, 0x0004003b, 0x00000008, 0x00000009, 0x00000003, 0x00040020,
    0x0000000a, 0x00000001, 0x00000007, 0x0004003b, 0x0000000a, 0x0000000b, 0x00000001, 0x00050036,
    0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x0004003d, 0x00000007,
    0x0000000c, 0x0000000b, 0x0003003e, 0x00000009, 0x0000000c, 0x000100fd, 0x00010038
};

TextEditor editor;
std::vector<uint32_t> spirvData;

// main
int main(int argc, char* argv[])
{
    __cgl_unused(argc);
    __cgl_unused(argv);

    // init glfw window
    GLFWwindow* window = init_glfw("OpenGL", screen.x, screen.y, true);

    if (!window) {
        cout << "GLFW window create failed." << endl;
        return -1;
    }

    // imgui
    init_imgui(window);

    load_glslang();
    load_spirv_tools();

    /*
    GLSLToSPIRVCompiler spirv_comp;
    spirv_comp.compileGLSLToSPIRV(fragCode, GLSLANG_STAGE_FRAGMENT, spirvData);
    print_spirv_info(spirvData);
    print_spirv(spirvData);
    */

    // spirv_disass.disassemble(std::cout, color_frag, _countof(color_frag));

    editor.SetShowWhitespaces(false);
    editor.SetText(GLSL_EXSAMPLE_CODE);

    spirv_compiler compiler;
    compiler.compileGLSLToSPIRV(GLSL_EXSAMPLE_CODE, GLSLANG_STAGE_FRAGMENT, "main", spirvData);

    // spirv_print(std::cout, spirvData.data(), spirvData.size());
    // spirv_disassemble(std::cout, spirvData.data(), spirvData.size());
    // spirv_disassemble(std::cout, SPIRV_EXSAMPLE_CODE2, _countof(SPIRV_EXSAMPLE_CODE2));

    // main loop
    double t = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        // update
        t = glfwGetTime() - t;
        update(window, t);
        t = glfwGetTime();

        // clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // display
        display(window);
        display_ui(window);

        // swap buffers
        glfwSwapBuffers(window);

        // poll events
        glfwPollEvents();
    }

    // free variable and resource
    imgui_shutdown();

    // close window
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void reshape_event(GLFWwindow* window, int width, int height)
{
    __cgl_unused(window);

    // Window size cannot be 0
    if (width == 0 || height == 0) {
        return;
    }
    screen = vec2i(width, height);
    glViewport(0, 0, width, height);
}

void keyboard_event(GLFWwindow* window, int key, int scancode, int action, int modifier)
{
    __cgl_unused(scancode);
    __cgl_unused(modifier);

    if (action != GLFW_PRESS) {
        return;
    }

    switch (key) {
    case GLFW_KEY_ESCAPE: // press esc to exit
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    case GLFW_KEY_F1:
        break;
    case GLFW_KEY_F2:
        break;
    case GLFW_KEY_F3:
        break;
    case GLFW_KEY_F4:
        break;
    default:
        break;
    }

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows | ImGuiFocusedFlags_AnyWindow)) {
        return;
    }

    switch (key) {
    case GLFW_KEY_1:
        break;
    case GLFW_KEY_2:
        break;
    case GLFW_KEY_3:
        break;
    case GLFW_KEY_4:
        break;
    default:
        break;
    }
}

void mouse_event(GLFWwindow* window, int button, int action, int mods)
{
    __cgl_unused(window);
    __cgl_unused(mods);

    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows | ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByPopup)) {
        lastpos = mouse;
        return;
    }

    if (action == GLFW_PRESS) {
        lastpos      = mouse;
        mouse_button = button;
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            // printf("mouse left button down\n");
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            break;
        default:
            return;
        }
    }
    else if (action == GLFW_RELEASE) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            // printf("mouse left button up\n");
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            break;
        default:
            return;
        }
        mouse_button = -1;
    }
    else {
        cout << action;
    }
}

void motion_event(GLFWwindow* window, double x, double y)
{
    __cgl_unused(window);

    // printf("mouse move : %d, %d\n", int(x), int(y));
    mouse = vec2f(x, y);

    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows | ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByPopup)) {
        if (mouse_button != -1) {
            lastpos = mouse;
        }
        return;
    }

    if (mouse_button == GLFW_MOUSE_BUTTON_LEFT) {
        // vec2f offset = mouse - lastpos;

        lastpos = mouse;
    }
    else if (mouse_button == GLFW_MOUSE_BUTTON_RIGHT) {
        vec2f offset = mouse - lastpos;
        offset *= 0.05f;

        lastpos = mouse;
    }
}

void scroll_event(GLFWwindow* window, double x, double y)
{
    __cgl_unused(window);
    __cgl_unused(x);
    __cgl_unused(y);

    // printf("scroll : %0.3lf, %0.3lf\n", x, y);

    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows | ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByPopup)) {
        return;
    }

    // viewPos.z -= (float) y;
}

void update(GLFWwindow* window, double delay)
{
    __cgl_unused(window);
    __cgl_unused(delay);
}

void display(GLFWwindow* window)
{
    __cgl_unused(window);
}

void display_ui(GLFWwindow* window)
{
    glViewport(0, 0, screen.x, screen.y);

    imgui_begin();

    display_main_menu(window);

    // ImGuiID dockspaceID =
    display_dock_space();

    if (ImGui::Begin("SPIR-V")) {
        ImGui::PushFont(consolas_font());
        editor.Render(u8"编辑器"); //, ImVec2(-1.0f, -0.5f));
        ImGui::PopFont();
    }
    ImGui::End();

    if (ImGui::Begin("SPIR-V Desassemble")) {
        if (!spirvData.empty()) {
            display_spirv(spirvData.data(), spirvData.size());
        }
    }
    ImGui::End();

    imgui_end();
}

/* 反汇编规则：
 * [数量|指令][hasResult][hasResultType][...]
 *
 * [hasResult]                    [指令][参数...]
 * [hasResultType]                [指令][参数...]
 * [hasResult:hide][hasResultType][指令][参数...]
 *
 * 参数目前看来是根据不同指令解析
 *
 */
std::string format_spirv_inst(const uint32_t* codes, size_t size)
{
    std::stringstream out;

    uint32_t word = codes[0];
    // uint32_t wordCount = word >> SpvWordCountShift;
    SpvOp opcode = static_cast<SpvOp>(word & SpvOpCodeMask);

    bool hasResult     = false;
    bool hasResultType = false;
    SpvHasResultAndType(opcode, &hasResult, &hasResultType);

    size_t offset = 1;
    if (hasResult && hasResultType) {
        out << "%" << codes[2] << " = "; // 返回值类型
        offset += 2;
    }
    else if (hasResult || hasResultType) {
        out << "%" << codes[1] << " = ";
        offset += 1;
    }

    // 指令
    out << SpvOpToString(opcode);

    if (hasResult && hasResultType) {
        out << " %" << codes[1]; // 返回值
    }

    // uint32_t targetID;
    switch (opcode) {
    // SpvOpNop = 0,
    // SpvOpUndef = 1,
    // SpvOpSourceContinued = 2,
    // SpvOpSource = 3,
    // SpvOpSourceExtension = 4,
    case SpvOpName:
        out << " %" << codes[offset];
        out << " \"" << (char*) &codes[offset + 1] << "\"";
        break;
    case SpvOpMemberName:
        out << " %" << codes[offset];
        out << " " << codes[offset + 1];
        out << " \"" << (char*) &codes[offset + 2] << "\"";
        break;
        // SpvOpString = 7,
        // SpvOpLine = 8,
        // SpvOpExtension = 10,
    case SpvOpExtInstImport:
        out << " \"" << (char*) &codes[offset] << "\"";
        break;
        // SpvOpExtInst
    case SpvOpMemoryModel:
        out << " " << SpvAddressingModelToString(static_cast<SpvAddressingModel>(codes[offset]));
        out << " " << SpvMemoryModelToString(static_cast<SpvMemoryModel>(codes[offset + 1]));
        break;
    case SpvOpEntryPoint:
        out << " " << SpvExecutionModelToString(static_cast<SpvExecutionModel>(codes[offset]));
        out << " %" << codes[offset + 1];
        out << " \"" << (char*) &codes[offset + 2] << "\"";
        for (uint32_t i = size - 4; i < size; ++i) {
            out << " %" << codes[i];
        }
        break;
        // SpvOpExecutionMode = 16,
        // SpvOpCapability = 17,

    case SpvOpTypeVoid: // 值
    case SpvOpTypeBool:
    case SpvOpTypeInt:
    case SpvOpTypeFloat:
        for (; offset < size; ++offset) {
            out << " " << codes[offset];
        }
        break;
    case SpvOpTypeVector:
        out << " %" << codes[offset];
        out << " " << codes[offset + 1];
        break;
    // case SpvOpTypeMatrix:
    // case SpvOpTypeImage:
    // case SpvOpTypeSampler:
    // case SpvOpTypeSampledImage:
    // case SpvOpTypeArray:
    // case SpvOpTypeRuntimeArray:
    //     SpvOpTypeStruct: // %x ...
    // case SpvOpTypeOpaque:
    //     break;
    case SpvOpTypePointer:
        out << " " << SpvStorageClassToString(static_cast<SpvStorageClass>(codes[offset]));
        out << " %" << codes[offset];
        break;

        // SpvOpTypeFunction: // %x ...
        // case SpvOpTypeEvent:
        // case SpvOpTypeDeviceEvent:
        // case SpvOpTypeReserveId:
        // case SpvOpTypeQueue:
        // case SpvOpTypePipe:
        // case SpvOpTypeForwardPointer:
        //     break;

    case SpvOpConstant: // 43
        out << " " << *(float*) &codes[offset];
        break;

    case SpvOpFunction: // 54
        // out << " %" << codes[offset];
        out << " " << (codes[offset] ? "??" : "None");
        out << " %" << codes[offset + 1];
        break;

    case SpvOpVariable: // 59
        out << " " << SpvStorageClassToString(static_cast<SpvStorageClass>(codes[offset]));
        break;

    case SpvOpDecorate: // 71
        out << " %" << codes[offset];
        out << " " << SpvDecorationToString(static_cast<SpvDecoration>(codes[offset + 1]));
        if (size == 4) {
            out << " " << codes[offset];
        }
        break;
    case SpvOpMemberDecorate:
        out << " %" << codes[offset];
        out << " " << codes[offset + 1];
        out << " " << SpvDecorationToString(static_cast<SpvDecoration>(codes[offset + 2]));
        out << " " << codes[offset + 3];
        break;
    default:
        // 默认，输出指令
        for (; offset < size; ++offset) {
            out << " %" << codes[offset];
        }
        break;
    }

    return out.str();
}

// 显示 SPIR-V 代码反汇编
void display_spirv(const uint32_t* codes, size_t size)
{
    using namespace cgl;

    // OP + [Result] + [ResultType] + XXX

    const ImGuiTableFlags TABLE_FLAGS_SCROLL =
        ImGuiTableFlags_NoSavedSettings |
        ImGuiTableFlags_RowBg |
        // ImGuiTableFlags_SizingFixedFit |
        // ImGuiTableFlags_NoHostExtendX |
        // ImGuiTableFlags_NoHostExtendY |
        ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersV |
        ImGuiTableFlags_SizingStretchProp;

    if (ImGui::BeginTable("", 4, TABLE_FLAGS_SCROLL)) {
        ImGui::TableSetupColumn(u8"指令", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(u8"操作符", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(u8"值", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(u8"解释", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        size_t address  = 5;
        size_t instAddr = 5;

        uint32_t inst;
        SpvOp opcode;
        uint32_t count;
        bool hasResult     = false;
        bool hasResultType = false;

        for (; address < size;) {
            if (instAddr == address) {
                inst   = codes[address];
                opcode = static_cast<SpvOp>(inst & SpvOpCodeMask);
                count  = inst >> SpvWordCountShift;
                SpvHasResultAndType(opcode, &hasResult, &hasResultType);
            }

            ImGui::TableNextRow();

            //----------------------------------------------------------------
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%08X", codes[address]);

            //----------------------------------------------------------------
            if (instAddr == address) { // 指令
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", SpvOpToString(opcode));

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%u", count);

                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%s", format_spirv_inst(&codes[address], count).c_str());
            }
            else {
                //----------------------------------------------------------------
                ImGui::TableSetColumnIndex(1);
                const char* fmt = "    %s";
                if (address == instAddr + 1) {
                    if (hasResult) {
                        ImGui::Text(fmt, u8"返回值");
                    }
                    else if (hasResultType) {
                        ImGui::Text(fmt, u8"返回值类型");
                    }
                    else {
                        switch (opcode) {
                        case SpvOpName:
                            break;
                        case SpvOpEntryPoint:
                            ImGui::Text(fmt, u8"着色器类型");
                            break;
                        default:
                            break;
                        }
                    }
                }
                else if (address == instAddr + 2) {
                    if (hasResultType) {
                        ImGui::Text(fmt, u8"返回值类型");
                    }
                    else {
                        switch (opcode) {
                        case SpvOpName:
                            ImGui::Text(fmt, u8"字符串");
                            break;
                        case SpvOpEntryPoint:
                            // ImGui::Text(fmt, u8"字符串");
                            break;
                        default:
                            break;
                        }
                    }
                }
                else if (address == instAddr + 3) {
                    switch (opcode) {
                    case SpvOpName:
                        break;
                    case SpvOpEntryPoint:
                        ImGui::Text(fmt, u8"字符串");
                        break;
                    default:
                        break;
                    }
                }

                //----------------------------------------------------------------
                ImGui::TableSetColumnIndex(2);
                if (address == instAddr + 1) {
                    if (hasResult) {
                        ImGui::Text("%%%u", codes[instAddr + 1]);
                    }
                    else if (hasResultType) {
                        ImGui::Text("%%%u", codes[instAddr + 1]);
                    }
                    else {
                        switch (opcode) {
                        case SpvOpName:
                            ImGui::Text("%u", codes[instAddr + 1]);
                            break;
                        case SpvOpEntryPoint:
                            ImGui::Text("%s", SpvExecutionModelToString(static_cast<SpvExecutionModel>(codes[instAddr + 1])));
                            break;
                        default:
                            break;
                        }
                    }
                }
                else if (address == instAddr + 2) {
                    if (hasResultType) {
                        ImGui::Text("%%%u", codes[instAddr + 2]);
                    }
                    else {
                        switch (opcode) {
                        case SpvOpName: {
                            std::string str((char*) &codes[instAddr + 2], codes[instAddr + 1]);
                            ImGui::Text("%s", str.c_str());
                            break;
                        }
                        case SpvOpExtInstImport:
                            ImGui::Text("%s", (char*) &codes[instAddr + 2]);
                            break;
                        case SpvOpEntryPoint:
                            ImGui::Text("%u", codes[instAddr + 2]);
                            break;
                        default:
                            break;
                        }
                    }
                }
                else if (address == instAddr + 3) {
                    switch (opcode) {
                    case SpvOpEntryPoint:
                        ImGui::Text("%s", (char*) &codes[instAddr + 3]);
                        break;
                    default:
                        ImGui::Text("%s", "");
                        break;
                    }
                }
                else {
                    ImGui::Text("%s", "");
                }

                //----------------------------------------------------------------
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%s", "");
            }

            ++address;
            if (instAddr + count <= address) {
                instAddr += count;
            }
        }

        ImGui::EndTable();
    }
}

// 编译菜单
void display_compile_menu()
{
    if (ImGui::MenuItem(u8"编译")) {
        spirv_compiler compiler;
        std::string text = editor.GetText();
        compiler.compileGLSLToSPIRV(text, GLSLANG_STAGE_FRAGMENT, "main", spirvData);
    }

    if (ImGui::MenuItem(u8"打印代码")) {
        spirv_print(std::cout, spirvData.data(), spirvData.size());
    }

    if (ImGui::MenuItem(u8"反汇编")) {
        spirv_disassemble(std::cout, spirvData.data(), spirvData.size());
    }

    if (ImGui::MenuItem(u8"反汇编 - SPIR-V Tools")) {
        spirv_tools_disassemble(spirvData);
    }

    ImGui::Separator();
    if (ImGui::MenuItem(u8"清屏")) {
        system("cls");
    }
}
