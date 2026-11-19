#pragma once

#include <lkCommon/lkCommon.hpp>
#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Math/Matrix4.hpp>

#include "Math/Frustum.hpp"


namespace Krayo {
namespace Scene {

enum class CameraMode: unsigned char
{
    FREEROAM = 0,
    TRACED
};

struct CameraDesc
{
    lkCommon::Math::Vector4 pos;
    lkCommon::Math::Vector4 at;
    lkCommon::Math::Vector4 up;
};

class Camera final
{
    lkCommon::Math::Vector4 mPosition;
    lkCommon::Math::Vector4 mAtPosition;
    lkCommon::Math::Vector4 mUpVector;
    lkCommon::Math::Matrix4 mView;

public:
    void Update(const CameraDesc& desc);

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetPosition() const
    {
        return mPosition;
    }

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetAtPosition() const
    {
        return mAtPosition;
    }

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetUpVector() const
    {
        return mUpVector;
    }

    LKCOMMON_INLINE const lkCommon::Math::Matrix4& GetView() const
    {
        return mView;
    }
};

} // namespace Scene
} // namespace Krayo
