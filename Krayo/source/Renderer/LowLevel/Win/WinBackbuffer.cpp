#include "Renderer/LowLevel/Win/WinBackbuffer.hpp"

#include "Renderer/LowLevel/Backbuffer.hpp"
#include "Renderer/LowLevel/Util.hpp"
#include "Renderer/LowLevel/Extensions.hpp"

#include <lkCommon/lkCommon.hpp>


namespace Krayo {
namespace Renderer {

bool Backbuffer::CreateSurface(const BackbufferDesc& desc)
{
    VkWin32SurfaceCreateInfoKHR surfInfo;
    LKCOMMON_ZERO_MEMORY(surfInfo);
    surfInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfInfo.hinstance = desc.windowDesc.hInstance;
    surfInfo.hwnd = desc.windowDesc.hWnd;
    VkResult result = vkCreateWin32SurfaceKHR(mInstance->GetVkInstance(), &surfInfo, nullptr, &mSurface);
    RETURN_FALSE_IF_FAILED(result, "Failed to create Vulkan Surface for Win32");

    return true;
}

} // namespace Krayo
} // namespace Renderer