#pragma once

#include <lkCommon/lkCommon.hpp>


namespace Krayo {
namespace Scene {

enum class ComponentType: unsigned char
{
    Invalid = 0,
    Model,
    Light,
    Emitter,
};

class Component
{
protected:
    std::string mName;

public:
    Component(const std::string& name);

    virtual ComponentType GetType() const = 0;

    LKCOMMON_INLINE std::string GetName() const
    {
        return mName;
    }
};

} // namespace Scene
} // namespace Krayo
