#ifndef SPIRV_TOOLS_LOADER_H
#define SPIRV_TOOLS_LOADER_H

#include <spirv-tools/libspirv.h>

#ifdef __cplusplus
extern "C"
{
#endif

// Platform
typedef const char* (*PFN_SPV_SOFTWARE_VERSION_STRING)(void);
typedef const char* (*PFN_SPV_SOFTWARE_VERSION_DETAILS_STRING)(void);

// Context
typedef spv_context (*PFN_SPV_CONTEXT_CREATE)(spv_target_env env);
typedef void (*PFN_SPV_CONTEXT_DESTROY)(spv_context context);

// Text <-> Binary
typedef spv_result_t (*PFN_SPV_TEXT_TO_BINARY)(const spv_const_context context, const char* text, const size_t length, spv_binary* binary, spv_diagnostic* diagnostic);
typedef spv_result_t (*PFN_SPV_TEXT_TO_BINARY_WITH_OPTIONS)(const spv_const_context context, const char* text, const size_t length, const uint32_t options, spv_binary* binary, spv_diagnostic* diagnostic);
typedef spv_result_t (*PFN_SPV_BINARY_TO_TEXT)(const spv_const_context context, const uint32_t* binary, const size_t word_count, const uint32_t options, spv_text* text, spv_diagnostic* diagnostic);
typedef void (*PFN_SPV_BINARY_DESTROY)(spv_binary binary);
typedef void (*PFN_SPV_TEXT_DESTROY)(spv_text text);

// Validation
typedef spv_result_t (*PFN_SPV_VALIDATE)(const spv_const_context context, const spv_const_binary binary, spv_diagnostic* diagnostic);
typedef spv_result_t (*PFN_SPV_VALIDATE_WITH_OPTIONS)(const spv_const_context context, const spv_const_validator_options options, const spv_const_binary binary, spv_diagnostic* diagnostic);
typedef spv_result_t (*PFN_SPV_VALIDATE_BINARY)(const spv_const_context context, const uint32_t* words, const size_t num_words, spv_diagnostic* diagnostic);

// Diagnostic
typedef spv_diagnostic (*PFN_SPV_DIAGNOSTIC_CREATE)(const spv_position position, const char* message);
typedef void (*PFN_SPV_DIAGNOSTIC_DESTROY)(spv_diagnostic diagnostic);
typedef spv_result_t (*PFN_SPV_DIAGNOSTIC_PRINT)(const spv_diagnostic diagnostic);

// Validator options
typedef spv_validator_options (*PFN_SPV_VALIDATOR_OPTIONS_CREATE)(void);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_DESTROY)(spv_validator_options options);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_UNIVERSAL_LIMIT)(spv_validator_options options, spv_validator_limit limit_type, uint32_t limit);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_RELAX_STORE_STRUCT)(spv_validator_options options, bool val);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_RELAX_LOGICAL_POINTER)(spv_validator_options options, bool val);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_BEFORE_HLSL_LEGALIZATION)(spv_validator_options options, bool val);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_RELAX_BLOCK_LAYOUT)(spv_validator_options options, bool val);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_UNIFORM_BUFFER_STANDARD_LAYOUT)(spv_validator_options options, bool val);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_SCALAR_BLOCK_LAYOUT)(spv_validator_options options, bool val);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_WORKGROUP_SCALAR_BLOCK_LAYOUT)(spv_validator_options options, bool val);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_SKIP_BLOCK_LAYOUT)(spv_validator_options options, bool val);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_ALLOW_LOCAL_SIZE_ID)(spv_validator_options options, bool val);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_ALLOW_OFFSET_TEXTURE_OPERAND)(spv_validator_options options, bool val);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_ALLOW_VULKAN32_BIT_BITWISE)(spv_validator_options options, bool val);
typedef void (*PFN_SPV_VALIDATOR_OPTIONS_SET_FRIENDLY_NAMES)(spv_validator_options options, bool val);

// Optimizer options
typedef spv_optimizer_options (*PFN_SPV_OPTIMIZER_OPTIONS_CREATE)(void);
typedef void (*PFN_SPV_OPTIMIZER_OPTIONS_DESTROY)(spv_optimizer_options options);
typedef void (*PFN_SPV_OPTIMIZER_OPTIONS_SET_RUN_VALIDATOR)(spv_optimizer_options options, bool val);
typedef void (*PFN_SPV_OPTIMIZER_OPTIONS_SET_VALIDATOR_OPTIONS)(spv_optimizer_options options, spv_validator_options val);
typedef void (*PFN_SPV_OPTIMIZER_OPTIONS_SET_MAX_ID_BOUND)(spv_optimizer_options options, uint32_t val);
typedef void (*PFN_SPV_OPTIMIZER_OPTIONS_SET_PRESERVE_BINDINGS)(spv_optimizer_options options, bool val);
typedef void (*PFN_SPV_OPTIMIZER_OPTIONS_SET_PRESERVE_SPEC_CONSTANTS)(spv_optimizer_options options, bool val);

// Reducer options
typedef spv_reducer_options (*PFN_SPV_REDUCER_OPTIONS_CREATE)(void);
typedef void (*PFN_SPV_REDUCER_OPTIONS_DESTROY)(spv_reducer_options options);
typedef void (*PFN_SPV_REDUCER_OPTIONS_SET_STEP_LIMIT)(spv_reducer_options options, uint32_t step_limit);
typedef void (*PFN_SPV_REDUCER_OPTIONS_SET_FAIL_ON_VALIDATION_ERROR)(spv_reducer_options options, bool fail_on_validation_error);
typedef void (*PFN_SPV_REDUCER_OPTIONS_SET_TARGET_FUNCTION)(spv_reducer_options options, uint32_t target_function);

// Fuzzer options
typedef spv_fuzzer_options (*PFN_SPV_FUZZER_OPTIONS_CREATE)(void);
typedef void (*PFN_SPV_FUZZER_OPTIONS_DESTROY)(spv_fuzzer_options options);
typedef void (*PFN_SPV_FUZZER_OPTIONS_ENABLE_REPLAY_VALIDATION)(spv_fuzzer_options options);
typedef void (*PFN_SPV_FUZZER_OPTIONS_SET_RANDOM_SEED)(spv_fuzzer_options options, uint32_t seed);
typedef void (*PFN_SPV_FUZZER_OPTIONS_SET_REPLAY_RANGE)(spv_fuzzer_options options, int32_t replay_range);
typedef void (*PFN_SPV_FUZZER_OPTIONS_SET_SHRINKER_STEP_LIMIT)(spv_fuzzer_options options, uint32_t shrinker_step_limit);
typedef void (*PFN_SPV_FUZZER_OPTIONS_ENABLE_FUZZER_PASS_VALIDATION)(spv_fuzzer_options options);
typedef void (*PFN_SPV_FUZZER_OPTIONS_ENABLE_ALL_PASSES)(spv_fuzzer_options options);

// Opcode
typedef const char* (*PFN_SPV_OPCODE_STRING)(const uint32_t opcode);

// Binary parser
typedef spv_result_t (*PFN_SPV_BINARY_PARSE)(const spv_const_context context, void* user_data, const uint32_t* words, const size_t num_words, spv_parsed_header_fn_t parse_header, spv_parsed_instruction_fn_t parse_instruction, spv_diagnostic* diagnostic);

// Optimizer
typedef spv_optimizer_t* (*PFN_SPV_OPTIMIZER_CREATE)(spv_target_env env);
typedef void (*PFN_SPV_OPTIMIZER_DESTROY)(spv_optimizer_t* optimizer);
typedef void (*PFN_SPV_OPTIMIZER_SET_MESSAGE_CONSUMER)(spv_optimizer_t* optimizer, spv_message_consumer consumer);
typedef void (*PFN_SPV_OPTIMIZER_REGISTER_LEGALIZATION_PASSES)(spv_optimizer_t* optimizer);
typedef void (*PFN_SPV_OPTIMIZER_REGISTER_PERFORMANCE_PASSES)(spv_optimizer_t* optimizer);
typedef void (*PFN_SPV_OPTIMIZER_REGISTER_SIZE_PASSES)(spv_optimizer_t* optimizer);
typedef bool (*PFN_SPV_OPTIMIZER_REGISTER_PASS_FROM_FLAG)(spv_optimizer_t* optimizer, const char* flag);
typedef bool (*PFN_SPV_OPTIMIZER_REGISTER_PASSES_FROM_FLAGS)(spv_optimizer_t* optimizer, const char** flags, const size_t flag_count);
typedef bool (*PFN_SPV_OPTIMIZER_REGISTER_PASSES_FROM_FLAGS_PRESERVING_INTERFACE)(spv_optimizer_t* optimizer, const char** flags, const size_t flag_count);
typedef spv_result_t (*PFN_SPV_OPTIMIZER_RUN)(spv_optimizer_t* optimizer, const uint32_t* binary, const size_t word_count, spv_binary* optimized_binary, const spv_optimizer_options options);

// load spirv-tools functions
int load_spirv_tools(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // SPIRV_TOOLS_LOADER_H
