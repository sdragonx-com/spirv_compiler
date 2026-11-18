#include "spirv_disassemble.h"

#include <Windows.h>
#include <conio.h>

// 控制台颜色
enum CONSOLE_COLOR
{
    BLACK    = 0,
    NAVY     = 1,
    GREEN    = 2,
    MAROON   = 4,
    DARKGRAY = 8,
    GRAY     = MAROON | GREEN | NAVY,
    OLIVE    = GREEN | MAROON,
    TEAL     = NAVY | GREEN,
    PURPLE   = NAVY | MAROON,
    BLUE     = 8 | NAVY,
    LIME     = 8 | GREEN,
    RED      = 8 | MAROON,
    YELLOW   = LIME | RED,
    CYAN     = BLUE | LIME,
    MAGENTA  = BLUE | RED,
    WHITE    = BLUE | LIME | RED,
};

// 设置颜色
static inline void color(int front, int back = 0)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), front | back);
}

#define SPV_ENABLE_UTILITY_CODE
#include <spirv_cross/spirv.h>
#include <spirv_cross/GLSL.std.450.h>

namespace cgl {

// 解析字符串
static uint32_t spirv_parse_string(std::vector<std::string>& names, const uint32_t* codes, size_t offset, uint32_t wordCount)
{
    uint32_t targetID = codes[offset + 1];
    if (targetID < codes[3] && wordCount >= 3) {
        std::string name;
        for (uint32_t i = 2; i < wordCount; i++) {
            uint32_t wordVal = codes[offset + i];
            for (int j = 0; j < 4; j++) {
                char c = static_cast<char>((wordVal >> (j * 8)) & 0xFF);
                if (c == '\0')
                    break;
                name.push_back(c);
            }
        }
        names[targetID] = name;
        return targetID;
    }
    return UINT32_MAX;
}

// SPIR-V 反汇编函数
int spirv_disassemble(std::ostream& out, const uint32_t* codes, size_t size)
{
    // 检查魔法数字
    if (size < 5) {
        out << "Error: SPIR-V code too small\n";
        return -1;
    }

    if (codes[0] != SpvMagicNumber) {
        out << "Error: Invalid SPIR-V magic number\n";
        return -1;
    }

    // 输出头部信息
    const spirv_header* spirvInfo = (const spirv_header*) codes;
    spirv_print_info(out, codes, size);

    // 遍历所有指令
    size_t offset = 5; // 跳过头部
    uint32_t targetID;

    std::vector<std::string> names(spirvInfo->bound);

    while (offset < size) {
        uint32_t word      = codes[offset];
        uint32_t wordCount = word >> SpvWordCountShift;
        SpvOp opcode       = static_cast<SpvOp>(word & SpvOpCodeMask);

        /*
        // 输出偏移量
        out << std::setw(6) << std::setfill('0') << std::hex << offset * 4
            << std::setfill(' ') << std::dec << " ";

        if (wordCount == 0) {
            out << "Error: Invalid word count 0\n";
            break;
        }

        // 输出指令二进制表示
        for (uint32_t i = 0; i < wordCount && (offset + i) < size; i++) {
            out << std::setw(8) << std::setfill('0') << std::hex
                << code[offset + i] << " ";
        }
        out << std::setfill(' ') << std::dec;
        */

        // 对于 OpName 和 OpMemberName，保存 ID 名称以便后续引用
        if (opcode == SpvOpName) {
            targetID = spirv_parse_string(names, codes, offset, wordCount);
            if (targetID != UINT32_MAX) {
                color(WHITE);
                out << SpvOpToString(opcode);
                color(GRAY);
                out << "(" << wordCount << ")";
                color(GREEN);
                out << " \"" << names[targetID] << "\"" << std::endl;
                color(GRAY);
            }
            offset += wordCount;
            continue;
        }

        // 处理指令操作数
        size_t operandIndex = 1;

        // 处理结果类型和结果ID（如果有）
        bool hasResult     = false;
        bool hasResultType = false;
        SpvHasResultAndType(static_cast<SpvOp>(opcode), &hasResult, &hasResultType);

        uint32_t resultType = 0;
        uint32_t resultID   = 0;
        if (hasResultType) {
            if (operandIndex < wordCount) {
                resultType = codes[offset + operandIndex];
                color(CYAN);
                out << '%' << resultType << ' ';
                operandIndex++;
            }
        }

        if (hasResult) {
            if (operandIndex < wordCount) {
                resultID = codes[offset + operandIndex];
                color(RED);
                out << '%' << resultID << ' ';
                // operandIndex++;
            }
        }

        if (hasResult || hasResultType) {
            color(GRAY);
            out << "= ";
        }

        // 输出指令助记符
        color(WHITE);
        out << SpvOpToString(opcode);
        // color(GRAY);
        // out << "(" << wordCount << ")";
        color(GRAY);

        switch (opcode) {
        case SpvOpExtInstImport:
            targetID = spirv_parse_string(names, codes, offset, wordCount);
            if (targetID != UINT32_MAX) {
                out << "SpvOpExtInstImport: " << names[targetID] << std::endl;
            }
            offset += wordCount;
            continue;
        default:
            break;
        }

        // 处理其他操作数
        for (uint32_t i = operandIndex; i < wordCount; i++) {
            uint32_t operand = codes[offset + i];

            switch (opcode) {
            case SpvOpExtInst:
                if (i == operandIndex) {
                    // 结果类型
                    out << " %" << operand;
                }
                else if (i == operandIndex + 1) {
                    // 结果 ID
                    out << " %" << operand;
                }
                else if (i == operandIndex + 2) {
                    // 扩展指令集 ID
                    out << " %" << operand;
                }
                else if (i == operandIndex + 3) {
                    // 扩展指令编号
                    uint32_t extInst = operand;
                    out << " " << cgl::glsl_std450_to_string(static_cast<GLSLstd450>(extInst));
                }
                else {
                    // 其他操作数
                    out << " %" << operand;
                }
                break;
            case SpvOpConstant:
            case SpvOpSpecConstant:
                if (i == operandIndex) {
                    // 结果类型
                    out << " %" << operand;
                }
                else if (i == operandIndex + 1) {
                    // 结果 ID
                    out << " %" << operand;
                }
                else {
                    // 常量值
                    out << " " << operand;
                }
                break;
            case SpvOpTypeVoid:
            case SpvOpTypeBool:
            case SpvOpTypeInt:
            case SpvOpTypeFloat:
            case SpvOpTypeVector:
            case SpvOpTypeMatrix:
            case SpvOpTypeImage:
            case SpvOpTypeSampler:
            case SpvOpTypeSampledImage:
            case SpvOpTypeArray:
            case SpvOpTypeRuntimeArray:
            case SpvOpTypeStruct:
            case SpvOpTypeOpaque:
            case SpvOpTypePointer:
            case SpvOpTypeFunction:
            case SpvOpTypeEvent:
            case SpvOpTypeDeviceEvent:
            case SpvOpTypeReserveId:
            case SpvOpTypeQueue:
            case SpvOpTypePipe:
            case SpvOpTypeForwardPointer:
                if (i == operandIndex) {
                    // 结果 ID
                    out << " %" << operand; // 这个可能会被忽略，比如：OpTypeFloat
                }
                else {
                    // 类型参数
                    switch (opcode) {
                    case SpvOpTypeInt:
                        if (i == operandIndex + 1) {
                            out << " " << operand; // 宽度（字面值）
                        }
                        else if (i == operandIndex + 2) {
                            out << " " << operand; // 有符号性（0或1）
                        }
                        break;
                    case SpvOpTypeFloat:
                        if (i == operandIndex + 1) {
                            out << " " << operand; // 宽度（字面值）
                        }
                        break;
                    case SpvOpTypeVector:
                        if (i == operandIndex + 1) {
                            out << " %" << operand; // 组件类型（ID引用）
                        }
                        else if (i == operandIndex + 2) {
                            out << " " << operand; // 组件数量（字面值）
                        }
                        break;
                    case SpvOpTypeMatrix:
                        if (i == operandIndex + 1) {
                            out << " %" << operand; // 列类型（ID引用）
                        }
                        else if (i == operandIndex + 2) {
                            out << " " << operand; // 列数（字面值）
                        }
                        break;
                    case SpvOpTypeArray:
                        if (i == operandIndex + 1) {
                            out << " %" << operand; // 元素类型（ID引用）
                        }
                        else if (i == operandIndex + 2) {
                            out << " %" << operand; // 长度（ID引用）
                        }
                        break;
                    case SpvOpTypeRuntimeArray:
                        if (i == operandIndex + 1) {
                            out << " %" << operand; // 元素类型（ID引用）
                        }
                        break;
                    case SpvOpTypePointer:
                        if (i == operandIndex + 1) {
                            out << " " << SpvStorageClassToString(static_cast<SpvStorageClass>(operand));
                        }
                        else if (i == operandIndex + 2) {
                            out << " %" << operand; // 类型（ID引用）
                        }
                        break;
                    case SpvOpTypeFunction:
                        if (i == operandIndex + 1) {
                            out << " %r" << operand; // 返回类型（ID引用）
                        }
                        else {
                            out << " %a" << operand; // 参数类型（ID引用）
                        }
                        break;
                    case SpvOpTypeStruct:
                        // 所有成员类型都是ID引用
                        out << " %" << operand;
                        break;
                    case SpvOpTypeOpaque:
                        // OpTypeOpaque "name"
                        if (i == operandIndex + 1) {
                            // 需要解析字符串
                            out << " \"";
                            uint32_t wordVal = operand;
                            for (int j = 0; j < 4; j++) {
                                char c = static_cast<char>((wordVal >> (j * 8)) & 0xFF);
                                if (c == '\0')
                                    break;
                                out << c;
                            }
                            out << "\"";
                        }
                        break;
                    default:
                        // 其他类型指令的参数通常是ID引用
                        out << " +++%" << operand;
                        break;
                    }
                }
                break;
            case SpvOpDecorate:
            case SpvOpMemberDecorate:
                if (i == operandIndex) {
                    // 目标 ID
                    out << " %" << operand;
                }
                else if (i == operandIndex + 1) {
                    // 装饰类型
                    out << " " << SpvDecorationToString(static_cast<SpvDecoration>(operand));
                }
                else {
                    // 装饰参数（通常是字面值）
                    out << " " << operand;
                }
                break;
            case SpvOpEntryPoint:
                if (i == operandIndex) {
                    out << " " << SpvExecutionModelToString(static_cast<SpvExecutionModel>(operand));
                }
                else if (i == operandIndex + 1) {
                    out << " %" << operand; // 入口点函数ID
                }
                else if (i == operandIndex + 2) {
                    // 入口点名称字符串
                    out << " \"";
                    // 从当前字开始解析字符串，直到遇到'\0'
                    for (uint32_t strIdx = i; strIdx < wordCount; strIdx++) {
                        uint32_t wordVal = codes[offset + strIdx];
                        bool endOfString = false;
                        for (int j = 0; j < 4; j++) {
                            char c = static_cast<char>((wordVal >> (j * 8)) & 0xFF);
                            if (c == '\0') {
                                endOfString = true;
                                break;
                            }
                            out << c;
                        }
                        if (endOfString) {
                            i = strIdx; // 跳过已处理的字符串部分
                            break;
                        }
                    }
                    out << "\"";
                }
                else {
                    // 接口变量ID
                    out << " %" << operand;
                }
                break;
            default:
                // 判断是否为字面值的启发式规则：
                // 1. 如果操作数大于等于bound，则不是ID
                // 2. 某些指令的特定位置是字面值
                // 3. 某些枚举值应该输出字符串表示

                bool isLiteral = (operand >= spirvInfo->bound);
                bool isEnum    = false;

                // 检查常见指令的字面值位置
                switch (opcode) {
                case SpvOpCapability:
                    if (i == operandIndex) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvCapabilityToString(static_cast<SpvCapability>(operand));
                    }
                    break;
                case SpvOpSource:
                    if (i == operandIndex) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvSourceLanguageToString(static_cast<SpvSourceLanguage>(operand));
                    }
                    else if (i == operandIndex + 1) {
                        isLiteral = true; // 版本号
                        out << " " << operand;
                    }
                    break;
                case SpvOpExecutionMode:
                    if (i == operandIndex + 1) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvExecutionModeToString(static_cast<SpvExecutionMode>(operand));
                    }
                    else if (i > operandIndex + 1) {
                        // 执行模式的额外参数通常是字面值
                        isLiteral = true;
                        out << " " << operand;
                    }
                    break;
                case SpvOpMemoryModel:
                    if (i == operandIndex) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvAddressingModelToString(static_cast<SpvAddressingModel>(operand));
                    }
                    else if (i == operandIndex + 1) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvMemoryModelToString(static_cast<SpvMemoryModel>(operand));
                    }
                    break;
                case SpvOpEntryPoint: // SpvOpExecutionModel:
                    if (i == operandIndex) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvExecutionModelToString(static_cast<SpvExecutionModel>(operand));
                    }
                    break;
                /*
                case SpvOpFPRoundingMode:
                    if (i == operandIndex) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvFPRoundingModeToString(static_cast<SpvFPRoundingMode>(operand));
                    }
                    break;
                case SpvOpLinkageType:
                    if (i == operandIndex) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvLinkageTypeToString(static_cast<SpvLinkageType>(operand));
                    }
                    break;
                case SpvOpAccessQualifier:
                    if (i == operandIndex) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvAccessQualifierToString(static_cast<SpvAccessQualifier>(operand));
                    }
                    break;
                case SpvOpFunctionParameterAttribute:
                    if (i == operandIndex) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvFunctionParameterAttributeToString(static_cast<SpvFunctionParameterAttribute>(operand));
                    }
                    break;
                case SpvOpLoopControl:
                case SpvOpSelectionControl:
                case SpvOpFunctionControl:
                    // 这些是掩码值
                    isLiteral = true;
                    out << " " << operand;
                    break;
                case SpvOpImageOperands:
                case SpvOpFPFastMathMode:
                case SpvOpMemoryAccess:
                case SpvOpMemorySemantics:
                    // 这些是掩码值
                    isLiteral = true;
                    out << " " << operand;
                    break;
                case SpvOpScope:
                    if (i == operandIndex) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvScopeToString(static_cast<SpvScope>(operand));
                    }
                    break;
                case SpvOpGroupOperation:
                    if (i == operandIndex) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvGroupOperationToString(static_cast<SpvGroupOperation>(operand));
                    }
                    break;
                case SpvOpKernelEnqueueFlags:
                    if (i == operandIndex) {
                        isLiteral = true;
                        isEnum    = true;
                        out << " " << SpvKernelEnqueueFlagsToString(static_cast<SpvKernelEnqueueFlags>(operand));
                    }
                    break;
                */
                case SpvOpConstantTrue:
                case SpvOpConstantFalse:
                case SpvOpSpecConstantTrue:
                case SpvOpSpecConstantFalse:
                    // 这些指令没有额外的操作数
                    break;
                case SpvOpConstantNull:
                case SpvOpUndef:
                    // 这些指令只有结果类型和结果 ID
                    break;
                default:
                    // 对于其他指令，使用默认启发式
                    if (operand < spirvInfo->bound && !isLiteral && !isEnum) {
                        // 可能是ID引用
                        out << " %" << operand;
                        // 如果这个 ID 有名称，也输出它
                        if (!names[operand].empty()) {
                            out << " \"" << names[operand] << "\"";
                        }
                    }
                    else if (!isEnum) {
                        // 字面值
                        out << " " << operand;
                    }
                    break;
                }
                break;
            }
        }

        out << "\n";
        offset += wordCount;
    }

    return 0;
}

} // end namespace cgl
