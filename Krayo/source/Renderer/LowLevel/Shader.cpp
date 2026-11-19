#include "Renderer/LowLevel/Shader.hpp"

#include "ResourceDir.hpp"
#include "Renderer/LowLevel/Util.hpp"
#include "Renderer/LowLevel/Extensions.hpp"
#include "Renderer/HighLevel/Renderer.hpp"

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/System/FS.hpp>

#include <fstream>

#include <shaderc/shaderc.hpp>


namespace Krayo {
namespace Renderer {

namespace {

const std::string SHADER_HEADER_START = "#version 450\n\
#extension GL_ARB_separate_shader_objects: enable\n";
const std::string DEFINE_STR = "#define ";
const std::string SHADER_HEADER_TAIL = "\0";

shaderc_shader_kind GetShaderKind(ShaderType type)
{
    switch (type)
    {
    case ShaderType::VERTEX:        return shaderc_vertex_shader;
    case ShaderType::TESS_CONTROL:  return shaderc_tess_control_shader;
    case ShaderType::TESS_EVAL:     return shaderc_tess_evaluation_shader;
    case ShaderType::GEOMETRY:      return shaderc_geometry_shader;
    case ShaderType::FRAGMENT:      return shaderc_fragment_shader;
    case ShaderType::COMPUTE:       return shaderc_compute_shader;
    // default returns whatever, we won't get here anyway
    default:                        return shaderc_glsl_infer_from_source;
    }
}

std::string TranslateShaderCompilationStatusToString(shaderc_compilation_status status)
{
    switch (status)
    {
    case shaderc_compilation_status_success: return "Success";
    case shaderc_compilation_status_invalid_stage: return "Invalid stage";
    case shaderc_compilation_status_compilation_error: return "Compilation error";
    case shaderc_compilation_status_internal_error: return "Internal error";
    case shaderc_compilation_status_null_result_object: return "Null result object";
    case shaderc_compilation_status_invalid_assembly: return "Invalid assembly";
    case shaderc_compilation_status_validation_error: return "Validation error";
    default: return"Unknown";
    }
}

} // namespace



Shader::Shader()
    : mShaderModule(VK_NULL_HANDLE)
{
}

Shader::~Shader()
{
    if (mShaderModule != VK_NULL_HANDLE)
        vkDestroyShaderModule(mDevice->GetDevice(), mShaderModule, nullptr);
}

bool Shader::CompileToFile(ShaderType type, const std::string& shaderFile, const std::string& spvShaderFile,
                           const ShaderMacros& macros, std::vector<uint32_t>& code)
{
    // read shader file
    std::ifstream glslSourceFile(shaderFile, std::ifstream::in);
    if (!glslSourceFile.good())
    {
        LOGE("Failed to open GLSL source file " << shaderFile);
        return false;
    }

    std::string glslSource((std::istreambuf_iterator<char>(glslSourceFile)),
                            std::istreambuf_iterator<char>());

    // assemble full source string with macros
    std::string fullSource = SHADER_HEADER_START;
    for (auto& macro: macros)
    {
        fullSource += DEFINE_STR + macro.name + " " + std::to_string(macro.value) + "\n";
    }
    fullSource += SHADER_HEADER_TAIL + glslSource;

    // compile source
    shaderc::Compiler compiler;
    if (!compiler.IsValid())
    {
        LOGE("Failed to initialize shaderc compiler.");
        return false;
    }

    shaderc_shader_kind kind = GetShaderKind(type);
    if (kind == shaderc_glsl_infer_from_source)
    {
        LOGE("Unrecognized shader type");
        return false;
    }

    shaderc::SpvCompilationResult shader = compiler.CompileGlslToSpv(fullSource, kind, shaderFile.c_str());
    if (shader.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        LOGE("Error while compiling shader " << shaderFile << ": " <<
             TranslateShaderCompilationStatusToString(shader.GetCompilationStatus()));
        if (shader.GetNumErrors() != 0)
        {
            LOGE("Shader error log:\n" << shader.GetErrorMessage());
        }

        return false;
    }

    if (shader.GetNumWarnings() != 0)
    {
        LOGW("Warnings while compiling shader " << shaderFile << ":\n" << shader.GetErrorMessage());
    }

    code = std::vector<uint32_t>{shader.cbegin(), shader.cend()};

    // save to file
    std::ofstream spvFile(spvShaderFile, std::ofstream::out | std::ofstream::binary);
    if (!spvFile)
    {
        LOGE("Unable to open SPV shader file " << shaderFile << ".spv for writing");
        return false;
    }

    spvFile.write(reinterpret_cast<const char*>(code.data()), code.size() * sizeof(uint32_t));

    return true;
}

bool Shader::CreateVkShaderModule(const std::vector<uint32_t>& code)
{
    VkShaderModuleCreateInfo shaderInfo;
    LKCOMMON_ZERO_MEMORY(shaderInfo);
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.pCode = code.data();
    shaderInfo.codeSize = code.size() * sizeof(uint32_t);
    VkResult result = vkCreateShaderModule(mDevice->GetDevice(), &shaderInfo, nullptr, &mShaderModule);
    RETURN_FALSE_IF_FAILED(result, "Failed to create shader module");

    return true;
}

bool Shader::Init(const DevicePtr& device, const ShaderDesc& desc)
{
    mDevice = device;

    std::vector<uint32_t> code;

    std::string shaderPath(lkCommon::System::FS::JoinPaths(ResourceDir::SHADERS, desc.filename));
    std::string spvShaderPath(lkCommon::System::FS::JoinPaths(ResourceDir::SHADER_CACHE, desc.filename));
    if (!desc.macros.empty())
    {
        spvShaderPath += ".";
        for (auto& m: desc.macros)
        {
            std::stringstream macroSS;
            macroSS << std::hex << m.value;
            spvShaderPath += macroSS.str();
        }
    }
    spvShaderPath += ".spv";

    if (!lkCommon::System::FS::Exists(spvShaderPath))
    {
        LOGW("Generating SPV version of shader file " << desc.filename);

        if (!CompileToFile(desc.type, shaderPath, spvShaderPath, desc.macros, code))
        {
            LOGE("Failed to compile shader to SPIRV");
            return false;
        }
    }
    else if (lkCommon::System::FS::GetFileModificationTime(shaderPath) > lkCommon::System::FS::GetFileModificationTime(spvShaderPath))
    {
        LOGW("SPV version of shader " << desc.filename << " out of date - refreshing");

        if (!lkCommon::System::FS::RemoveFile(spvShaderPath))
        {
            LOGE("Failed to remove existing shader code");
            return false;
        }

        if (!CompileToFile(desc.type, shaderPath, spvShaderPath, desc.macros, code))
        {
            LOGE("Failed to compile shader to SPIRV");
            return false;
        }
    }
    else
    {
        std::ifstream codeFile(spvShaderPath, std::ifstream::in | std::ifstream::binary);
        if (!codeFile.good())
        {
            LOGE("Failed to open Shader code file " << desc.filename);
            return false;
        }

        codeFile.seekg(0, std::ifstream::end);
        size_t codeSize = static_cast<size_t>(codeFile.tellg());
        codeFile.seekg(0, std::ifstream::beg);

        code.resize(codeSize / sizeof(uint32_t));
        codeFile.read(reinterpret_cast<char*>(code.data()), codeSize);
        if (!codeFile.good())
        {
            LOGE("Failure while reading the file");
            LOGE("bits: " << codeFile.eof() << codeFile.fail() << codeFile.bad());
        }
    }

    if (!CreateVkShaderModule(code))
    {
        LOGE("Failed to create Vulkan Shader Module for shader " << desc.filename);
        return false;
    }

    LOGI("Successfully created shader from file " << desc.filename);
    return true;
}

} // namespace Renderer
} // namespace Krayo
