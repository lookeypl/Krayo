#pragma once

#include "Component.hpp"

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Utils/Pixel.hpp>


namespace Krayo {
namespace Scene {

// workaround to allow easy storage of Light data
// this way we will be able to easily pass all the params to shaders
LKCOMMON_ALIGN(16)
struct LightData
{
    lkCommon::Math::Vector4 position;
    lkCommon::Utils::Pixel<float, 3> diffuseIntensity;
    float range;

    LightData()
        : position(0.0f, 0.0f, 0.0f, 1.0f)
        , diffuseIntensity(1.0f)
        , range(0.0f)
    {
    }
};

class Light: public Component
{
    friend class Scene;

    LightData mLightData;

public:
    Light(const std::string& name);
    ~Light();

    LKCOMMON_INLINE void SetPosition(const float x, const float y, const float z)
    {
        SetPosition(lkCommon::Math::Vector4(x, y, z, 1.0f));
    }

    LKCOMMON_INLINE void SetPosition(const lkCommon::Math::Vector4& position)
    {
        mLightData.position = position;
    }

    LKCOMMON_INLINE void SetDiffuseIntensity(const float R, const float G, const float B)
    {
        mLightData.diffuseIntensity[0] = R;
        mLightData.diffuseIntensity[1] = G;
        mLightData.diffuseIntensity[2] = B;
    }

    LKCOMMON_INLINE void SetRange(const float range)
    {
        mLightData.range = range;
    }

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetPosition() const
    {
        return mLightData.position;
    }

    LKCOMMON_INLINE const lkCommon::Utils::Pixel<float, 3>& GetDiffuseIntensity() const
    {
        return mLightData.diffuseIntensity;
    }

    LKCOMMON_INLINE const float& GetRange() const
    {
        return mLightData.range;
    }

    LKCOMMON_INLINE const LightData* GetData() const
    {
        return &mLightData;
    }

    LKCOMMON_INLINE ComponentType GetType() const override
    {
        return ComponentType::Light;
    }
};

} // namespace Scene
} // namespace Krayo
