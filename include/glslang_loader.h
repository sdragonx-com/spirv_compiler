#ifndef GLSLANG_LOADER_H
#define GLSLANG_LOADER_H

#include <glslang/Include/glslang_c_interface.h>
#include <glslang/Public/resource_limits_c.h>

#ifdef __cplusplus
extern "C"
{
#endif

// glslang_c_interface.h
// glslang_get_version
typedef int (*PFN_GLSLANG_INITIALIZE_PROCESS)(void);
typedef void (*PFN_GLSLANG_FINALIZE_PROCESS)(void);
typedef glslang_shader_t* (*PFN_GLSLANG_SHADER_CREATE)(const glslang_input_t*);
typedef void (*PFN_GLSLANG_SHADER_DELETE)(glslang_shader_t*);
typedef void (*PFN_GLSLANG_SHADER_SET_PREAMBLE)(glslang_shader_t*, const char*);
typedef void (*PFN_GLSLANG_SHADER_SET_ENTRY_POINT)(glslang_shader_t*, const char*);
// glslang_shader_set_invert_y
typedef void (*PFN_GLSLANG_SHADER_SHIFT_BINDING)(glslang_shader_t*, glslang_resource_type_t, unsigned int);
typedef void (*PFN_GLSLANG_SHADER_SHIFT_BINDING_FOR_SET)(glslang_shader_t*, glslang_resource_type_t, unsigned int, unsigned int);
typedef void (*PFN_GLSLANG_SHADER_SET_OPTIONS)(glslang_shader_t*, int);
typedef void (*PFN_GLSLANG_SHADER_SET_GLSL_VERSION)(glslang_shader_t*, int);
typedef int (*PFN_GLSLANG_SHADER_PREPROCESS)(glslang_shader_t*, const glslang_input_t*);
typedef int (*PFN_GLSLANG_SHADER_PARSE)(glslang_shader_t*, const glslang_input_t*);
typedef const char* (*PFN_GLSLANG_SHADER_GET_PREPROCESSED_CODE)(glslang_shader_t*);
typedef const char* (*PFN_GLSLANG_SHADER_GET_INFO_LOG)(glslang_shader_t*);
typedef const char* (*PFN_GLSLANG_SHADER_GET_INFO_DEBUG_LOG)(glslang_shader_t*);

typedef glslang_program_t* (*PFN_GLSLANG_PROGRAM_CREATE)(void);
typedef void (*PFN_GLSLANG_PROGRAM_DELETE)(glslang_program_t*);
typedef void (*PFN_GLSLANG_PROGRAM_ADD_SHADER)(glslang_program_t*, glslang_shader_t*);
typedef int (*PFN_GLSLANG_PROGRAM_LINK)(glslang_program_t*, int);
typedef void (*PFN_GLSLANG_PROGRAM_ADD_SOURCE_TEXT)(glslang_program_t*, glslang_stage_t, const char*, size_t);
typedef void (*PFN_GLSLANG_PROGRAM_SET_SOURCE_FILE)(glslang_program_t*, glslang_stage_t, const char*);
typedef int (*PFN_GLSLANG_PROGRAM_MAP_IO)(glslang_program_t*);
typedef void (*PFN_GLSLANG_PROGRAM_SPIRV_GENERATE)(glslang_program_t*, glslang_stage_t);
typedef void (*PFN_GLSLANG_PROGRAM_SPIRV_GENERATE_WITH_OPTIONS)(glslang_program_t*, glslang_stage_t, glslang_spv_options_t*);
typedef size_t (*PFN_GLSLANG_PROGRAM_SPIRV_GET_SIZE)(glslang_program_t*);
typedef void (*PFN_GLSLANG_PROGRAM_SPIRV_GET)(glslang_program_t*, unsigned int*);
typedef unsigned int* (*PFN_GLSLANG_PROGRAM_SPIRV_GET_PTR)(glslang_program_t*);
typedef const char* (*PFN_GLSLANG_PROGRAM_SPIRV_GET_MESSAGES)(glslang_program_t*);
typedef const char* (*PFN_GLSLANG_PROGRAM_GET_INFO_LOG)(glslang_program_t*);
typedef const char* (*PFN_GLSLANG_PROGRAM_GET_INFO_DEBUG_LOG)(glslang_program_t*);

// resource_limits_c.h
#define RESOURCE_LIMITS_C
#ifdef RESOURCE_LIMITS_C
typedef glslang_resource_t* (*PFN_GLSLANG_RESOURCE)(void);
typedef const glslang_resource_t* (*PFN_GLSLANG_DEFAULT_RESOURCE)(void);
typedef const char* (*PFN_GLSLANG_DEFAULT_RESOURCE_STRING)();
typedef void (*PFN_GLSLANG_DECODE_RESOURCE_LIMITS)(glslang_resource_t*, char*);
#endif

// load glslang functions
int load_glslang();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // GLSLANG_LOADER_H
