#include "Math/Frustum.hpp"

#include <lkCommon/Math/Constants.hpp>


namespace Krayo {
namespace Math {

Frustum::Frustum()
    : mScreenRatio(0.0f)
    , mHalfFovTan(0.0f)
    , mNearZ(0.0f)
    , mFarZ(0.0f)
{
}

Frustum::~Frustum()
{
}

void Frustum::Init(const FrustumDesc& desc)
{
    float angle = LKCOMMON_DEG_TO_RADF(desc.fov * 0.5f);
    mHalfFovTan = cosf(angle) / sinf(angle);
    mScreenRatio = desc.ratio;
    mNearZ = desc.nearZ;
    mFarZ = desc.farZ;
}

void Frustum::Refresh(const lkCommon::Math::Vector4& pos, const lkCommon::Math::Vector4& at, const lkCommon::Math::Vector4& up)
{
    // camera's axes
    lkCommon::Math::Vector4 cameraZ = at - pos;
    cameraZ.Normalize();
    lkCommon::Math::Vector4 cameraX = up.Cross(cameraZ);
    cameraX.Normalize();
    lkCommon::Math::Vector4 cameraY = cameraZ.Cross(cameraX);
    cameraY.Normalize();

    // near points
    lkCommon::Math::Vector4 nearMid = pos + (cameraZ * mNearZ);
    float nearHalfH = mNearZ * mHalfFovTan;
    float nearHalfW = nearHalfH / mScreenRatio;
    lkCommon::Math::Vector4 nearTopLeft = nearMid - (cameraX * nearHalfW) + (cameraY * nearHalfH);
    lkCommon::Math::Vector4 nearTopRight = nearMid + (cameraX * nearHalfW) + (cameraY * nearHalfH);
    lkCommon::Math::Vector4 nearBottomLeft = nearMid - (cameraX * nearHalfW) - (cameraY * nearHalfH);
    lkCommon::Math::Vector4 nearBottomRight = nearMid + (cameraX * nearHalfW) - (cameraY * nearHalfH);

    // far points
    lkCommon::Math::Vector4 farMid = pos + (cameraZ * mFarZ);
    float farHalfH = mFarZ * mHalfFovTan;
    float farHalfW = farHalfH / mScreenRatio;
    lkCommon::Math::Vector4 farTopLeft = farMid - (cameraX * farHalfW) + (cameraY * farHalfH);
    lkCommon::Math::Vector4 farTopRight = farMid + (cameraX * farHalfW) + (cameraY * farHalfH);
    lkCommon::Math::Vector4 farBottomLeft = farMid - (cameraX * farHalfW) - (cameraY * farHalfH);
    lkCommon::Math::Vector4 farBottomRight = farMid + (cameraX * farHalfW) - (cameraY * farHalfH);

    // calculated assuming LH coordinate system
    mPlanes[PlaneSide::FRONT] = Plane(nearBottomLeft, nearBottomRight, nearTopLeft);
    mPlanes[PlaneSide::LEFT] = Plane(nearBottomLeft, nearTopLeft, farBottomLeft);
    mPlanes[PlaneSide::TOP] = Plane(nearTopLeft, nearTopRight, farTopLeft);
    mPlanes[PlaneSide::RIGHT] = Plane(nearTopRight, nearBottomRight, farTopRight);
    mPlanes[PlaneSide::BOTTOM] = Plane(nearBottomRight, nearBottomLeft, farBottomRight);
    mPlanes[PlaneSide::BACK] = Plane(farTopLeft, farTopRight, farBottomLeft);
}

bool Frustum::Intersects(const AABB& aabb) const
{
    // check for every plane
    for (uint32_t i = 0; i < PlaneSide::COUNT; ++i)
    {
        // acquire p-vertex's index from Plane's normal
        uint32_t px = static_cast<uint32_t>(mPlanes[i].GetPlaneVec()[0] > 0.0f);
        uint32_t py = static_cast<uint32_t>(mPlanes[i].GetPlaneVec()[1] > 0.0f);
        uint32_t pz = static_cast<uint32_t>(mPlanes[i].GetPlaneVec()[2] > 0.0f);

        // calculate dot product of Plane's normal and our p-vertex
        // Zeroing w coordinate in pvert will still yield proper 3D dot product, despite plane normal's distance
        lkCommon::Math::Vector4 pvert(aabb.mVert[px][0], aabb.mVert[py][1], aabb.mVert[pz][2], 0.0f);
        float pdot = mPlanes[i].GetPlaneVec().Dot(pvert);

        if (pdot < mPlanes[i].GetPlaneVec()[3])
        {
            return false;
        }
    }

    return true;
}

} // namespace Math
} // namespace Krayo
