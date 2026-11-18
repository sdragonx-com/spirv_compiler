/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 spirv_compiler.h

 2026-11-18 03:41:09

*/
#ifndef SPIRV_COMPILER_H_20261118034109
#define SPIRV_COMPILER_H_20261118034109

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

// glslang C接口
// #include <glslang/SPIRV/GLSL.std.450.h>
// #include <glslang/SPIRV/doc.h>
// #include <glslang/SPIRV/disassemble.h>

// glslang C接口头文件
#include <glslang/Include/glslang_c_interface.h>
#include <glslang/Public/resource_limits_c.h>

namespace cgl {

// 根据文件扩展名确定色器类型
glslang_stage_t shader_stage_from_extension(const std::string& filename);

// 根据着色器类型字符串获取类型枚举
glslang_stage_t shader_stage_from_string(const std::string& shaderType);

// OpenGL 着色器类型转换
glslang_stage_t shader_stage_from_gl(int shaderType);
int shader_stage_to_gl(glslang_stage_t shaderLang);

//
// spirv_compiler
//

class spirv_compiler
{
public:
    spirv_compiler()
    {
        // 初始化 glslang
        glslang_initialize_process();
    }

    ~spirv_compiler()
    {
        // 清理 glslang
        glslang_finalize_process();
    }

    // 编译 GLSL 代码为 SPIR-V
    bool compileGLSLToSPIRV(
        const std::string& glslCode,
        glslang_stage_t shaderType,
        const std::string& entryPoint,
        std::vector<uint32_t>& spirvData,
        const glslang_input_t* inputConfig = nullptr
    )
    {
        // 设置默认输入配置
        glslang_input_t defaultInput = { };
        if (!inputConfig) {
            defaultInput.language                          = GLSLANG_SOURCE_GLSL;
            defaultInput.stage                             = shaderType;
            defaultInput.client                            = GLSLANG_CLIENT_VULKAN;
            defaultInput.client_version                    = GLSLANG_TARGET_VULKAN_1_2;
            defaultInput.target_language                   = GLSLANG_TARGET_SPV;
            defaultInput.target_language_version           = GLSLANG_TARGET_SPV_1_5;
            defaultInput.code                              = glslCode.c_str();
            defaultInput.default_version                   = 450; // GLSL 版本
            defaultInput.default_profile                   = GLSLANG_NO_PROFILE;
            defaultInput.force_default_version_and_profile = false;
            defaultInput.forward_compatible                = false;
            defaultInput.messages =
                // GLSLANG_MSG_DEFAULT_BIT;
                (glslang_messages_t) (GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT);
            defaultInput.resource = glslang_default_resource();

            inputConfig = &defaultInput;
        }

        // 创建着色器
        glslang_shader_t* shader = glslang_shader_create(inputConfig);
        if (!shader) {
            std::cerr << "shader create failed." << std::endl;
            return false;
        }

        // 设置入口点，无法设置
        glslang_shader_set_entry_point(shader, entryPoint.c_str());

        // 预处理
        if (!glslang_shader_preprocess(shader, inputConfig)) {
            std::cerr << "GLSL preprocess failed:" << std::endl;
            std::cerr << glslang_shader_get_info_log(shader) << std::endl;
            glslang_shader_delete(shader);
            return false;
        }

        // 词法分析和语法分析
        if (!glslang_shader_parse(shader, inputConfig)) {
            std::cerr << "GLSL parse failed:" << std::endl;
            std::cerr << glslang_shader_get_info_log(shader) << std::endl;
            glslang_shader_delete(shader);
            return false;
        }

        // 获取中间表示
        glslang_program_t* program = glslang_program_create();
        if (!program) {
            std::cerr << "program create failed." << std::endl;
            glslang_shader_delete(shader);
            return false;
        }

        glslang_program_add_shader(program, shader);

        // 链接程序
        if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT)) {
            std::cerr << "program link failed:" << std::endl;
            std::cerr << glslang_program_get_info_log(program) << std::endl;
            glslang_program_delete(program);
            glslang_shader_delete(shader);
            return false;
        }

        // 设置 SPIR-V 生成选项
        #if 0
        glslang_program_SPIRV_generate(program, shaderType); // 无优化生成
        #else
        glslang_spv_options_t spv_options;
        std::memset(&spv_options, 0, sizeof(spv_options));
        // spv_options.optimize_performance             = true;  // 等价 -O
        spv_options.optimize_size                    = true;  // 等价 -Os
        spv_options.disable_optimizer                = false; // 禁用优化
        spv_options.validate                         = true;  // 验证
        spv_options.disassemble                      = false; // 终端输出反汇编
        spv_options.optimize_allow_expanded_id_bound = false; // 允许扩展索引上限
        glslang_program_SPIRV_generate_with_options(program, shaderType, &spv_options);
        #endif

        // 获取 SPIR-V 数据
        const uint32_t* pSpirvData = glslang_program_SPIRV_get_ptr(program);
        size_t spirvSize           = glslang_program_SPIRV_get_size(program);

        if (spirvSize == 0 || pSpirvData == nullptr) {
            std::cerr << "SPIR-V generate failed." << std::endl;
            glslang_program_delete(program);
            glslang_shader_delete(shader);
            return false;
        }

        // 复制到输出向量
        spirvData.assign(pSpirvData, pSpirvData + spirvSize);

        // 获取 SPIR-V 文本信息
        const char* spirvMessages = glslang_program_SPIRV_get_messages(program);
        if (spirvMessages && strlen(spirvMessages) > 0) {
            std::cout << "SPIR-V 生成消息: " << spirvMessages << std::endl;
        }

        // 清理资源
        glslang_program_delete(program);
        glslang_shader_delete(shader);

        return !spirvData.empty();
    }

    // 从文件读取 GLSL 并编译
    bool compileFromFile(
        const std::string& filename,
        const std::string& shaderType,
        const std::string& entryPoint,
        std::vector<uint32_t>& spirvData,
        const glslang_input_t* inputConfig = nullptr
    )
    {
        // 读取GLSL文件
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "file open failed: " << filename << std::endl;
            return false;
        }

        std::string glslCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // 获取着色器类型
        glslang_stage_t type = shader_stage_from_string(shaderType);

        // 使用自定义输入配置或创建默认配置
        glslang_input_t defaultInput = { };
        glslang_input_t* config      = nullptr;

        if (inputConfig) {
            config = const_cast<glslang_input_t*>(inputConfig);
            // 确保配置中的代码指针指向我们的 GLSL 代码
            glslang_input_t* mutableConfig = const_cast<glslang_input_t*>(inputConfig);
            mutableConfig->code            = glslCode.c_str();
            mutableConfig->stage           = type;
        }
        else {
            defaultInput.language                          = GLSLANG_SOURCE_GLSL;
            defaultInput.stage                             = type;
            defaultInput.client                            = GLSLANG_CLIENT_VULKAN;
            defaultInput.client_version                    = GLSLANG_TARGET_VULKAN_1_0;
            defaultInput.target_language                   = GLSLANG_TARGET_SPV;
            defaultInput.target_language_version           = GLSLANG_TARGET_SPV_1_0;
            defaultInput.code                              = glslCode.c_str();
            defaultInput.default_version                   = 450;
            defaultInput.default_profile                   = GLSLANG_NO_PROFILE;
            defaultInput.force_default_version_and_profile = false;
            defaultInput.forward_compatible                = false;
            defaultInput.messages                          = GLSLANG_MSG_DEFAULT_BIT;
            defaultInput.resource                          = glslang_default_resource();
            config                                         = &defaultInput;
        }

        // 编译
        return compileGLSLToSPIRV(glslCode, type, entryPoint, spirvData, config);
    }

    // 保存 SPIR-V 到文件
    bool saveSPIRVToFile(const std::vector<uint32_t>& spirv, const std::string& filename)
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "create SPIR-V file failed: " << filename << std::endl;
            return false;
        }

        // 写入 SPIR-V 数据
        file.write(reinterpret_cast<const char*>(spirv.data()), spirv.size() * sizeof(uint32_t));
        file.close();

        std::cout << "SPIR-V save: " << filename
                  << " (" << spirv.size() * sizeof(uint32_t) << " bytes)" << std::endl;
        return true;
    }

    // 读取 SPIR-V 文件
    bool readSPIRVFromFile(const std::string& filename, std::vector<uint32_t>& spirv)
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "open SPIR-V file failed: " << filename << std::endl;
            return false;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        if (size % sizeof(uint32_t) != 0) {
            std::cerr << "SPIR-V file size error." << std::endl;
            return false;
        }

        spirv.resize(size / sizeof(uint32_t));
        file.read(reinterpret_cast<char*>(spirv.data()), size);
        file.close();

        return true;
    }
};

} // end namespace cgl

#endif // SPIRV_COMPILER_H_20261118034109
