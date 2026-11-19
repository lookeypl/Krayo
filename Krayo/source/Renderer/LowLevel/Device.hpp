#pragma once

#include "Prerequisites.hpp"
#include "Renderer/LowLevel/Instance.hpp"
#include "Renderer/LowLevel/CommandBuffer.hpp"
#include "Renderer/LowLevel/QueueManager.hpp"

#include <lkCommon/lkCommon.hpp>


namespace Krayo {
namespace Renderer {

class Device
{
    friend class Backbuffer;
    friend class Pipeline;

    InstancePtr mInstance;
    VkDevice mDevice;
    VkPhysicalDevice mPhysicalDevice;
    VkPhysicalDeviceMemoryProperties mMemoryProperties;
    QueueManager mQueueManager;

    VkPhysicalDevice SelectPhysicalDevice();

public:
    Device();
    ~Device();

    bool Init(const InstancePtr& inst, bool noAsync);

    uint32_t GetMemoryTypeIndex(uint32_t typeBits, VkFlags properties) const;

    void Wait(DeviceQueueType queueType) const;
    bool Execute(DeviceQueueType queueType, CommandBuffer* cmd) const;
    bool Execute(DeviceQueueType queueType, CommandBuffer* cmd, uint32_t waitSemaphoresCount,
                 const VkPipelineStageFlags* waitFlags, const VkSemaphore* waitSemaphores,
                 VkSemaphore signalSemaphore, VkFence waitFence) const;

    LKCOMMON_INLINE VkDevice GetDevice() const
    {
        return mDevice;
    }

    LKCOMMON_INLINE VkCommandPool GetCommandPool(DeviceQueueType queueType) const
    {
        return mQueueManager.GetCommandPool(queueType);
    }

    LKCOMMON_INLINE uint32_t GetQueueIndex(DeviceQueueType queueType) const
    {
        return mQueueManager.GetQueueIndex(queueType);
    }

    LKCOMMON_INLINE uint32_t GetQueueCount() const
    {
        return mQueueManager.GetQueueCount();
    }

    LKCOMMON_INLINE const uint32_t* GetQueueIndices() const
    {
        return mQueueManager.GetQueueIndices();
    }
};

using DevicePtr = std::shared_ptr<Device>;

} // namespace Renderer
} // namespace Krayo
