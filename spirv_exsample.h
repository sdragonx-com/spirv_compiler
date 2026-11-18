#pragma once

#include <stdint.h>

// GLSL Fragment
// 参考：https://registry.khronos.org/SPIR-V/specs/unified1/SPIRV.html
const char* GLSL_EXSAMPLE_CODE = R"(
#version 450

#extension GL_EXT_multi_entry_point : enable

layout(location = 0) in vec4 vPos;
layout(location = 1) in vec4 vNorm;
layout(location = 2) noperspective in vec4 vColor;
layout(location = 0) out vec4 FragColor;

struct Light {
    bool b;
    vec4 v[5];
    int i;
};

layout(binding = 0) uniform blockName {
    Light light;
    bool lighting;
};

vec4 fn_mult(vec4 a, vec4 b)
{
    return a * b;
}

void main()
{
    vec4 scale = vec4(1.0, 1.0, 2.0, 1.0);

    if (lighting)
        FragColor = vPos + light.v[2];
    else
        FragColor = sqrt(vPos) * scale;

    for (int i = 0; i < 4; ++i) {
        FragColor *= vColor;
        FragColor = fn_mult(FragColor, vNorm);
    }
}
)";

// SPIRV_EXSAMPLE_CODE
/*
; SPIR-V
; Magic: 0x07230203
; Version: 1.0
; Generator: Khronos Glslang Reference Front End; 11
; Bound: 78
; Schema: 0
; Size: 523 dwords
; Instructions: 137
*/
const uint32_t SPIRV_EXSAMPLE_CODE[] = {
    0x07230203, 0x00010000, 0x0008000B, 0x0000004E, 0x00000000, 0x00020011, 0x00000001, 0x0006000B,
    0x00000001, 0x4C534C47, 0x6474732E, 0x3035342E, 0x00000000, 0x0003000E, 0x00000000, 0x00000001,
    0x0009000F, 0x00000004, 0x00000004, 0x6E69616D, 0x00000000, 0x00000029, 0x0000002B, 0x00000042,
    0x00000046, 0x00030010, 0x00000004, 0x00000007, 0x00030003, 0x00000002, 0x000001C2, 0x00040005,
    0x00000004, 0x6E69616D, 0x00000000, 0x00070005, 0x0000000C, 0x6D5F6E66, 0x28746C75, 0x3B346676,
    0x3B346676, 0x00000000, 0x00030005, 0x0000000A, 0x00000061, 0x00030005, 0x0000000B, 0x00000062,
    0x00040005, 0x00000013, 0x6C616373, 0x00000065, 0x00040005, 0x0000001B, 0x6867694C, 0x00000074,
    0x00040006, 0x0000001B, 0x00000000, 0x00000062, 0x00040006, 0x0000001B, 0x00000001, 0x00000076,
    0x00040006, 0x0000001B, 0x00000002, 0x00000069, 0x00050005, 0x0000001C, 0x636F6C62, 0x6D614E6B,
    0x00000065, 0x00050006, 0x0000001C, 0x00000000, 0x6867696C, 0x00000074, 0x00060006, 0x0000001C,
    0x00000001, 0x6867696C, 0x676E6974, 0x00000000, 0x00030005, 0x0000001E, 0x00000000, 0x00050005,
    0x00000029, 0x67617246, 0x6F6C6F43, 0x00000072, 0x00040005, 0x0000002B, 0x736F5076, 0x00000000,
    0x00030005, 0x00000039, 0x00000069, 0x00040005, 0x00000042, 0x6C6F4376, 0x0000726F, 0x00040005,
    0x00000046, 0x726F4E76, 0x0000006D, 0x00040005, 0x00000047, 0x61726170, 0x0000006D, 0x00040005,
    0x00000049, 0x61726170, 0x0000006D, 0x00040047, 0x00000019, 0x00000006, 0x00000010, 0x00050048,
    0x0000001B, 0x00000000, 0x00000023, 0x00000000, 0x00050048, 0x0000001B, 0x00000001, 0x00000023,
    0x00000010, 0x00050048, 0x0000001B, 0x00000002, 0x00000023, 0x00000060, 0x00030047, 0x0000001C,
    0x00000002, 0x00050048, 0x0000001C, 0x00000000, 0x00000023, 0x00000000, 0x00050048, 0x0000001C,
    0x00000001, 0x00000023, 0x00000070, 0x00040047, 0x0000001E, 0x00000021, 0x00000000, 0x00040047,
    0x0000001E, 0x00000022, 0x00000000, 0x00040047, 0x00000029, 0x0000001E, 0x00000000, 0x00040047,
    0x0000002B, 0x0000001E, 0x00000000, 0x00030047, 0x00000042, 0x0000000D, 0x00040047, 0x00000042,
    0x0000001E, 0x00000002, 0x00040047, 0x00000046, 0x0000001E, 0x00000001, 0x00020013, 0x00000002,
    0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007,
    0x00000006, 0x00000004, 0x00040020, 0x00000008, 0x00000007, 0x00000007, 0x00050021, 0x00000009,
    0x00000007, 0x00000008, 0x00000008, 0x0004002B, 0x00000006, 0x00000014, 0x3F800000, 0x0004002B,
    0x00000006, 0x00000015, 0x40000000, 0x0007002C, 0x00000007, 0x00000016, 0x00000014, 0x00000014,
    0x00000015, 0x00000014, 0x00040015, 0x00000017, 0x00000020, 0x00000000, 0x0004002B, 0x00000017,
    0x00000018, 0x00000005, 0x0004001C, 0x00000019, 0x00000007, 0x00000018, 0x00040015, 0x0000001A,
    0x00000020, 0x00000001, 0x0005001E, 0x0000001B, 0x00000017, 0x00000019, 0x0000001A, 0x0004001E,
    0x0000001C, 0x0000001B, 0x00000017, 0x00040020, 0x0000001D, 0x00000002, 0x0000001C, 0x0004003B,
    0x0000001D, 0x0000001E, 0x00000002, 0x0004002B, 0x0000001A, 0x0000001F, 0x00000001, 0x00040020,
    0x00000020, 0x00000002, 0x00000017, 0x00020014, 0x00000023, 0x0004002B, 0x00000017, 0x00000024,
    0x00000000, 0x00040020, 0x00000028, 0x00000003, 0x00000007, 0x0004003B, 0x00000028, 0x00000029,
    0x00000003, 0x00040020, 0x0000002A, 0x00000001, 0x00000007, 0x0004003B, 0x0000002A, 0x0000002B,
    0x00000001, 0x0004002B, 0x0000001A, 0x0000002D, 0x00000000, 0x0004002B, 0x0000001A, 0x0000002E,
    0x00000002, 0x00040020, 0x0000002F, 0x00000002, 0x00000007, 0x00040020, 0x00000038, 0x00000007,
    0x0000001A, 0x0004002B, 0x0000001A, 0x00000040, 0x00000004, 0x0004003B, 0x0000002A, 0x00000042,
    0x00000001, 0x0004003B, 0x0000002A, 0x00000046, 0x00000001, 0x00050036, 0x00000002, 0x00000004,
    0x00000000, 0x00000003, 0x000200F8, 0x00000005, 0x0004003B, 0x00000008, 0x00000013, 0x00000007,
    0x0004003B, 0x00000038, 0x00000039, 0x00000007, 0x0004003B, 0x00000008, 0x00000047, 0x00000007,
    0x0004003B, 0x00000008, 0x00000049, 0x00000007, 0x0003003E, 0x00000013, 0x00000016, 0x00050041,
    0x00000020, 0x00000021, 0x0000001E, 0x0000001F, 0x0004003D, 0x00000017, 0x00000022, 0x00000021,
    0x000500AB, 0x00000023, 0x00000025, 0x00000022, 0x00000024, 0x000300F7, 0x00000027, 0x00000000,
    0x000400FA, 0x00000025, 0x00000026, 0x00000033, 0x000200F8, 0x00000026, 0x0004003D, 0x00000007,
    0x0000002C, 0x0000002B, 0x00070041, 0x0000002F, 0x00000030, 0x0000001E, 0x0000002D, 0x0000001F,
    0x0000002E, 0x0004003D, 0x00000007, 0x00000031, 0x00000030, 0x00050081, 0x00000007, 0x00000032,
    0x0000002C, 0x00000031, 0x0003003E, 0x00000029, 0x00000032, 0x000200F9, 0x00000027, 0x000200F8,
    0x00000033, 0x0004003D, 0x00000007, 0x00000034, 0x0000002B, 0x0006000C, 0x00000007, 0x00000035,
    0x00000001, 0x0000001F, 0x00000034, 0x0004003D, 0x00000007, 0x00000036, 0x00000013, 0x00050085,
    0x00000007, 0x00000037, 0x00000035, 0x00000036, 0x0003003E, 0x00000029, 0x00000037, 0x000200F9,
    0x00000027, 0x000200F8, 0x00000027, 0x0003003E, 0x00000039, 0x0000002D, 0x000200F9, 0x0000003A,
    0x000200F8, 0x0000003A, 0x000400F6, 0x0000003C, 0x0000003D, 0x00000000, 0x000200F9, 0x0000003E,
    0x000200F8, 0x0000003E, 0x0004003D, 0x0000001A, 0x0000003F, 0x00000039, 0x000500B1, 0x00000023,
    0x00000041, 0x0000003F, 0x00000040, 0x000400FA, 0x00000041, 0x0000003B, 0x0000003C, 0x000200F8,
    0x0000003B, 0x0004003D, 0x00000007, 0x00000043, 0x00000042, 0x0004003D, 0x00000007, 0x00000044,
    0x00000029, 0x00050085, 0x00000007, 0x00000045, 0x00000044, 0x00000043, 0x0003003E, 0x00000029,
    0x00000045, 0x0004003D, 0x00000007, 0x00000048, 0x00000029, 0x0003003E, 0x00000047, 0x00000048,
    0x0004003D, 0x00000007, 0x0000004A, 0x00000046, 0x0003003E, 0x00000049, 0x0000004A, 0x00060039,
    0x00000007, 0x0000004B, 0x0000000C, 0x00000047, 0x00000049, 0x0003003E, 0x00000029, 0x0000004B,
    0x000200F9, 0x0000003D, 0x000200F8, 0x0000003D, 0x0004003D, 0x0000001A, 0x0000004C, 0x00000039,
    0x00050080, 0x0000001A, 0x0000004D, 0x0000004C, 0x0000001F, 0x0003003E, 0x00000039, 0x0000004D,
    0x000200F9, 0x0000003A, 0x000200F8, 0x0000003C, 0x000100FD, 0x00010038, 0x00050036, 0x00000007,
    0x0000000C, 0x00000000, 0x00000009, 0x00030037, 0x00000008, 0x0000000A, 0x00030037, 0x00000008,
    0x0000000B, 0x000200F8, 0x0000000D, 0x0004003D, 0x00000007, 0x0000000E, 0x0000000A, 0x0004003D,
    0x00000007, 0x0000000F, 0x0000000B, 0x00050085, 0x00000007, 0x00000010, 0x0000000E, 0x0000000F,
    0x000200FE, 0x00000010, 0x00010038
};

/*
; SPIR-V
; Version: 1.0
; Generator: Khronos Glslang Reference Front End; 11
; Bound: 78
; Schema: 0
               OpCapability Shader
          %1 = OpExtInstImport "GLSL.std.450"
               OpMemoryModel Logical GLSL450
               OpEntryPoint Fragment %4 "main" %41 %43 %66 %70
               OpExecutionMode %4 OriginUpperLeft
               OpSource GLSL 450
               OpName %4 "main"
               OpName %12 "fn_mult(vf4;vf4;"
               OpName %10 "a"
               OpName %11 "b"
               OpName %19 "scale"
               OpName %27 "Light"
               OpMemberName %27 0 "b"
               OpMemberName %27 1 "v"
               OpMemberName %27 2 "i"
               OpName %28 "blockName"
               OpMemberName %28 0 "light"
               OpMemberName %28 1 "lighting"
               OpName %30 ""
               OpName %41 "FragColor"
               OpName %43 "vPos"
               OpName %57 "i"
               OpName %66 "vColor"
               OpName %70 "vNorm"
               OpName %71 "param"
               OpName %73 "param"
               OpDecorate %25 ArrayStride 16
               OpMemberDecorate %27 0 Offset 0
               OpMemberDecorate %27 1 Offset 16
               OpMemberDecorate %27 2 Offset 96
               OpDecorate %28 Block
               OpMemberDecorate %28 0 Offset 0
               OpMemberDecorate %28 1 Offset 112
               OpDecorate %30 Binding 0
               OpDecorate %30 DescriptorSet 0
               OpDecorate %41 Location 0
               OpDecorate %43 Location 0
               OpDecorate %66 NoPerspective
               OpDecorate %66 Location 2
               OpDecorate %70 Location 1
          %2 = OpTypeVoid
          %3 = OpTypeFunction %2
          %6 = OpTypeFloat 32
          %7 = OpTypeVector %6 4
          %8 = OpTypePointer Function %7
          %9 = OpTypeFunction %7 %8 %8
         %20 = OpConstant %6 1
         %21 = OpConstant %6 2
         %22 = OpConstantComposite %7 %20 %20 %21 %20
         %23 = OpTypeInt 32 0
         %24 = OpConstant %23 5
         %25 = OpTypeArray %7 %24
         %26 = OpTypeInt 32 1
         %27 = OpTypeStruct %23 %25 %26
         %28 = OpTypeStruct %27 %23
         %29 = OpTypePointer Uniform %28
         %30 = OpVariable %29 Uniform
         %31 = OpConstant %26 1
         %32 = OpTypePointer Uniform %23
         %35 = OpTypeBool
         %36 = OpConstant %23 0
         %40 = OpTypePointer Output %7
         %41 = OpVariable %40 Output
         %42 = OpTypePointer Input %7
         %43 = OpVariable %42 Input
         %45 = OpConstant %26 0
         %46 = OpConstant %26 2
         %47 = OpTypePointer Uniform %7
         %56 = OpTypePointer Function %26
         %64 = OpConstant %26 4
         %66 = OpVariable %42 Input
         %70 = OpVariable %42 Input
          %4 = OpFunction %2 None %3
          %5 = OpLabel
         %19 = OpVariable %8 Function
         %57 = OpVariable %56 Function
         %71 = OpVariable %8 Function
         %73 = OpVariable %8 Function
               OpStore %19 %22
         %33 = OpAccessChain %32 %30 %31
         %34 = OpLoad %23 %33
         %37 = OpINotEqual %35 %34 %36
               OpSelectionMerge %39 None
               OpBranchConditional %37 %38 %51
         %38 = OpLabel
         %44 = OpLoad %7 %43
         %48 = OpAccessChain %47 %30 %45 %31 %46
         %49 = OpLoad %7 %48
         %50 = OpFAdd %7 %44 %49
               OpStore %41 %50
               OpBranch %39
         %51 = OpLabel
         %52 = OpLoad %7 %43
         %53 = OpExtInst %7 %1 Sqrt %52
         %54 = OpLoad %7 %19
         %55 = OpFMul %7 %53 %54
               OpStore %41 %55
               OpBranch %39
         %39 = OpLabel
               OpStore %57 %45
               OpBranch %58
         %58 = OpLabel
               OpLoopMerge %60 %61 None
               OpBranch %62
         %62 = OpLabel
         %63 = OpLoad %26 %57
         %65 = OpSLessThan %35 %63 %64
               OpBranchConditional %65 %59 %60
         %59 = OpLabel
         %67 = OpLoad %7 %66
         %68 = OpLoad %7 %41
         %69 = OpFMul %7 %68 %67
               OpStore %41 %69
         %72 = OpLoad %7 %41
               OpStore %71 %72
         %74 = OpLoad %7 %70
               OpStore %73 %74
         %75 = OpFunctionCall %7 %12 %71 %73
               OpStore %41 %75
               OpBranch %61
         %61 = OpLabel
         %76 = OpLoad %26 %57
         %77 = OpIAdd %26 %76 %31
               OpStore %57 %77
               OpBranch %58
         %60 = OpLabel
               OpReturn
               OpFunctionEnd
         %12 = OpFunction %7 None %9
         %10 = OpFunctionParameter %8
         %11 = OpFunctionParameter %8
         %13 = OpLabel
         %14 = OpLoad %7 %10
         %15 = OpLoad %7 %11
         %16 = OpFMul %7 %14 %15
               OpReturnValue %16
               OpFunctionEnd
*/
