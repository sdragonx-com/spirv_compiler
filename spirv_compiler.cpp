#include "spirv_compiler.h"

const char* stageNames[] = {
    "顶点", "曲面细分控制", "曲面细分求值", "几何",
    "片段", "计算", "光线生成", "相交", "任意命中",
    "最近命中", "未命中", "可调用", "网格", "任务"
};

// 根据文件扩展名确定色器类型
glslang_stage_t shader_stage_from_extension(const std::string& filename)
{
    size_t pos = filename.find_last_of('.');
    if (pos == std::string::npos) {
        return GLSLANG_STAGE_COUNT;
    }

    std::string ext = filename.substr(pos + 1);

    if (ext == "vert")
        return GLSLANG_STAGE_VERTEX;
    if (ext == "frag")
        return GLSLANG_STAGE_FRAGMENT;
    if (ext == "geom")
        return GLSLANG_STAGE_GEOMETRY;
    if (ext == "tesc")
        return GLSLANG_STAGE_TESSCONTROL;
    if (ext == "tese")
        return GLSLANG_STAGE_TESSEVALUATION;
    if (ext == "comp")
        return GLSLANG_STAGE_COMPUTE;
    if (ext == "mesh")
        return GLSLANG_STAGE_MESH;
    if (ext == "task")
        return GLSLANG_STAGE_TASK;
    if (ext == "rgen")
        return GLSLANG_STAGE_RAYGEN;
    if (ext == "rint")
        return GLSLANG_STAGE_INTERSECT;
    if (ext == "rahit")
        return GLSLANG_STAGE_ANYHIT;
    if (ext == "rchit")
        return GLSLANG_STAGE_CLOSESTHIT;
    if (ext == "rmiss")
        return GLSLANG_STAGE_MISS;
    if (ext == "rcall")
        return GLSLANG_STAGE_CALLABLE;

    return GLSLANG_STAGE_COUNT;
}

// 根据着色器类型字符串获取类型枚举
glslang_stage_t shader_stage_from_string(const std::string& shaderType)
{
    if (shaderType == "vertex")
        return GLSLANG_STAGE_VERTEX;
    if (shaderType == "fragment")
        return GLSLANG_STAGE_FRAGMENT;
    if (shaderType == "geometry")
        return GLSLANG_STAGE_GEOMETRY;
    if (shaderType == "tesscontrol")
        return GLSLANG_STAGE_TESSCONTROL;
    if (shaderType == "tessevaluation")
        return GLSLANG_STAGE_TESSEVALUATION;
    if (shaderType == "compute")
        return GLSLANG_STAGE_COMPUTE;
    if (shaderType == "rgen")
        return GLSLANG_STAGE_RAYGEN;
    if (shaderType == "rint")
        return GLSLANG_STAGE_INTERSECT;
    if (shaderType == "rahit")
        return GLSLANG_STAGE_ANYHIT;
    if (shaderType == "rchit")
        return GLSLANG_STAGE_CLOSESTHIT;
    if (shaderType == "rmiss")
        return GLSLANG_STAGE_MISS;
    if (shaderType == "rcallable")
        return GLSLANG_STAGE_CALLABLE;
    if (shaderType == "task")
        return GLSLANG_STAGE_TASK;
    if (shaderType == "mesh")
        return GLSLANG_STAGE_MESH;

    return GLSLANG_STAGE_COUNT;
}

// 使用十六进制值直接转换
glslang_stage_t shader_stage_from_gl(int shaderType)
{
    switch (shaderType) {
    case 0x8B30: return GLSLANG_STAGE_VERTEX;         // GL_VERTEX_SHADER
    case 0x8B31: return GLSLANG_STAGE_FRAGMENT;       // GL_FRAGMENT_SHADER
    case 0x8DD9: return GLSLANG_STAGE_GEOMETRY;       // GL_GEOMETRY_SHADER
    case 0x8E87: return GLSLANG_STAGE_TESSCONTROL;    // GL_TESS_CONTROL_SHADER
    case 0x8E88: return GLSLANG_STAGE_TESSEVALUATION; // GL_TESS_EVALUATION_SHADER
    case 0x91B9: return GLSLANG_STAGE_COMPUTE;        // GL_COMPUTE_SHADER
    default: return GLSLANG_STAGE_COUNT;              // 错误指示
    }
}

// 方法4：反向转换（EShLanguage 转 GLenum）
int shader_stage_to_gl(glslang_stage_t shaderLang)
{
    switch (shaderLang) {
    case GLSLANG_STAGE_VERTEX: return 0x8B30;         // GL_VERTEX_SHADER
    case GLSLANG_STAGE_FRAGMENT: return 0x8B31;       // GL_FRAGMENT_SHADER
    case GLSLANG_STAGE_GEOMETRY: return 0x8DD9;       // GL_GEOMETRY_SHADER
    case GLSLANG_STAGE_TESSCONTROL: return 0x8E87;    // GL_TESS_CONTROL_SHADER
    case GLSLANG_STAGE_TESSEVALUATION: return 0x8E88; // GL_TESS_EVALUATION_SHADER
    case GLSLANG_STAGE_COMPUTE: return 0x91B9;        // GL_COMPUTE_SHADER
    default: return 0;                                // 无效值
    }
}
