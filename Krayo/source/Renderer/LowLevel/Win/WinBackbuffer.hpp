#pragma once

struct IUnknown;
#include <Windows.h>


namespace Krayo {
namespace Renderer {

struct BackbufferWindowDesc
{
    HINSTANCE hInstance;
    HWND hWnd;

    BackbufferWindowDesc()
        : hInstance(0)
        , hWnd(0)
    {
    }
};

} // namespace Krayo
} // namespace Renderer
