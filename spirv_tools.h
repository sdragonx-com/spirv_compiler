#ifndef SPIRV_TOOLS_H
#define SPIRV_TOOLS_H

#include "pch.h"

#include <spirv_tools_loader.h>

// spirv-tools 输出汇编代码
void spirv_tools_disassemble(const std::vector<uint32_t>& spirvData);

struct SpirvOptOptions
{
    spv_target_env env = SPV_ENV_UNIVERSAL_1_3; // Vulkan 请用 SPV_ENV_VULKAN_1_1/1_2/1_3
    bool validate      = true;                  // 优化前是否跑 validator
    std::vector<std::string> passes;            // 空 => RegisterPerformancePasses()
};

// 输入/输出均为 SPIR-V 二进制（以 uint32_t 为单位的 word 序列，含 5 word 模块头）
bool SpirvOptimizeBinary(
    const uint32_t* words, size_t word_count,
    std::vector<uint32_t>* out,
    const SpirvOptOptions& opt = SpirvOptOptions(),
    std::string* error         = nullptr
);

// 输入/输出均为 SPIR-V 汇编文本
bool SpirvOptimizeAssembly(
    const std::string& assembly,
    std::string* out_assembly,
    const SpirvOptOptions& opt = SpirvOptOptions(),
    std::string* error         = nullptr
);


#endif // SPIRV_TOOLS_H
