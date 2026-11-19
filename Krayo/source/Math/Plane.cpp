#include "Math/Plane.hpp"


namespace Krayo {
namespace Math {

Plane::Plane()
    : mPlaneVec()
{
}

Plane::Plane(const lkCommon::Math::Vector4& p0, const lkCommon::Math::Vector4& p1, const lkCommon::Math::Vector4& p2)
{
    lkCommon::Math::Vector4 v0 = p1 - p0;
    lkCommon::Math::Vector4 v2 = p2 - p0;

    mPlaneVec = v0.Cross(v2);
    mPlaneVec[3] = 0.0f;
    mPlaneVec.Normalize();
    mPlaneVec[3] = mPlaneVec.Dot(p0);
}

std::ostream& operator<<(std::ostream& o, const Plane& plane)
{
    o << "[" << plane.mPlaneVec[0] << ", " << plane.mPlaneVec[1] << ", " << plane.mPlaneVec[2] << "], " << plane.mPlaneVec[3];
    return o;
}

} // namespace Math
} // namespace Krayo
