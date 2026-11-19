#pragma once

#include <lkCommon/Utils/Pixel.hpp>

#include "Renderer/HighLevel/ResourceManager.hpp"


namespace Krayo {
namespace Scene {

struct MaterialDesc
{
    std::string materialName;

    lkCommon::Utils::PixelFloat4 color;
    std::string diffusePath;
    std::string normalPath;
    std::string maskPath;

    MaterialDesc()
        : materialName()
        , color(1.0f)
        , diffusePath()
        , normalPath()
        , maskPath()
    {
    }
};

class Material
{
    friend class Mesh;

    std::string mMaterialName;

    lkCommon::Utils::PixelFloat4 mColor;
    Renderer::TexturePtr mDiffuseTexture;
    Renderer::TexturePtr mNormalTexture;
    Renderer::TexturePtr mMaskTexture;

    bool CreateRendererTexture(const std::string& image, VkImageUsageFlags usage, Renderer::TexturePtr& texture);

public:
    Material(const std::string& name);
    ~Material();

    bool Init(const MaterialDesc& desc);

    LKCOMMON_INLINE const lkCommon::Utils::PixelFloat4& GetColor() const
    {
        return mColor;
    }

    LKCOMMON_INLINE const Renderer::TexturePtr& GetDiffuse() const
    {
        return mDiffuseTexture;
    }

    LKCOMMON_INLINE const Renderer::TexturePtr& GetNormal() const
    {
        return mNormalTexture;
    }

    LKCOMMON_INLINE const Renderer::TexturePtr& GetMask() const
    {
        return mMaskTexture;
    }
};

} // namespace Scene
} // namespace Krayo
