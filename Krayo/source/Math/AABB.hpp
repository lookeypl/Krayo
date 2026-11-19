#pragma once

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/Math/Matrix4.hpp>


namespace Krayo {
namespace Math {

class AABB
{
    friend class Frustum;

public:
    enum AABBVert
    {
        MIN = 0,
        MAX,
        COUNT
    };

private:
    lkCommon::Math::Vector4 mVert[AABBVert::COUNT];

public:
    AABB();
    AABB(const lkCommon::Math::Vector4& min, const lkCommon::Math::Vector4& max);

    void Realign();

    LKCOMMON_INLINE const lkCommon::Math::Vector4& operator[](AABBVert v) const
    {
        return mVert[v];
    }

    friend std::ostream& operator<<(std::ostream& o, const AABB& aabb);
};

std::ostream& operator<<(std::ostream& o, const AABB& aabb);
AABB operator*(const lkCommon::Math::Matrix4& a, const AABB& b);

} // namespace Math
} // namespace Krayo
