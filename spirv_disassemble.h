/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 spirv_disassemble.h

 2026-11-18 03:41:38

*/
#ifndef SPIRV_DISASSEMBLE_H_20261118034138
#define SPIRV_DISASSEMBLE_H_20261118034138

#define SPV_ENABLE_UTILITY_CODE
#include "spirv.hpp"

namespace cgl {

// SPIR-V 反汇编函数
int spirv_disassemble(std::ostream& out, const uint32_t* codes, size_t size);

} // end namespace cgl

#endif // SPIRV_DISASSEMBLE_H_20261118034138
