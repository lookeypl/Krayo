#pragma once

#include "Prerequisites.hpp"

#include <vulkan/vulkan.h>

#include <lkCommon/System/Library.hpp>


namespace Krayo {
namespace Renderer {

// An object representing Vulkan's VkInstance
class Instance
{
    friend class Device;

    VkInstance mInstance;
    lkCommon::System::Library mVulkanLibrary;
    bool mDebuggingEnabled;

public:
    Instance();
    ~Instance();

    bool Init(VkDebugReportFlagsEXT debugFlags = 0);
    const VkInstance& GetVkInstance() const;
    bool IsDebuggingEnabled() const;
};

using InstancePtr = std::shared_ptr<Instance>;

} // namespace Renderer
} // namespace Krayo