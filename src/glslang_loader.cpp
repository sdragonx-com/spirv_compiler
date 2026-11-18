#include "glslang_loader.h"
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32
typedef HMODULE glslang_module;
typedef FARPROC glslang_proc;
#else
typedef void* glslang_module;
typedef void* glslang_proc;
#endif

static glslang_module _m_module = NULL;

static void free_glslang_library()
{
    if (_m_module) {
        #ifdef _WIN32
        FreeLibrary(_m_module);
        #else
        dlclose(_m_module);
        #endif
    }
}

static glslang_module load_glslang_library()
{
    glslang_module handle = NULL;

    #ifdef _WIN32
    const char* LIBS[] = { "glslang.dll", "glslang_shared.dll", "libglslang.dll" };

    for (size_t i = 0; i < _countof(LIBS); ++i) {
        handle = LoadLibraryA(LIBS[i]);
        if (handle) {
            return handle;
        }
    }

    if (!handle) {
        printf("load \"glslang.dll\" failed.\n");
    }

    #else
    const char* LIBS[] = { "libglslang.so", "libglslang.so.1", "libglslang.so.0" };

    for (size_t i = 0; i < _countof(LIBS); ++i) {
        handle = dlopen(LIBS[i], RTLD_LAZY | RTLD_LOCAL);
        if (handle) {
            return handle;
        }
    }

    if (!handle) {
        printf("load \"glslang.so\" failed.\n");
    }

    #endif

    if (handle) {
        atexit(free_glslang_library);
    }

    return handle;
}

static glslang_proc glslang_get_proc(const char* name)
{
    #ifdef _WIN32
    return GetProcAddress(_m_module, name);
    #else
    return dlsym(_m_module, name);
    #endif
}

// glslang_c_interface.h
PFN_GLSLANG_INITIALIZE_PROCESS _m_glslang_initialize_process                     = NULL;
PFN_GLSLANG_FINALIZE_PROCESS _m_glslang_finalize_process                         = NULL;
PFN_GLSLANG_SHADER_CREATE _m_glslang_shader_create                               = NULL;
PFN_GLSLANG_SHADER_DELETE _m_glslang_shader_delete                               = NULL;
PFN_GLSLANG_SHADER_SET_PREAMBLE _m_glslang_shader_set_preamble                   = NULL;
PFN_GLSLANG_SHADER_SET_ENTRY_POINT _m_glslang_shader_set_entry_point             = NULL;
PFN_GLSLANG_SHADER_SHIFT_BINDING _m_glslang_shader_shift_binding                 = NULL;
PFN_GLSLANG_SHADER_SHIFT_BINDING_FOR_SET _m_glslang_shader_shift_binding_for_set = NULL;
PFN_GLSLANG_SHADER_SET_OPTIONS _m_glslang_shader_set_options                     = NULL;
PFN_GLSLANG_SHADER_SET_GLSL_VERSION _m_glslang_shader_set_glsl_version           = NULL;
PFN_GLSLANG_SHADER_PREPROCESS _m_glslang_shader_preprocess                       = NULL;
PFN_GLSLANG_SHADER_PARSE _m_glslang_shader_parse                                 = NULL;
PFN_GLSLANG_SHADER_GET_PREPROCESSED_CODE _m_glslang_shader_get_preprocessed_code = NULL;
PFN_GLSLANG_SHADER_GET_INFO_LOG _m_glslang_shader_get_info_log                   = NULL;
PFN_GLSLANG_SHADER_GET_INFO_DEBUG_LOG _m_glslang_shader_get_info_debug_log       = NULL;

PFN_GLSLANG_PROGRAM_CREATE _m_glslang_program_create                                           = NULL;
PFN_GLSLANG_PROGRAM_DELETE _m_glslang_program_delete                                           = NULL;
PFN_GLSLANG_PROGRAM_ADD_SHADER _m_glslang_program_add_shader                                   = NULL;
PFN_GLSLANG_PROGRAM_LINK _m_glslang_program_link                                               = NULL;
PFN_GLSLANG_PROGRAM_ADD_SOURCE_TEXT _m_glslang_program_add_source_text                         = NULL;
PFN_GLSLANG_PROGRAM_SET_SOURCE_FILE _m_glslang_program_set_source_file                         = NULL;
PFN_GLSLANG_PROGRAM_MAP_IO _m_glslang_program_map_io                                           = NULL;
PFN_GLSLANG_PROGRAM_SPIRV_GENERATE _m_glslang_program_SPIRV_generate                           = NULL;
PFN_GLSLANG_PROGRAM_SPIRV_GENERATE_WITH_OPTIONS _m_glslang_program_SPIRV_generate_with_options = NULL;
PFN_GLSLANG_PROGRAM_SPIRV_GET_SIZE _m_glslang_program_SPIRV_get_size                           = NULL;
PFN_GLSLANG_PROGRAM_SPIRV_GET _m_glslang_program_SPIRV_get                                     = NULL;
PFN_GLSLANG_PROGRAM_SPIRV_GET_PTR _m_glslang_program_SPIRV_get_ptr                             = NULL;
PFN_GLSLANG_PROGRAM_SPIRV_GET_MESSAGES _m_glslang_program_SPIRV_get_messages                   = NULL;
PFN_GLSLANG_PROGRAM_GET_INFO_LOG _m_glslang_program_get_info_log                               = NULL;
PFN_GLSLANG_PROGRAM_GET_INFO_DEBUG_LOG _m_glslang_program_get_info_debug_log                   = NULL;

// resource_limits_c.h
#ifdef RESOURCE_LIMITS_C
PFN_GLSLANG_RESOURCE _m_glslang_resource                               = NULL;
PFN_GLSLANG_DEFAULT_RESOURCE _m_glslang_default_resource               = NULL;
PFN_GLSLANG_DEFAULT_RESOURCE_STRING _m_glslang_default_resource_string = NULL;
PFN_GLSLANG_DECODE_RESOURCE_LIMITS _m_glslang_decode_resource_limits   = NULL;
#endif

// load glslang functions
int load_glslang()
{
    if (!_m_module) {
        _m_module = load_glslang_library();
        if (!_m_module) {
            return -1;
        }
    }

    // glslang_c_interface.h
    _m_glslang_initialize_process           = (PFN_GLSLANG_INITIALIZE_PROCESS) glslang_get_proc("glslang_initialize_process");
    _m_glslang_finalize_process             = (PFN_GLSLANG_FINALIZE_PROCESS) glslang_get_proc("glslang_finalize_process");
    _m_glslang_shader_create                = (PFN_GLSLANG_SHADER_CREATE) glslang_get_proc("glslang_shader_create");
    _m_glslang_shader_delete                = (PFN_GLSLANG_SHADER_DELETE) glslang_get_proc("glslang_shader_delete");
    _m_glslang_shader_set_preamble          = (PFN_GLSLANG_SHADER_SET_PREAMBLE) glslang_get_proc("glslang_shader_set_preamble");
    _m_glslang_shader_set_entry_point       = (PFN_GLSLANG_SHADER_SET_ENTRY_POINT) glslang_get_proc("glslang_shader_set_entry_point");
    _m_glslang_shader_shift_binding         = (PFN_GLSLANG_SHADER_SHIFT_BINDING) glslang_get_proc("glslang_shader_shift_binding");
    _m_glslang_shader_shift_binding_for_set = (PFN_GLSLANG_SHADER_SHIFT_BINDING_FOR_SET) glslang_get_proc("glslang_shader_shift_binding_for_set");
    _m_glslang_shader_set_options           = (PFN_GLSLANG_SHADER_SET_OPTIONS) glslang_get_proc("glslang_shader_set_options");
    _m_glslang_shader_set_glsl_version      = (PFN_GLSLANG_SHADER_SET_GLSL_VERSION) glslang_get_proc("glslang_shader_set_glsl_version");
    _m_glslang_shader_preprocess            = (PFN_GLSLANG_SHADER_PREPROCESS) glslang_get_proc("glslang_shader_preprocess");
    _m_glslang_shader_parse                 = (PFN_GLSLANG_SHADER_PARSE) glslang_get_proc("glslang_shader_parse");
    _m_glslang_shader_get_preprocessed_code = (PFN_GLSLANG_SHADER_GET_PREPROCESSED_CODE) glslang_get_proc("glslang_shader_get_preprocessed_code");
    _m_glslang_shader_get_info_log          = (PFN_GLSLANG_SHADER_GET_INFO_LOG) glslang_get_proc("glslang_shader_get_info_log");
    _m_glslang_shader_get_info_debug_log    = (PFN_GLSLANG_SHADER_GET_INFO_DEBUG_LOG) glslang_get_proc("glslang_shader_get_info_debug_log");

    _m_glslang_program_create                      = (PFN_GLSLANG_PROGRAM_CREATE) glslang_get_proc("glslang_program_create");
    _m_glslang_program_delete                      = (PFN_GLSLANG_PROGRAM_DELETE) glslang_get_proc("glslang_program_delete");
    _m_glslang_program_add_shader                  = (PFN_GLSLANG_PROGRAM_ADD_SHADER) glslang_get_proc("glslang_program_add_shader");
    _m_glslang_program_link                        = (PFN_GLSLANG_PROGRAM_LINK) glslang_get_proc("glslang_program_link");
    _m_glslang_program_add_source_text             = (PFN_GLSLANG_PROGRAM_ADD_SOURCE_TEXT) glslang_get_proc("glslang_program_add_source_text");
    _m_glslang_program_set_source_file             = (PFN_GLSLANG_PROGRAM_SET_SOURCE_FILE) glslang_get_proc("glslang_program_set_source_file");
    _m_glslang_program_map_io                      = (PFN_GLSLANG_PROGRAM_MAP_IO) glslang_get_proc("glslang_program_map_io");
    _m_glslang_program_SPIRV_generate              = (PFN_GLSLANG_PROGRAM_SPIRV_GENERATE) glslang_get_proc("glslang_program_SPIRV_generate");
    _m_glslang_program_SPIRV_generate_with_options = (PFN_GLSLANG_PROGRAM_SPIRV_GENERATE_WITH_OPTIONS) glslang_get_proc("glslang_program_SPIRV_generate_with_options");
    _m_glslang_program_SPIRV_get_size              = (PFN_GLSLANG_PROGRAM_SPIRV_GET_SIZE) glslang_get_proc("glslang_program_SPIRV_get_size");
    _m_glslang_program_SPIRV_get                   = (PFN_GLSLANG_PROGRAM_SPIRV_GET) glslang_get_proc("glslang_program_SPIRV_get");
    _m_glslang_program_SPIRV_get_ptr               = (PFN_GLSLANG_PROGRAM_SPIRV_GET_PTR) glslang_get_proc("glslang_program_SPIRV_get_ptr");
    _m_glslang_program_SPIRV_get_messages          = (PFN_GLSLANG_PROGRAM_SPIRV_GET_MESSAGES) glslang_get_proc("glslang_program_SPIRV_get_messages");
    _m_glslang_program_get_info_log                = (PFN_GLSLANG_PROGRAM_GET_INFO_LOG) glslang_get_proc("glslang_program_get_info_log");
    _m_glslang_program_get_info_debug_log          = (PFN_GLSLANG_PROGRAM_GET_INFO_DEBUG_LOG) glslang_get_proc("glslang_program_get_info_debug_log");


    #ifdef RESOURCE_LIMITS_C
    // resource_limits_c.h
    _m_glslang_resource                = (PFN_GLSLANG_RESOURCE) glslang_get_proc("glslang_resource");
    _m_glslang_default_resource        = (PFN_GLSLANG_DEFAULT_RESOURCE) glslang_get_proc("glslang_default_resource");
    _m_glslang_default_resource_string = (PFN_GLSLANG_DEFAULT_RESOURCE_STRING) glslang_get_proc("glslang_default_resource_string");
    _m_glslang_decode_resource_limits  = (PFN_GLSLANG_DECODE_RESOURCE_LIMITS) glslang_get_proc("glslang_decode_resource_limits");
    #endif

    // check errors
    if (!_m_glslang_initialize_process || !_m_glslang_finalize_process ||
        !_m_glslang_shader_create || !_m_glslang_shader_delete ||
        !_m_glslang_shader_parse || !_m_glslang_program_create ||
        !_m_glslang_program_link
        #ifdef RESOURCE_LIMITS_C
        || !_m_glslang_default_resource
        #endif
    ) {
        fprintf(stderr, "failed to load glslang functions\n");
        return -1;
    }

    return 0;
}

//
// init
//

int glslang_initialize_process(void)
{
    return _m_glslang_initialize_process();
}

void glslang_finalize_process(void)
{
    _m_glslang_finalize_process();
}

//
// shader
//

glslang_shader_t* glslang_shader_create(const glslang_input_t* input)
{
    return _m_glslang_shader_create(input);
}

void glslang_shader_delete(glslang_shader_t* shader)
{
    _m_glslang_shader_delete(shader);
}

void glslang_shader_set_preamble(glslang_shader_t* shader, const char* s)
{
    _m_glslang_shader_set_preamble(shader, s);
}

void glslang_shader_set_entry_point(glslang_shader_t* shader, const char* s)
{
    return _m_glslang_shader_set_entry_point(shader, s);
}

void glslang_shader_shift_binding(glslang_shader_t* shader, glslang_resource_type_t res, unsigned int base)
{
    _m_glslang_shader_shift_binding(shader, res, base);
}

void glslang_shader_shift_binding_for_set(glslang_shader_t* shader, glslang_resource_type_t res, unsigned int base, unsigned int set)
{
    _m_glslang_shader_shift_binding_for_set(shader, res, base, set);
}

void glslang_shader_set_options(glslang_shader_t* shader, int options)
{
    _m_glslang_shader_set_options(shader, options);
}

void glslang_shader_set_glsl_version(glslang_shader_t* shader, int version)
{
    _m_glslang_shader_set_glsl_version(shader, version);
}

int glslang_shader_preprocess(glslang_shader_t* shader, const glslang_input_t* input)
{
    return _m_glslang_shader_preprocess(shader, input);
}

int glslang_shader_parse(glslang_shader_t* shader, const glslang_input_t* input)
{
    return _m_glslang_shader_parse(shader, input);
}

const char* glslang_shader_get_preprocessed_code(glslang_shader_t* shader)
{
    return _m_glslang_shader_get_preprocessed_code(shader);
}

const char* glslang_shader_get_info_log(glslang_shader_t* shader)
{
    return _m_glslang_shader_get_info_log(shader);
}

const char* glslang_shader_get_info_debug_log(glslang_shader_t* shader)
{
    return _m_glslang_shader_get_info_debug_log(shader);
}

//
// program
//

glslang_program_t* glslang_program_create(void)
{
    return _m_glslang_program_create();
}

void glslang_program_delete(glslang_program_t* program)
{
    _m_glslang_program_delete(program);
}

void glslang_program_add_shader(glslang_program_t* program, glslang_shader_t* shader)
{
    _m_glslang_program_add_shader(program, shader);
}

int glslang_program_link(glslang_program_t* program, int messages)
{
    return _m_glslang_program_link(program, messages);
}

void glslang_program_add_source_text(glslang_program_t* program, glslang_stage_t stage, const char* text, size_t len)
{
    _m_glslang_program_add_source_text(program, stage, text, len);
}

void glslang_program_set_source_file(glslang_program_t* program, glslang_stage_t stage, const char* file)
{
    _m_glslang_program_set_source_file(program, stage, file);
}

int glslang_program_map_io(glslang_program_t* program)
{
    return _m_glslang_program_map_io(program);
}

void glslang_program_SPIRV_generate(glslang_program_t* program, glslang_stage_t stage)
{
    _m_glslang_program_SPIRV_generate(program, stage);
}

void glslang_program_SPIRV_generate_with_options(glslang_program_t* program, glslang_stage_t stage, glslang_spv_options_t* spv_options)
{
    _m_glslang_program_SPIRV_generate_with_options(program, stage, spv_options);
}

size_t glslang_program_SPIRV_get_size(glslang_program_t* program)
{
    return _m_glslang_program_SPIRV_get_size(program);
}

void glslang_program_SPIRV_get(glslang_program_t* program, unsigned int* spirv)
{
    _m_glslang_program_SPIRV_get(program, spirv);
}

unsigned int* glslang_program_SPIRV_get_ptr(glslang_program_t* program)
{
    return _m_glslang_program_SPIRV_get_ptr(program);
}

const char* glslang_program_SPIRV_get_messages(glslang_program_t* program)
{
    return _m_glslang_program_SPIRV_get_messages(program);
}

const char* glslang_program_get_info_log(glslang_program_t* program)
{
    return _m_glslang_program_get_info_log(program);
}

const char* glslang_program_get_info_debug_log(glslang_program_t* program)
{
    return _m_glslang_program_get_info_debug_log(program);
}

#ifdef RESOURCE_LIMITS_C

glslang_resource_t* glslang_resource(void)
{
    return _m_glslang_resource();
}

const glslang_resource_t* glslang_default_resource(void)
{
    return _m_glslang_default_resource();
}

const char* glslang_default_resource_string()
{
    return _m_glslang_default_resource_string();
}

void glslang_decode_resource_limits(glslang_resource_t* resources, char* config)
{
    _m_glslang_decode_resource_limits(resources, config);
}

#endif

#ifdef __cplusplus
} // extern "C"
#endif
