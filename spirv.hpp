/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 spirv.hpp

 2026-02-04 21:38:23

*/
#ifndef SPIRV_HPP_20260204213823
#define SPIRV_HPP_20260204213823

#include <iostream>
#include <iomanip>
#include <vector>

// #define SPV_ENABLE_UTILITY_CODE
#include <spirv_cross/spirv.h>
#include <spirv_cross/GLSL.std.450.h>

namespace cgl {

// generators
// https://github.com/KhronosGroup/SPIRV-Headers
typedef enum
{
    SPV_GENERATOR_KHRONOS,                 // Khronos
    SPV_GENERATOR_LUNARG,                  // LunarG
    SPV_GENERATOR_VALVE,                   // Valve
    SPV_GENERATOR_CODEPLAY,                // Codeplay
    SPV_GENERATOR_NVIDIA,                  // NVIDIA
    SPV_GENERATOR_ARM,                     // ARM
    SPV_GENERATOR_KHRONOS_LLVM_TO_SPIRV,   // Khronos LLVM/SPIR-V Translator
    SPV_GENERATOR_KHRONOS_SPIRV_ASSEMBLER, // Khronos SPIR-V Tools Assembler
    SPV_GENERATOR_KHRONOS_GLSLANG,         // Khronos Glslang Reference Front End
    SPV_GENERATOR_QUALCOMM,                // Qualcomm
    SPV_GENERATOR_AMD,                     // AMD
    SPV_GENERATOR_INTEL,                   // Intel
    SPV_GENERATOR_IMAGINATION,             // Imagination
    SPV_GENERATOR_GOOGLE_SHADERC,          // Google Shaderc over Glslang
    SPV_GENERATOR_GOOGLE_SPIREGG,          // Google spiregg
    SPV_GENERATOR_GOOGLE_RSPIRV,           // Google rspirv
    SPV_GENERATOR_X_LEGEND,                // X-LEGEND Mesa-IR/SPIR-V Translator
    SPV_GENERATOR_KHRONOS_SPIRV_LINKER,    // Khronos SPIR-V Tools Linker
    SPV_GENERATOR_WINE,                    // Wine VKD3D Shader Compiler
    SPV_GENERATOR_TELLUSIM,                // Tellusim Clay Shader Compiler
    SPV_GENERATOR_W3C_WEBGPU_GROUP,        // W3C WebGPU Group WHLSL Shader Translator
    SPV_GENERATOR_GOOGLE_CLSPV,            // Google Clspv
    SPV_GENERATOR_Google_MLIR,             // Google MLIR SPIR-V Serializer
    SPV_GENERATOR_GOOGLE_TINT,             // Google Tint Compiler
    SPV_GENERATOR_GOOGLE_ANGLE,            // Google ANGLE Shader Compiler
    SPV_GENERATOR_NETEASE_GAMES,           // Netease Games Messiah Shader Compiler
    SPV_GENERATOR_XENIA,                   // Xenia Xenia Emulator Microcode Translator
    SPV_GENERATOR_EMBARK_STUDIOS,          // Embark Studios Rust GPU Compiler Backend
    SPV_GENERATOR_GFX_RS_COMMUNITY,        // gfx-rs community Naga
    SPV_GENERATOR_MIKKOSOFT_PRODUCTIONS,   // Mikkosoft Productions MSP Shader Compiler
    SPV_GENERATOR_SPVGENTWO_COMMUNITY,     // SpvGenTwo community SpvGenTwo SPIR-V IR Tools
    SPV_GENERATOR_GOOGLE_SKIA,             // Google Skia SkSL
    SPV_GENERATOR_TORNADOVM,               // TornadoVM SPIRV Beehive Toolkit
    SPV_GENERATOR_DRAGONJOKER,             // DragonJoker ShaderWriter
    SPV_GENERATOR_RAYAN_HATOUT,            // Rayan Hatout SPIRVSmith
    SPV_GENERATOR_SAARLAND_UNIVERSITY,     // Saarland University Shady
    SPV_GENERATOR_TAICHI,                  // Taichi Graphics Taichi
} SpvGeneratorID;

// generator name
inline const char* spirv_generator_to_string(uint32_t id)
{
    // Represents a vendor tool entry in the SPIR-V XML Registry.
    const struct VendorTool
    {
        uint32_t value;
        const char* vendor;
        const char* tool;        // Might be empty string.
        const char* vendor_tool; // Combination of vendor and tool.
    } vendor_tools[] = {
        {  0,               "Khronos",                                    "",                                       "Khronos" },
        {  1,                "LunarG",                                    "",                                        "LunarG" },
        {  2,                 "Valve",                                    "",                                         "Valve" },
        {  3,              "Codeplay",                                    "",                                      "Codeplay" },
        {  4,                "NVIDIA",                                    "",                                        "NVIDIA" },
        {  5,                   "ARM",                                    "",                                           "ARM" },
        {  6,               "Khronos",              "LLVM/SPIR-V Translator",                "Khronos LLVM/SPIR-V Translator" },
        {  7,               "Khronos",              "SPIR-V Tools Assembler",                "Khronos SPIR-V Tools Assembler" },
        {  8,               "Khronos",         "Glslang Reference Front End",           "Khronos Glslang Reference Front End" },
        {  9,              "Qualcomm",                                    "",                                      "Qualcomm" },
        { 10,                   "AMD",                                    "",                                           "AMD" },
        { 11,                 "Intel",                                    "",                                         "Intel" },
        { 12,           "Imagination",                                    "",                                   "Imagination" },
        { 13,                "Google",                "Shaderc over Glslang",                   "Google Shaderc over Glslang" },
        { 14,                "Google",                             "spiregg",                                "Google spiregg" },
        { 15,                "Google",                              "rspirv",                                 "Google rspirv" },
        { 16,              "X-LEGEND",           "Mesa-IR/SPIR-V Translator",            "X-LEGEND Mesa-IR/SPIR-V Translator" },
        { 17,               "Khronos",                 "SPIR-V Tools Linker",                   "Khronos SPIR-V Tools Linker" },
        { 18,                  "Wine",               "VKD3D Shader Compiler",                    "Wine VKD3D Shader Compiler" },
        { 19,              "Tellusim",                "Clay Shader Compiler",                 "Tellusim Clay Shader Compiler" },
        { 20,      "W3C WebGPU Group",             "WHLSL Shader Translator",      "W3C WebGPU Group WHLSL Shader Translator" },
        { 21,                "Google",                               "Clspv",                                  "Google Clspv" },
        { 22,                "Google",              "MLIR SPIR-V Serializer",                 "Google MLIR SPIR-V Serializer" },
        { 23,                "Google",                       "Tint Compiler",                          "Google Tint Compiler" },
        { 24,                "Google",               "ANGLE Shader Compiler",                  "Google ANGLE Shader Compiler" },
        { 25,         "Netease Games",             "Messiah Shader Compiler",         "Netease Games Messiah Shader Compiler" },
        { 26,                 "Xenia", "Xenia Emulator Microcode Translator",     "Xenia Xenia Emulator Microcode Translator" },
        { 27,        "Embark Studios",           "Rust GPU Compiler Backend",      "Embark Studios Rust GPU Compiler Backend" },
        { 28,      "gfx-rs community",                                "Naga",                         "gfx-rs community Naga" },
        { 29, "Mikkosoft Productions",                 "MSP Shader Compiler",     "Mikkosoft Productions MSP Shader Compiler" },
        { 30,   "SpvGenTwo community",           "SpvGenTwo SPIR-V IR Tools", "SpvGenTwo community SpvGenTwo SPIR-V IR Tools" },
        { 31,                "Google",                           "Skia SkSL",                              "Google Skia SkSL" },
        { 32,             "TornadoVM",               "SPIRV Beehive Toolkit",               "TornadoVM SPIRV Beehive Toolkit" },
        { 33,           "DragonJoker",                        "ShaderWriter",                      "DragonJoker ShaderWriter" },
        { 34,          "Rayan Hatout",                          "SPIRVSmith",                       "Rayan Hatout SPIRVSmith" },
        { 35,   "Saarland University",                               "Shady",                     "Saarland University Shady" },
        { 36,       "Taichi Graphics",                              "Taichi",                        "Taichi Graphics Taichi" },
    };

    return vendor_tools[id].vendor_tool;
}

struct spirv_header
{
    uint32_t magic;
    uint32_t version;
    uint32_t generator;
    uint32_t bound;
    uint32_t schema;
};

/*
union spirv_word
{
    uint32_t code;
    struct
    {
        uint16_t wordCount;
        uint16_t op;
    };
};

struct spirv_line
{
    std::vector<uint32_t> code;
};
*/

#if defined(SPV_ENABLE_UTILITY_CODE)

// clang-format off

// 获取 GLSL.std.450 扩展指令名称
inline const char* glsl_std450_to_string(uint32_t inst)
{
    switch (inst) {
    case GLSLstd450Round:                   return "Round";
    case GLSLstd450RoundEven:               return "RoundEven";
    case GLSLstd450Trunc:                   return "Trunc";
    case GLSLstd450FAbs:                    return "FAbs";
    case GLSLstd450SAbs:                    return "SAbs";
    case GLSLstd450FSign:                   return "FSign";
    case GLSLstd450SSign:                   return "SSign";
    case GLSLstd450Floor:                   return "Floor";
    case GLSLstd450Ceil:                    return "Ceil";
    case GLSLstd450Fract:                   return "Fract";
    case GLSLstd450Radians:                 return "Radians";
    case GLSLstd450Degrees:                 return "Degrees";
    case GLSLstd450Sin:                     return "Sin";
    case GLSLstd450Cos:                     return "Cos";
    case GLSLstd450Tan:                     return "Tan";
    case GLSLstd450Asin:                    return "Asin";
    case GLSLstd450Acos:                    return "Acos";
    case GLSLstd450Atan:                    return "Atan";
    case GLSLstd450Sinh:                    return "Sinh";
    case GLSLstd450Cosh:                    return "Cosh";
    case GLSLstd450Tanh:                    return "Tanh";
    case GLSLstd450Asinh:                   return "Asinh";
    case GLSLstd450Acosh:                   return "Acosh";
    case GLSLstd450Atanh:                   return "Atanh";
    case GLSLstd450Atan2:                   return "Atan2";
    case GLSLstd450Pow:                     return "Pow";
    case GLSLstd450Exp:                     return "Exp";
    case GLSLstd450Log:                     return "Log";
    case GLSLstd450Exp2:                    return "Exp2";
    case GLSLstd450Log2:                    return "Log2";
    case GLSLstd450Sqrt:                    return "Sqrt";
    case GLSLstd450InverseSqrt:             return "InverseSqrt";
    case GLSLstd450Modf:                    return "Modf";
    case GLSLstd450ModfStruct:              return "ModfStruct";
    case GLSLstd450FMin:                    return "FMin";
    case GLSLstd450FMax:                    return "FMax";
    case GLSLstd450FClamp:                  return "FClamp";
    case GLSLstd450FMix:                    return "FMix";
    case GLSLstd450Step:                    return "Step";
    case GLSLstd450SmoothStep:              return "SmoothStep";
    case GLSLstd450Fma:                     return "Fma";
    case GLSLstd450Frexp:                   return "Frexp";
    case GLSLstd450FrexpStruct:             return "FrexpStruct";
    case GLSLstd450Ldexp:                   return "Ldexp";
    case GLSLstd450PackSnorm4x8:            return "PackSnorm4x8";
    case GLSLstd450PackUnorm4x8:            return "PackUnorm4x8";
    case GLSLstd450PackSnorm2x16:           return "PackSnorm2x16";
    case GLSLstd450PackUnorm2x16:           return "PackUnorm2x16";
    case GLSLstd450PackHalf2x16:            return "PackHalf2x16";
    case GLSLstd450PackDouble2x32:          return "PackDouble2x32";
    case GLSLstd450UnpackSnorm2x16:         return "UnpackSnorm2x16";
    case GLSLstd450UnpackUnorm2x16:         return "UnpackUnorm2x16";
    case GLSLstd450UnpackHalf2x16:          return "UnpackHalf2x16";
    case GLSLstd450UnpackSnorm4x8:          return "UnpackSnorm4x8";
    case GLSLstd450UnpackUnorm4x8:          return "UnpackUnorm4x8";
    case GLSLstd450UnpackDouble2x32:        return "UnpackDouble2x32";
    case GLSLstd450Length:                  return "Length";
    case GLSLstd450Distance:                return "Distance";
    case GLSLstd450Cross:                   return "Cross";
    case GLSLstd450Normalize:               return "Normalize";
    case GLSLstd450FaceForward:             return "FaceForward";
    case GLSLstd450Reflect:                 return "Reflect";
    case GLSLstd450Refract:                 return "Refract";
    case GLSLstd450FindILsb:                return "FindILsb";
    case GLSLstd450FindSMsb:                return "FindSMsb";
    case GLSLstd450FindUMsb:                return "FindUMsb";
    case GLSLstd450InterpolateAtCentroid:   return "InterpolateAtCentroid";
    case GLSLstd450InterpolateAtSample:     return "InterpolateAtSample";
    case GLSLstd450InterpolateAtOffset:     return "InterpolateAtOffset";
    case GLSLstd450NMin:                    return "NMin";
    case GLSLstd450NMax:                    return "NMax";
    case GLSLstd450NClamp:                  return "NClamp";
    default:
        return "Unknown";
    }
}

// clang-format on

#endif // SPV_ENABLE_UTILITY_CODE

// count the number of SPIR-V code instructions
inline size_t spirv_instruction_count(const uint32_t* spirvData, size_t size)
{
    size_t count   = 0;
    size_t address = 5;

    while (address < size) {
        uint32_t wordCount = (spirvData[address] >> 16) & 0xFFFF;

        if (wordCount == 0) {
            fprintf(stderr, "Error: Invalid instruction length\n");
            break;
        }

        count++;
        address += wordCount;
    }

    return count;
}

// print SPIR-V infomations
inline void print_spirv_info(const std::vector<uint32_t>& spirvData)
{
    if (spirvData.size() < 6) {
        printf("SPIR-V data is empty.\n");
        return;
    }

    printf("; SPIR-V\n");
    printf("; Magic: 0x%08X\n", spirvData[0]);

    int major = (spirvData[1] >> 16) & 0xFF;
    int minor = (spirvData[1] >> 8) & 0xFF;
    printf("; Version: %d.%d\n", major, minor);

    // 厂商; 版本
    printf("; Generator: %s; %u\n", spirv_generator_to_string(spirvData[2] >> 16), spirvData[2] & 0xFFFF);
    printf("; Bound: %u\n", spirvData[3]);
    printf("; Schema: %u\n", spirvData[4]);
    printf("; Size: %u dwords\n", spirvData.size());
    printf("; Instructions: %u\n", spirv_instruction_count(spirvData.data(), spirvData.size()));
}

// print SPIR-V binary data
inline void print_spirv(const std::vector<uint32_t>& spirvData)
{
    printf("const uint32_t data[] = {\n    ");
    size_t size = spirvData.size();
    for (size_t i = 0; i < size; ++i) {
        printf("0x%08X", spirvData[i]);
        if (i != size - 1) {
            printf(", ");
        }
        if (i != 0 && i % 8 == 7) {
            printf("\n    ");
        }
    }
    if (size & 7) {
        printf("\n");
    }
    printf("};\n");
}

// 打印 SPIR-V 信息
inline void spirv_print_info(std::ostream& out, const uint32_t* codes, size_t size)
{
    if (size < 6) {
        printf("SPIR-V data is empty.\n");
        return;
    }

    out << "; SPIR-V\n";
    out << "; Version: " << ((codes[1] >> 16) & 0xFF) << "." << ((codes[1] >> 8) & 0xFF) << "\n";
    out << "; Generator: " << spirv_generator_to_string(codes[2] >> 16) << "; " << (codes[2] & 0xFFFF) << "\n";
    out << "; Bound: " << codes[3] << "\n";
    out << "; Schema: " << codes[4] << "\n";
}

// 打印 SPIR-V 代码
inline void spirv_print(std::ostream& out, const uint32_t* codes, size_t size)
{
    // 检查魔法数字
    if (size < 5) {
        out << "Error: SPIR-V code too small\n";
        return;
    }

    if (codes[0] != SpvMagicNumber) {
        out << "Error: Invalid SPIR-V magic number\n";
        return;
    }

    // 输出头部信息
    spirv_print_info(out, codes, size);
    out << "; Size: " << size << " dwords\n";
    out << "; Instructions: " << spirv_instruction_count(codes, size) << "\n";

    std::cout << "const uint32_t data[] = {\n    ";
    for (size_t i = 0; i < size; ++i) {
        std::cout << "0x" << std::uppercase << std::hex << std::setw(8)
                  << std::setfill('0') << codes[i] << std::dec;
        if (i != size - 1) {
            std::cout << ", ";
        }
        if (i != 0 && i % 8 == 7) {
            std::cout << "\n    ";
        }
    }
    if (size & 7) {
        std::cout << "\n";
    }
    std::cout << "};\n\n";
}

} // end namespace cgl

#endif // SPIRV_HPP_20260204213823
