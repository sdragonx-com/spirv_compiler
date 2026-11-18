#include "spirv_tools_loader.h"
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
typedef HMODULE spv_module;
typedef FARPROC spv_proc;
#else
typedef void* spv_module;
typedef void* spv_proc;
#endif

static spv_module _m_module = NULL;

static void free_spirv_tools_library()
{
    if (_m_module) {
        #ifdef _WIN32
        FreeLibrary(_m_module);
        #else
        dlclose(_m_module);
        #endif
        _m_module = NULL;
    }
}

static spv_module load_spirv_tools_library()
{
    spv_module handle = NULL;

    #ifdef _WIN32
    const char* LIBS[] = { "SPIRV-Tools.dll", "libSPIRV-Tools-shared.dll", "glslang.dll", "glslang_shared.dll", "libglslang.dll" };

    for (size_t i = 0; i < _countof(LIBS); ++i) {
        handle = LoadLibraryA(LIBS[i]);
        if (handle) return handle;
    }
    printf("load SPIRV-Tools library failed.\n");
    #else
    const char* LIBS[] = { "libSPIRV-Tools.so", "libglslang.so", "libglslang.so.1", "libglslang.so.0" };

    for (size_t i = 0; i < _countof(LIBS); ++i) {
        handle = dlopen(LIBS[i], RTLD_LAZY | RTLD_LOCAL);
        if (handle) return handle;
    }
    printf("load libSPIRV-Tools.so failed.\n");
    #endif

    if (handle) {
        atexit(free_spirv_tools_library);
    }
    return handle;
}

static spv_proc spirv_get_proc(const char* name)
{
    #ifdef _WIN32
    return GetProcAddress(_m_module, name);
    #else
    return dlsym(_m_module, name);
    #endif
}

static PFN_SPV_SOFTWARE_VERSION_STRING         _m_spvSoftwareVersionString         = NULL;
static PFN_SPV_SOFTWARE_VERSION_DETAILS_STRING _m_spvSoftwareVersionDetailsString = NULL;

static PFN_SPV_CONTEXT_CREATE  _m_spvContextCreate  = NULL;
static PFN_SPV_CONTEXT_DESTROY _m_spvContextDestroy = NULL;

static PFN_SPV_TEXT_TO_BINARY                _m_spvTextToBinary                = NULL;
static PFN_SPV_TEXT_TO_BINARY_WITH_OPTIONS   _m_spvTextToBinaryWithOptions     = NULL;
static PFN_SPV_BINARY_TO_TEXT                _m_spvBinaryToText                = NULL;
static PFN_SPV_BINARY_DESTROY                _m_spvBinaryDestroy               = NULL;
static PFN_SPV_TEXT_DESTROY                  _m_spvTextDestroy                 = NULL;

static PFN_SPV_VALIDATE                      _m_spvValidate                    = NULL;
static PFN_SPV_VALIDATE_WITH_OPTIONS         _m_spvValidateWithOptions         = NULL;
static PFN_SPV_VALIDATE_BINARY               _m_spvValidateBinary              = NULL;

static PFN_SPV_DIAGNOSTIC_CREATE  _m_spvDiagnosticCreate  = NULL;
static PFN_SPV_DIAGNOSTIC_DESTROY _m_spvDiagnosticDestroy = NULL;
static PFN_SPV_DIAGNOSTIC_PRINT   _m_spvDiagnosticPrint   = NULL;

static PFN_SPV_VALIDATOR_OPTIONS_CREATE                              _m_spvValidatorOptionsCreate                              = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_DESTROY                             _m_spvValidatorOptionsDestroy                             = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_UNIVERSAL_LIMIT                 _m_spvValidatorOptionsSetUniversalLimit                   = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_RELAX_STORE_STRUCT              _m_spvValidatorOptionsSetRelaxStoreStruct                 = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_RELAX_LOGICAL_POINTER           _m_spvValidatorOptionsSetRelaxLogicalPointer              = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_BEFORE_HLSL_LEGALIZATION        _m_spvValidatorOptionsSetBeforeHlslLegalization           = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_RELAX_BLOCK_LAYOUT              _m_spvValidatorOptionsSetRelaxBlockLayout                 = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_UNIFORM_BUFFER_STANDARD_LAYOUT  _m_spvValidatorOptionsSetUniformBufferStandardLayout      = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_SCALAR_BLOCK_LAYOUT             _m_spvValidatorOptionsSetScalarBlockLayout                = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_WORKGROUP_SCALAR_BLOCK_LAYOUT   _m_spvValidatorOptionsSetWorkgroupScalarBlockLayout       = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_SKIP_BLOCK_LAYOUT               _m_spvValidatorOptionsSetSkipBlockLayout                  = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_ALLOW_LOCAL_SIZE_ID             _m_spvValidatorOptionsSetAllowLocalSizeId                 = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_ALLOW_OFFSET_TEXTURE_OPERAND    _m_spvValidatorOptionsSetAllowOffsetTextureOperand        = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_ALLOW_VULKAN32_BIT_BITWISE      _m_spvValidatorOptionsSetAllowVulkan32BitBitwise          = NULL;
static PFN_SPV_VALIDATOR_OPTIONS_SET_FRIENDLY_NAMES                  _m_spvValidatorOptionsSetFriendlyNames                    = NULL;

static PFN_SPV_OPTIMIZER_OPTIONS_CREATE                       _m_spvOptimizerOptionsCreate                      = NULL;
static PFN_SPV_OPTIMIZER_OPTIONS_DESTROY                      _m_spvOptimizerOptionsDestroy                     = NULL;
static PFN_SPV_OPTIMIZER_OPTIONS_SET_RUN_VALIDATOR            _m_spvOptimizerOptionsSetRunValidator             = NULL;
static PFN_SPV_OPTIMIZER_OPTIONS_SET_VALIDATOR_OPTIONS        _m_spvOptimizerOptionsSetValidatorOptions         = NULL;
static PFN_SPV_OPTIMIZER_OPTIONS_SET_MAX_ID_BOUND             _m_spvOptimizerOptionsSetMaxIdBound               = NULL;
static PFN_SPV_OPTIMIZER_OPTIONS_SET_PRESERVE_BINDINGS        _m_spvOptimizerOptionsSetPreserveBindings         = NULL;
static PFN_SPV_OPTIMIZER_OPTIONS_SET_PRESERVE_SPEC_CONSTANTS  _m_spvOptimizerOptionsSetPreserveSpecConstants    = NULL;

static PFN_SPV_REDUCER_OPTIONS_CREATE                     _m_spvReducerOptionsCreate                    = NULL;
static PFN_SPV_REDUCER_OPTIONS_DESTROY                    _m_spvReducerOptionsDestroy                   = NULL;
static PFN_SPV_REDUCER_OPTIONS_SET_STEP_LIMIT             _m_spvReducerOptionsSetStepLimit              = NULL;
static PFN_SPV_REDUCER_OPTIONS_SET_FAIL_ON_VALIDATION_ERROR _m_spvReducerOptionsSetFailOnValidationError = NULL;
static PFN_SPV_REDUCER_OPTIONS_SET_TARGET_FUNCTION        _m_spvReducerOptionsSetTargetFunction         = NULL;

static PFN_SPV_FUZZER_OPTIONS_CREATE                         _m_spvFuzzerOptionsCreate                        = NULL;
static PFN_SPV_FUZZER_OPTIONS_DESTROY                        _m_spvFuzzerOptionsDestroy                       = NULL;
static PFN_SPV_FUZZER_OPTIONS_ENABLE_REPLAY_VALIDATION       _m_spvFuzzerOptionsEnableReplayValidation        = NULL;
static PFN_SPV_FUZZER_OPTIONS_SET_RANDOM_SEED                _m_spvFuzzerOptionsSetRandomSeed                 = NULL;
static PFN_SPV_FUZZER_OPTIONS_SET_REPLAY_RANGE               _m_spvFuzzerOptionsSetReplayRange                = NULL;
static PFN_SPV_FUZZER_OPTIONS_SET_SHRINKER_STEP_LIMIT        _m_spvFuzzerOptionsSetShrinkerStepLimit          = NULL;
static PFN_SPV_FUZZER_OPTIONS_ENABLE_FUZZER_PASS_VALIDATION  _m_spvFuzzerOptionsEnableFuzzerPassValidation    = NULL;
static PFN_SPV_FUZZER_OPTIONS_ENABLE_ALL_PASSES              _m_spvFuzzerOptionsEnableAllPasses               = NULL;

static PFN_SPV_OPCODE_STRING   _m_spvOpcodeString  = NULL;
static PFN_SPV_BINARY_PARSE    _m_spvBinaryParse   = NULL;

static PFN_SPV_OPTIMIZER_CREATE                                        _m_spvOptimizerCreate                                     = NULL;
static PFN_SPV_OPTIMIZER_DESTROY                                       _m_spvOptimizerDestroy                                    = NULL;
static PFN_SPV_OPTIMIZER_SET_MESSAGE_CONSUMER                          _m_spvOptimizerSetMessageConsumer                         = NULL;
static PFN_SPV_OPTIMIZER_REGISTER_LEGALIZATION_PASSES                  _m_spvOptimizerRegisterLegalizationPasses                 = NULL;
static PFN_SPV_OPTIMIZER_REGISTER_PERFORMANCE_PASSES                   _m_spvOptimizerRegisterPerformancePasses                  = NULL;
static PFN_SPV_OPTIMIZER_REGISTER_SIZE_PASSES                          _m_spvOptimizerRegisterSizePasses                         = NULL;
static PFN_SPV_OPTIMIZER_REGISTER_PASS_FROM_FLAG                       _m_spvOptimizerRegisterPassFromFlag                       = NULL;
static PFN_SPV_OPTIMIZER_REGISTER_PASSES_FROM_FLAGS                    _m_spvOptimizerRegisterPassesFromFlags                    = NULL;
static PFN_SPV_OPTIMIZER_REGISTER_PASSES_FROM_FLAGS_PRESERVING_INTERFACE _m_spvOptimizerRegisterPassesFromFlagsWhilePreservingTheInterface = NULL;
static PFN_SPV_OPTIMIZER_RUN                                           _m_spvOptimizerRun                                        = NULL;

// load spirv-tools functions
int load_spirv_tools()
{
    if (!_m_module) {
        _m_module = load_spirv_tools_library();
        if (!_m_module) return -1;
    }

    // Platform
    _m_spvSoftwareVersionString         = (PFN_SPV_SOFTWARE_VERSION_STRING)         spirv_get_proc("spvSoftwareVersionString");
    _m_spvSoftwareVersionDetailsString  = (PFN_SPV_SOFTWARE_VERSION_DETAILS_STRING)  spirv_get_proc("spvSoftwareVersionDetailsString");

    // Context
    _m_spvContextCreate  = (PFN_SPV_CONTEXT_CREATE)  spirv_get_proc("spvContextCreate");
    _m_spvContextDestroy = (PFN_SPV_CONTEXT_DESTROY) spirv_get_proc("spvContextDestroy");

    // Text <-> Binary
    _m_spvTextToBinary            = (PFN_SPV_TEXT_TO_BINARY)              spirv_get_proc("spvTextToBinary");
    _m_spvTextToBinaryWithOptions = (PFN_SPV_TEXT_TO_BINARY_WITH_OPTIONS) spirv_get_proc("spvTextToBinaryWithOptions");
    _m_spvBinaryToText            = (PFN_SPV_BINARY_TO_TEXT)              spirv_get_proc("spvBinaryToText");
    _m_spvBinaryDestroy           = (PFN_SPV_BINARY_DESTROY)              spirv_get_proc("spvBinaryDestroy");
    _m_spvTextDestroy             = (PFN_SPV_TEXT_DESTROY)                spirv_get_proc("spvTextDestroy");

    // Validation
    _m_spvValidate            = (PFN_SPV_VALIDATE)              spirv_get_proc("spvValidate");
    _m_spvValidateWithOptions = (PFN_SPV_VALIDATE_WITH_OPTIONS) spirv_get_proc("spvValidateWithOptions");
    _m_spvValidateBinary      = (PFN_SPV_VALIDATE_BINARY)       spirv_get_proc("spvValidateBinary");

    // Diagnostic
    _m_spvDiagnosticCreate  = (PFN_SPV_DIAGNOSTIC_CREATE)  spirv_get_proc("spvDiagnosticCreate");
    _m_spvDiagnosticDestroy = (PFN_SPV_DIAGNOSTIC_DESTROY) spirv_get_proc("spvDiagnosticDestroy");
    _m_spvDiagnosticPrint   = (PFN_SPV_DIAGNOSTIC_PRINT)   spirv_get_proc("spvDiagnosticPrint");

    // Validator options
    _m_spvValidatorOptionsCreate                             = (PFN_SPV_VALIDATOR_OPTIONS_CREATE)                             spirv_get_proc("spvValidatorOptionsCreate");
    _m_spvValidatorOptionsDestroy                            = (PFN_SPV_VALIDATOR_OPTIONS_DESTROY)                            spirv_get_proc("spvValidatorOptionsDestroy");
    _m_spvValidatorOptionsSetUniversalLimit                  = (PFN_SPV_VALIDATOR_OPTIONS_SET_UNIVERSAL_LIMIT)                 spirv_get_proc("spvValidatorOptionsSetUniversalLimit");
    _m_spvValidatorOptionsSetRelaxStoreStruct                = (PFN_SPV_VALIDATOR_OPTIONS_SET_RELAX_STORE_STRUCT)              spirv_get_proc("spvValidatorOptionsSetRelaxStoreStruct");
    _m_spvValidatorOptionsSetRelaxLogicalPointer             = (PFN_SPV_VALIDATOR_OPTIONS_SET_RELAX_LOGICAL_POINTER)           spirv_get_proc("spvValidatorOptionsSetRelaxLogicalPointer");
    _m_spvValidatorOptionsSetBeforeHlslLegalization          = (PFN_SPV_VALIDATOR_OPTIONS_SET_BEFORE_HLSL_LEGALIZATION)        spirv_get_proc("spvValidatorOptionsSetBeforeHlslLegalization");
    _m_spvValidatorOptionsSetRelaxBlockLayout                = (PFN_SPV_VALIDATOR_OPTIONS_SET_RELAX_BLOCK_LAYOUT)              spirv_get_proc("spvValidatorOptionsSetRelaxBlockLayout");
    _m_spvValidatorOptionsSetUniformBufferStandardLayout     = (PFN_SPV_VALIDATOR_OPTIONS_SET_UNIFORM_BUFFER_STANDARD_LAYOUT)  spirv_get_proc("spvValidatorOptionsSetUniformBufferStandardLayout");
    _m_spvValidatorOptionsSetScalarBlockLayout               = (PFN_SPV_VALIDATOR_OPTIONS_SET_SCALAR_BLOCK_LAYOUT)             spirv_get_proc("spvValidatorOptionsSetScalarBlockLayout");
    _m_spvValidatorOptionsSetWorkgroupScalarBlockLayout      = (PFN_SPV_VALIDATOR_OPTIONS_SET_WORKGROUP_SCALAR_BLOCK_LAYOUT)   spirv_get_proc("spvValidatorOptionsSetWorkgroupScalarBlockLayout");
    _m_spvValidatorOptionsSetSkipBlockLayout                 = (PFN_SPV_VALIDATOR_OPTIONS_SET_SKIP_BLOCK_LAYOUT)               spirv_get_proc("spvValidatorOptionsSetSkipBlockLayout");
    _m_spvValidatorOptionsSetAllowLocalSizeId                = (PFN_SPV_VALIDATOR_OPTIONS_SET_ALLOW_LOCAL_SIZE_ID)             spirv_get_proc("spvValidatorOptionsSetAllowLocalSizeId");
    _m_spvValidatorOptionsSetAllowOffsetTextureOperand       = (PFN_SPV_VALIDATOR_OPTIONS_SET_ALLOW_OFFSET_TEXTURE_OPERAND)    spirv_get_proc("spvValidatorOptionsSetAllowOffsetTextureOperand");
    _m_spvValidatorOptionsSetAllowVulkan32BitBitwise         = (PFN_SPV_VALIDATOR_OPTIONS_SET_ALLOW_VULKAN32_BIT_BITWISE)      spirv_get_proc("spvValidatorOptionsSetAllowVulkan32BitBitwise");
    _m_spvValidatorOptionsSetFriendlyNames                   = (PFN_SPV_VALIDATOR_OPTIONS_SET_FRIENDLY_NAMES)                  spirv_get_proc("spvValidatorOptionsSetFriendlyNames");

    // Optimizer options
    _m_spvOptimizerOptionsCreate                     = (PFN_SPV_OPTIMIZER_OPTIONS_CREATE)                      spirv_get_proc("spvOptimizerOptionsCreate");
    _m_spvOptimizerOptionsDestroy                    = (PFN_SPV_OPTIMIZER_OPTIONS_DESTROY)                     spirv_get_proc("spvOptimizerOptionsDestroy");
    _m_spvOptimizerOptionsSetRunValidator            = (PFN_SPV_OPTIMIZER_OPTIONS_SET_RUN_VALIDATOR)           spirv_get_proc("spvOptimizerOptionsSetRunValidator");
    _m_spvOptimizerOptionsSetValidatorOptions        = (PFN_SPV_OPTIMIZER_OPTIONS_SET_VALIDATOR_OPTIONS)       spirv_get_proc("spvOptimizerOptionsSetValidatorOptions");
    _m_spvOptimizerOptionsSetMaxIdBound              = (PFN_SPV_OPTIMIZER_OPTIONS_SET_MAX_ID_BOUND)            spirv_get_proc("spvOptimizerOptionsSetMaxIdBound");
    _m_spvOptimizerOptionsSetPreserveBindings        = (PFN_SPV_OPTIMIZER_OPTIONS_SET_PRESERVE_BINDINGS)       spirv_get_proc("spvOptimizerOptionsSetPreserveBindings");
    _m_spvOptimizerOptionsSetPreserveSpecConstants   = (PFN_SPV_OPTIMIZER_OPTIONS_SET_PRESERVE_SPEC_CONSTANTS) spirv_get_proc("spvOptimizerOptionsSetPreserveSpecConstants");

    // Reducer options
    _m_spvReducerOptionsCreate                   = (PFN_SPV_REDUCER_OPTIONS_CREATE)                      spirv_get_proc("spvReducerOptionsCreate");
    _m_spvReducerOptionsDestroy                  = (PFN_SPV_REDUCER_OPTIONS_DESTROY)                     spirv_get_proc("spvReducerOptionsDestroy");
    _m_spvReducerOptionsSetStepLimit             = (PFN_SPV_REDUCER_OPTIONS_SET_STEP_LIMIT)              spirv_get_proc("spvReducerOptionsSetStepLimit");
    _m_spvReducerOptionsSetFailOnValidationError = (PFN_SPV_REDUCER_OPTIONS_SET_FAIL_ON_VALIDATION_ERROR) spirv_get_proc("spvReducerOptionsSetFailOnValidationError");
    _m_spvReducerOptionsSetTargetFunction        = (PFN_SPV_REDUCER_OPTIONS_SET_TARGET_FUNCTION)         spirv_get_proc("spvReducerOptionsSetTargetFunction");

    // Fuzzer options
    _m_spvFuzzerOptionsCreate                       = (PFN_SPV_FUZZER_OPTIONS_CREATE)                        spirv_get_proc("spvFuzzerOptionsCreate");
    _m_spvFuzzerOptionsDestroy                      = (PFN_SPV_FUZZER_OPTIONS_DESTROY)                       spirv_get_proc("spvFuzzerOptionsDestroy");
    _m_spvFuzzerOptionsEnableReplayValidation       = (PFN_SPV_FUZZER_OPTIONS_ENABLE_REPLAY_VALIDATION)      spirv_get_proc("spvFuzzerOptionsEnableReplayValidation");
    _m_spvFuzzerOptionsSetRandomSeed                = (PFN_SPV_FUZZER_OPTIONS_SET_RANDOM_SEED)               spirv_get_proc("spvFuzzerOptionsSetRandomSeed");
    _m_spvFuzzerOptionsSetReplayRange               = (PFN_SPV_FUZZER_OPTIONS_SET_REPLAY_RANGE)              spirv_get_proc("spvFuzzerOptionsSetReplayRange");
    _m_spvFuzzerOptionsSetShrinkerStepLimit         = (PFN_SPV_FUZZER_OPTIONS_SET_SHRINKER_STEP_LIMIT)       spirv_get_proc("spvFuzzerOptionsSetShrinkerStepLimit");
    _m_spvFuzzerOptionsEnableFuzzerPassValidation   = (PFN_SPV_FUZZER_OPTIONS_ENABLE_FUZZER_PASS_VALIDATION) spirv_get_proc("spvFuzzerOptionsEnableFuzzerPassValidation");
    _m_spvFuzzerOptionsEnableAllPasses              = (PFN_SPV_FUZZER_OPTIONS_ENABLE_ALL_PASSES)             spirv_get_proc("spvFuzzerOptionsEnableAllPasses");

    // Opcode & Parser
    _m_spvOpcodeString  = (PFN_SPV_OPCODE_STRING) spirv_get_proc("spvOpcodeString");
    _m_spvBinaryParse   = (PFN_SPV_BINARY_PARSE)  spirv_get_proc("spvBinaryParse");

    // Optimizer
    _m_spvOptimizerCreate                                        = (PFN_SPV_OPTIMIZER_CREATE)                                        spirv_get_proc("spvOptimizerCreate");
    _m_spvOptimizerDestroy                                       = (PFN_SPV_OPTIMIZER_DESTROY)                                       spirv_get_proc("spvOptimizerDestroy");
    _m_spvOptimizerSetMessageConsumer                            = (PFN_SPV_OPTIMIZER_SET_MESSAGE_CONSUMER)                          spirv_get_proc("spvOptimizerSetMessageConsumer");
    _m_spvOptimizerRegisterLegalizationPasses                    = (PFN_SPV_OPTIMIZER_REGISTER_LEGALIZATION_PASSES)                  spirv_get_proc("spvOptimizerRegisterLegalizationPasses");
    _m_spvOptimizerRegisterPerformancePasses                     = (PFN_SPV_OPTIMIZER_REGISTER_PERFORMANCE_PASSES)                   spirv_get_proc("spvOptimizerRegisterPerformancePasses");
    _m_spvOptimizerRegisterSizePasses                            = (PFN_SPV_OPTIMIZER_REGISTER_SIZE_PASSES)                          spirv_get_proc("spvOptimizerRegisterSizePasses");
    _m_spvOptimizerRegisterPassFromFlag                          = (PFN_SPV_OPTIMIZER_REGISTER_PASS_FROM_FLAG)                       spirv_get_proc("spvOptimizerRegisterPassFromFlag");
    _m_spvOptimizerRegisterPassesFromFlags                       = (PFN_SPV_OPTIMIZER_REGISTER_PASSES_FROM_FLAGS)                    spirv_get_proc("spvOptimizerRegisterPassesFromFlags");
    _m_spvOptimizerRegisterPassesFromFlagsWhilePreservingTheInterface = (PFN_SPV_OPTIMIZER_REGISTER_PASSES_FROM_FLAGS_PRESERVING_INTERFACE) spirv_get_proc("spvOptimizerRegisterPassesFromFlagsWhilePreservingTheInterface");
    _m_spvOptimizerRun                                           = (PFN_SPV_OPTIMIZER_RUN)                                           spirv_get_proc("spvOptimizerRun");

    // check errors
    if (!_m_spvContextCreate || !_m_spvContextDestroy ||
        !_m_spvTextToBinary  || !_m_spvBinaryToText   ||
        !_m_spvBinaryDestroy || !_m_spvTextDestroy    ||
        !_m_spvValidate      || !_m_spvValidatorOptionsCreate) {
        fprintf(stderr, "failed to load SPIRV-Tools functions\n");
        return -1;
    }

    return 0;
}

//
// functions
//

const char* spvSoftwareVersionString(void)         { return _m_spvSoftwareVersionString(); }
const char* spvSoftwareVersionDetailsString(void)  { return _m_spvSoftwareVersionDetailsString(); }

spv_context spvContextCreate(spv_target_env env)   { return _m_spvContextCreate(env); }
void spvContextDestroy(spv_context context)        { _m_spvContextDestroy(context); }

spv_result_t spvTextToBinary(const spv_const_context context, const char* text,
                             const size_t length, spv_binary* binary,
                             spv_diagnostic* diagnostic)
{
    return _m_spvTextToBinary(context, text, length, binary, diagnostic);
}

spv_result_t spvTextToBinaryWithOptions(const spv_const_context context,
                                        const char* text, const size_t length,
                                        const uint32_t options,
                                        spv_binary* binary,
                                        spv_diagnostic* diagnostic)
{
    return _m_spvTextToBinaryWithOptions(context, text, length, options, binary, diagnostic);
}

spv_result_t spvBinaryToText(const spv_const_context context,
                             const uint32_t* binary, const size_t word_count,
                             const uint32_t options, spv_text* text,
                             spv_diagnostic* diagnostic)
{
    return _m_spvBinaryToText(context, binary, word_count, options, text, diagnostic);
}

void spvBinaryDestroy(spv_binary binary) { _m_spvBinaryDestroy(binary); }
void spvTextDestroy(spv_text text)       { _m_spvTextDestroy(text); }

spv_result_t spvValidate(const spv_const_context context,
                         const spv_const_binary binary,
                         spv_diagnostic* diagnostic)
{
    return _m_spvValidate(context, binary, diagnostic);
}

spv_result_t spvValidateWithOptions(const spv_const_context context,
                                    const spv_const_validator_options options,
                                    const spv_const_binary binary,
                                    spv_diagnostic* diagnostic)
{
    return _m_spvValidateWithOptions(context, options, binary, diagnostic);
}

spv_result_t spvValidateBinary(const spv_const_context context,
                               const uint32_t* words, const size_t num_words,
                               spv_diagnostic* diagnostic)
{
    return _m_spvValidateBinary(context, words, num_words, diagnostic);
}

spv_diagnostic spvDiagnosticCreate(const spv_position position,
                                   const char* message)
{
    return _m_spvDiagnosticCreate(position, message);
}
void spvDiagnosticDestroy(spv_diagnostic diagnostic) { _m_spvDiagnosticDestroy(diagnostic); }
spv_result_t spvDiagnosticPrint(const spv_diagnostic diagnostic) { return _m_spvDiagnosticPrint(diagnostic); }

spv_validator_options spvValidatorOptionsCreate(void) { return _m_spvValidatorOptionsCreate(); }
void spvValidatorOptionsDestroy(spv_validator_options options) { _m_spvValidatorOptionsDestroy(options); }
void spvValidatorOptionsSetUniversalLimit(spv_validator_options options,
                                          spv_validator_limit limit_type,
                                          uint32_t limit)
{
    _m_spvValidatorOptionsSetUniversalLimit(options, limit_type, limit);
}
void spvValidatorOptionsSetRelaxStoreStruct(spv_validator_options options, bool val)
{ _m_spvValidatorOptionsSetRelaxStoreStruct(options, val); }
void spvValidatorOptionsSetRelaxLogicalPointer(spv_validator_options options, bool val)
{ _m_spvValidatorOptionsSetRelaxLogicalPointer(options, val); }
void spvValidatorOptionsSetBeforeHlslLegalization(spv_validator_options options, bool val)
{ _m_spvValidatorOptionsSetBeforeHlslLegalization(options, val); }
void spvValidatorOptionsSetRelaxBlockLayout(spv_validator_options options, bool val)
{ _m_spvValidatorOptionsSetRelaxBlockLayout(options, val); }
void spvValidatorOptionsSetUniformBufferStandardLayout(spv_validator_options options, bool val)
{ _m_spvValidatorOptionsSetUniformBufferStandardLayout(options, val); }
void spvValidatorOptionsSetScalarBlockLayout(spv_validator_options options, bool val)
{ _m_spvValidatorOptionsSetScalarBlockLayout(options, val); }
void spvValidatorOptionsSetWorkgroupScalarBlockLayout(spv_validator_options options, bool val)
{ _m_spvValidatorOptionsSetWorkgroupScalarBlockLayout(options, val); }
void spvValidatorOptionsSetSkipBlockLayout(spv_validator_options options, bool val)
{ _m_spvValidatorOptionsSetSkipBlockLayout(options, val); }
void spvValidatorOptionsSetAllowLocalSizeId(spv_validator_options options, bool val)
{ _m_spvValidatorOptionsSetAllowLocalSizeId(options, val); }
void spvValidatorOptionsSetAllowOffsetTextureOperand(spv_validator_options options, bool val)
{ _m_spvValidatorOptionsSetAllowOffsetTextureOperand(options, val); }
void spvValidatorOptionsSetAllowVulkan32BitBitwise(spv_validator_options options, bool val)
{ _m_spvValidatorOptionsSetAllowVulkan32BitBitwise(options, val); }
void spvValidatorOptionsSetFriendlyNames(spv_validator_options options, bool val)
{ _m_spvValidatorOptionsSetFriendlyNames(options, val); }

spv_optimizer_options spvOptimizerOptionsCreate(void) { return _m_spvOptimizerOptionsCreate(); }
void spvOptimizerOptionsDestroy(spv_optimizer_options options) { _m_spvOptimizerOptionsDestroy(options); }
void spvOptimizerOptionsSetRunValidator(spv_optimizer_options options, bool val)
{ _m_spvOptimizerOptionsSetRunValidator(options, val); }
void spvOptimizerOptionsSetValidatorOptions(spv_optimizer_options options, spv_validator_options val)
{ _m_spvOptimizerOptionsSetValidatorOptions(options, val); }
void spvOptimizerOptionsSetMaxIdBound(spv_optimizer_options options, uint32_t val)
{ _m_spvOptimizerOptionsSetMaxIdBound(options, val); }
void spvOptimizerOptionsSetPreserveBindings(spv_optimizer_options options, bool val)
{ _m_spvOptimizerOptionsSetPreserveBindings(options, val); }
void spvOptimizerOptionsSetPreserveSpecConstants(spv_optimizer_options options, bool val)
{ _m_spvOptimizerOptionsSetPreserveSpecConstants(options, val); }

spv_reducer_options spvReducerOptionsCreate(void) { return _m_spvReducerOptionsCreate(); }
void spvReducerOptionsDestroy(spv_reducer_options options) { _m_spvReducerOptionsDestroy(options); }
void spvReducerOptionsSetStepLimit(spv_reducer_options options, uint32_t step_limit)
{ _m_spvReducerOptionsSetStepLimit(options, step_limit); }
void spvReducerOptionsSetFailOnValidationError(spv_reducer_options options, bool fail_on_validation_error)
{ _m_spvReducerOptionsSetFailOnValidationError(options, fail_on_validation_error); }
void spvReducerOptionsSetTargetFunction(spv_reducer_options options, uint32_t target_function)
{ _m_spvReducerOptionsSetTargetFunction(options, target_function); }

spv_fuzzer_options spvFuzzerOptionsCreate(void) { return _m_spvFuzzerOptionsCreate(); }
void spvFuzzerOptionsDestroy(spv_fuzzer_options options) { _m_spvFuzzerOptionsDestroy(options); }
void spvFuzzerOptionsEnableReplayValidation(spv_fuzzer_options options)
{ _m_spvFuzzerOptionsEnableReplayValidation(options); }
void spvFuzzerOptionsSetRandomSeed(spv_fuzzer_options options, uint32_t seed)
{ _m_spvFuzzerOptionsSetRandomSeed(options, seed); }
void spvFuzzerOptionsSetReplayRange(spv_fuzzer_options options, int32_t replay_range)
{ _m_spvFuzzerOptionsSetReplayRange(options, replay_range); }
void spvFuzzerOptionsSetShrinkerStepLimit(spv_fuzzer_options options, uint32_t shrinker_step_limit)
{ _m_spvFuzzerOptionsSetShrinkerStepLimit(options, shrinker_step_limit); }
void spvFuzzerOptionsEnableFuzzerPassValidation(spv_fuzzer_options options)
{ _m_spvFuzzerOptionsEnableFuzzerPassValidation(options); }
void spvFuzzerOptionsEnableAllPasses(spv_fuzzer_options options)
{ _m_spvFuzzerOptionsEnableAllPasses(options); }

const char* spvOpcodeString(const uint32_t opcode) { return _m_spvOpcodeString(opcode); }

spv_result_t spvBinaryParse(const spv_const_context context, void* user_data,
                            const uint32_t* words, const size_t num_words,
                            spv_parsed_header_fn_t parse_header,
                            spv_parsed_instruction_fn_t parse_instruction,
                            spv_diagnostic* diagnostic)
{
    return _m_spvBinaryParse(context, user_data, words, num_words,
                             parse_header, parse_instruction, diagnostic);
}

spv_optimizer_t* spvOptimizerCreate(spv_target_env env) { return _m_spvOptimizerCreate(env); }
void spvOptimizerDestroy(spv_optimizer_t* optimizer) { _m_spvOptimizerDestroy(optimizer); }
void spvOptimizerSetMessageConsumer(spv_optimizer_t* optimizer, spv_message_consumer consumer)
{ _m_spvOptimizerSetMessageConsumer(optimizer, consumer); }
void spvOptimizerRegisterLegalizationPasses(spv_optimizer_t* optimizer)
{ _m_spvOptimizerRegisterLegalizationPasses(optimizer); }
void spvOptimizerRegisterPerformancePasses(spv_optimizer_t* optimizer)
{ _m_spvOptimizerRegisterPerformancePasses(optimizer); }
void spvOptimizerRegisterSizePasses(spv_optimizer_t* optimizer)
{ _m_spvOptimizerRegisterSizePasses(optimizer); }
bool spvOptimizerRegisterPassFromFlag(spv_optimizer_t* optimizer, const char* flag)
{ return _m_spvOptimizerRegisterPassFromFlag(optimizer, flag); }
bool spvOptimizerRegisterPassesFromFlags(spv_optimizer_t* optimizer, const char** flags, const size_t flag_count)
{ return _m_spvOptimizerRegisterPassesFromFlags(optimizer, flags, flag_count); }
bool spvOptimizerRegisterPassesFromFlagsWhilePreservingTheInterface(spv_optimizer_t* optimizer, const char** flags, const size_t flag_count)
{ return _m_spvOptimizerRegisterPassesFromFlagsWhilePreservingTheInterface(optimizer, flags, flag_count); }
spv_result_t spvOptimizerRun(spv_optimizer_t* optimizer, const uint32_t* binary,
                             const size_t word_count, spv_binary* optimized_binary,
                             const spv_optimizer_options options)
{
    return _m_spvOptimizerRun(optimizer, binary, word_count, optimized_binary, options);
}

#ifdef __cplusplus
} // extern "C"
#endif
