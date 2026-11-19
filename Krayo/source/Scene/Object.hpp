#pragma once

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/Math/Matrix4.hpp>

#include "Scene/Component.hpp"


namespace Krayo {
namespace Scene {

class Object final
{
    Component* mComponent;

public:
    Object();
    ~Object();

    LKCOMMON_INLINE void SetComponent(Component* component)
    {
        mComponent = component;
    }

    LKCOMMON_INLINE Component* GetComponent() const
    {
        return mComponent;
    }
};

} // namespace Scene
} // namespace Krayo
