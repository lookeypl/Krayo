#pragma once

#include "Renderer/LowLevel/Buffer.hpp"
#include "Renderer/LowLevel/MultiPipeline.hpp"
#include "Renderer/LowLevel/DescriptorAllocator.hpp"
#include "Renderer/LowLevel/CommandBuffer.hpp"
#include "Renderer/LowLevel/Tools.hpp"
#include "Renderer/LowLevel/Device.hpp"

#include <lkCommon/Math/Matrix4.hpp>


namespace Krayo {
namespace Renderer {

struct GridFrustumsGenerationDesc
{
    lkCommon::Math::Matrix4 projMat;
    uint32_t viewportWidth;
    uint32_t viewportHeight;
    uint32_t pixelsPerGridFrustum;

    GridFrustumsGenerationDesc()
        : projMat()
        , viewportWidth(0)
        , viewportHeight(0)
        , pixelsPerGridFrustum(0)
    {
    }
};

class GridFrustumsGenerator
{
    DevicePtr mDevice;

    Buffer mGridFrustumsInfo;
    Buffer mGridFrustumsData;
    VkDescriptorSet mGridFrustumsDataSet;
    VkRAII<VkDescriptorSetLayout> mGridFrustumsDataSetLayout;
    VkRAII<VkPipelineLayout> mPipelineLayout;
    MultiPipeline mPipeline;
    CommandBuffer mDispatchCommandBuffer;

public:
    GridFrustumsGenerator();

    bool Init(const DevicePtr& device);
    bool Generate(const GridFrustumsGenerationDesc& desc);

    LKCOMMON_INLINE Buffer* GetGridFrustums()
    {
        return &mGridFrustumsData;
    }
};

} // namespace Renderer
} // namespace Krayo
