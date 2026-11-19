#include "Math/AABB.hpp"


namespace Krayo {
namespace Math {

AABB::AABB()
{
}

AABB::AABB(const lkCommon::Math::Vector4& min, const lkCommon::Math::Vector4& max)
    : mVert{min, max}
{
}

void AABB::Realign()
{
    // select which vertices are higher
    uint32_t px = static_cast<uint32_t>(mVert[AABBVert::MAX][0] > mVert[AABBVert::MIN][0]);
    uint32_t py = static_cast<uint32_t>(mVert[AABBVert::MAX][1] > mVert[AABBVert::MIN][1]);
    uint32_t pz = static_cast<uint32_t>(mVert[AABBVert::MAX][2] > mVert[AABBVert::MIN][2]);
    uint32_t pw = static_cast<uint32_t>(mVert[AABBVert::MAX][3] > mVert[AABBVert::MIN][3]);

    uint32_t nx = static_cast<uint32_t>(!px);
    uint32_t ny = static_cast<uint32_t>(!py);
    uint32_t nz = static_cast<uint32_t>(!pz);
    uint32_t nw = static_cast<uint32_t>(!pw);

    lkCommon::Math::Vector4 newMin(mVert[nx][0], mVert[ny][1], mVert[nz][2], mVert[nw][3]);
    lkCommon::Math::Vector4 newMax(mVert[px][0], mVert[py][1], mVert[pz][2], mVert[pw][3]);

    mVert[AABBVert::MIN] = newMin;
    mVert[AABBVert::MAX] = newMax;
}

AABB operator*(const lkCommon::Math::Matrix4& a, const AABB& b)
{
    AABB result(a * b[AABB::AABBVert::MIN], a * b[AABB::AABBVert::MAX]);
    result.Realign();
    return result;
}

std::ostream& operator<<(std::ostream& o, const AABB& aabb)
{
    o << "min: " << aabb.mVert[aabb.AABBVert::MIN] << ", max: " << aabb.mVert[aabb.AABBVert::MAX];
    return o;
}

} // namespace Math
} // namespace Krayo
