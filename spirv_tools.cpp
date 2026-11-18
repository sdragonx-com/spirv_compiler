#include "spirv_tools.h"

// spirv-tools 输出汇编代码
void spirv_tools_disassemble(const std::vector<uint32_t>& spirvData)
{
    // 创建上下文
    spv_context context = spvContextCreate(SPV_ENV_UNIVERSAL_1_5);

    // 准备输出变量
    spv_text text             = nullptr;
    spv_diagnostic diagnostic = nullptr;

    // 调用反汇编函数
    const uint32_t options = SPV_BINARY_TO_TEXT_OPTION_INDENT |
                             SPV_BINARY_TO_TEXT_OPTION_FRIENDLY_NAMES;
    // SPV_BINARY_TO_TEXT_OPTION_COLOR // linux 的 printf 着色

    spv_result_t result = spvBinaryToText(
        context,
        spirvData.data(),
        spirvData.size(),
        options,
        &text,
        &diagnostic
    );

    // 检查结果并使用
    if (result == SPV_SUCCESS && text != nullptr) {
        // 输出反汇编后的字符串
        std::cout << text->str << std::endl;
    }
    else if (diagnostic != nullptr) {
        // 处理错误信息
        std::cerr << diagnostic->error << std::endl;
    }

    // 清理资源
    if (text)
        spvTextDestroy(text);
    if (diagnostic)
        spvDiagnosticDestroy(diagnostic);

    spvContextDestroy(context);
}

namespace {

// spv_message_consumer 是普通函数指针，不能捕获上下文
void OptMessage(spv_message_level_t level, const char* source, const spv_position_t* pos, const char* message)
{
    static const char* const kName[] = { "FATAL", "INTERNAL", "ERROR", "WARN", "INFO", "DEBUG" };

    int i = static_cast<int>(level);
    std::fprintf(stderr, "[spirv-opt][%s] %s:%zu:%zu %s\n", (i >= 0 && i < 6) ? kName[i] : "?", source ? source : "<unknown>", pos ? pos->line : 0, pos ? pos->column : 0, message ? message : "");
}

} // namespace

bool SpirvOptimizeBinary(const uint32_t* words, size_t word_count, std::vector<uint32_t>* out, const SpirvOptOptions& opt, std::string* error)
{
    if (!out || !words || word_count < 5) {
        if (error)
            *error = "invalid SPIR-V binary (need >= 5 words)";
        return false;
    }
    out->clear();

    spv_optimizer_t* optimizer = spvOptimizerCreate(opt.env);
    if (!optimizer) {
        if (error)
            *error = "spvOptimizerCreate failed (invalid target env?)";
        return false;
    }
    spvOptimizerSetMessageConsumer(optimizer, &OptMessage);

    // ---- 1. 注册 pass ----
    bool registered = true;
    if (opt.passes.empty()) {
        spvOptimizerRegisterPerformancePasses(optimizer); // 等价 -O
    }
    else {
        std::vector<const char*> flags;
        flags.reserve(opt.passes.size());
        for (const std::string& p : opt.passes)
            flags.push_back(p.c_str());

        // 还要拿这个模块做反射/建 pipeline => 用 WhilePreservingTheInterface 版本，
        // 否则 AggressiveDCE 可能把没被用到的 IO 变量删掉，导致接口不匹配。
        registered = spvOptimizerRegisterPassesFromFlags(
            optimizer, flags.data(), flags.size()
        );
        if (!registered && error)
            *error = "invalid optimizer pass flag";
    }

    // ---- 2. 运行选项 ----
    spv_optimizer_options options = spvOptimizerOptionsCreate();
    if (!options) {
        spvOptimizerDestroy(optimizer);
        if (error)
            *error = "spvOptimizerOptionsCreate failed";
        return false;
    }
    spvOptimizerOptionsSetRunValidator(options, opt.validate);
    // 若输入 id bound 很大而机器内存紧张，可显式压一下：
    // spvOptimizerOptionsSetMaxIdBound(options, 0x3FFFFF);

    // ---- 3. 跑 ----
    spv_binary result = nullptr;
    spv_result_t res  = registered
                            ? spvOptimizerRun(optimizer, words, word_count, &result, options)
                            : SPV_ERROR_INVALID_VALUE;

    if (res == SPV_SUCCESS && result && result->code && result->wordCount > 0) {
        out->assign(result->code, result->code + result->wordCount);
    }
    else if (error) {
        *error = "spvOptimizerRun failed";
    }

    // ---- 4. 回收 ----
    if (result)
        spvBinaryDestroy(result);
    spvOptimizerOptionsDestroy(options);
    spvOptimizerDestroy(optimizer);
    return res == SPV_SUCCESS;
}

bool SpirvOptimizeAssembly(const std::string& assembly, std::string* out_assembly, const SpirvOptOptions& opt, std::string* error)
{
    if (!out_assembly)
        return false;
    out_assembly->clear();

    spv_context ctx = spvContextCreate(opt.env);
    if (!ctx) {
        if (error)
            *error = "spvContextCreate failed";
        return false;
    }

    // ---- 文本 -> 二进制（PRESERVE_NUMERIC_IDS：%1 %2 编号不重排，输出才好对照输入）----
    spv_binary binary   = nullptr;
    spv_diagnostic diag = nullptr;
    spv_result_t res    = spvTextToBinaryWithOptions(
        ctx, assembly.c_str(), assembly.size(),
        SPV_TEXT_TO_BINARY_OPTION_PRESERVE_NUMERIC_IDS, &binary, &diag
    );
    if (res != SPV_SUCCESS || !binary) {
        if (diag)
            spvDiagnosticPrint(diag);
        if (error)
            *error = "spvTextToBinary failed";
        if (diag)
            spvDiagnosticDestroy(diag);
        spvContextDestroy(ctx);
        return false;
    }
    if (diag) {
        spvDiagnosticDestroy(diag);
        diag = nullptr;
    }

    std::vector<uint32_t> optimized;
    bool ok = SpirvOptimizeBinary(binary->code, binary->wordCount, &optimized, opt, error);
    spvBinaryDestroy(binary);

    if (!ok) {
        spvContextDestroy(ctx);
        return false;
    }

    // ---- 二进制 -> 文本 ----
    spv_text text = nullptr;
    res           = spvBinaryToText(ctx, optimized.data(), optimized.size(), SPV_BINARY_TO_TEXT_OPTION_INDENT, &text, &diag);
    if (res == SPV_SUCCESS && text) {
        out_assembly->assign(text->str, text->length);
    }
    else {
        if (diag)
            spvDiagnosticPrint(diag);
        if (error)
            *error = "spvBinaryToText failed";
        ok = false;
    }
    if (text)
        spvTextDestroy(text);
    if (diag)
        spvDiagnosticDestroy(diag);
    spvContextDestroy(ctx);
    return ok;
}
