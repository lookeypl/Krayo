#pragma once

#include <lkCommon/Math/Vector4.hpp>


namespace Krayo {
namespace Math {

class Plane
{
    // [0..2] is normal, [3] is distance
    lkCommon::Math::Vector4 mPlaneVec;

public:
    Plane();
    Plane(const lkCommon::Math::Vector4& p0, const lkCommon::Math::Vector4& p1, const lkCommon::Math::Vector4& p2);

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetPlaneVec() const
    {
        return mPlaneVec;
    }

    friend std::ostream& operator<<(std::ostream& o, const Plane& plane);
};

std::ostream& operator<<(std::ostream& o, const Plane& plane);

} // namespace Math
} // namespace Krayo
