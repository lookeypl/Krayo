#pragma once

namespace Krayo {
namespace Renderer {

enum DeviceQueueType
{
    GRAPHICS = 0,
    TRANSFER,
    COMPUTE,
    COUNT // used only as a counter
};

enum ClearType
{
    NONE = 0,
    COLOR,
    DEPTH,
    ALL = COLOR | DEPTH,
};

} // namespace Renderer
} // namespace Krayo
