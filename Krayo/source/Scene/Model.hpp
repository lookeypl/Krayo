#pragma once

#include "Scene/Object.hpp"
#include "Scene/Component.hpp"
#include "Scene/Mesh.hpp"
#include "Scene/Material.hpp"

#include "Math/AABB.hpp"

#include <lkCommon/Math/Matrix4.hpp>
#include <lkCommon/Math/Vector4.hpp>

#include <functional>


namespace Krayo {
namespace Scene {

using MeshTraverseCallback = std::function<void(Mesh*)>;

struct ModelDesc
{
    FbxMesh* mesh;
    std::vector<Material*> materials;

    ModelDesc()
        : mesh(nullptr)
        , materials()
    {
    }
};

class Model final: public Component
{
    lkCommon::Math::Matrix4 mTransform;
    lkCommon::Math::Vector4 mPosition;
    lkCommon::Math::Vector4 mScale;
    std::vector<Mesh> mMeshes;
    Math::AABB mAABB;
    mutable bool mToRender;

    LKCOMMON_INLINE void UpdateTransform()
    {
        mTransform = lkCommon::Math::Matrix4::CreateTranslation(mPosition) * lkCommon::Math::Matrix4::CreateScale(mScale);
    }

public:
    Model(const std::string& name);
    ~Model();

    bool Init(const ModelDesc& modelDesc);
    void ForEachMesh(MeshTraverseCallback callback);

    LKCOMMON_INLINE void SetPosition(float x, float y, float z)
    {
        SetPosition(lkCommon::Math::Vector4(x, y, z, 1.0f));
    }

    LKCOMMON_INLINE void SetPosition(const lkCommon::Math::Vector4& position)
    {
        mPosition = position;
        UpdateTransform();
    }

    LKCOMMON_INLINE void SetScale(float scaleX, float scaleY, float scaleZ)
    {
        SetScale(lkCommon::Math::Vector4(scaleX, scaleY, scaleZ, 1.0f));
    }

    LKCOMMON_INLINE void SetScale(const lkCommon::Math::Vector4& scale)
    {
        mScale = scale;
        UpdateTransform();
    }

    LKCOMMON_INLINE void SetToRender(bool toRender) const
    {
        mToRender = toRender;
    }

    LKCOMMON_INLINE Mesh* GetMesh(size_t i)
    {
        return &mMeshes[i];
    }

    LKCOMMON_INLINE const lkCommon::Math::Vector4& GetPosition() const
    {
        return mPosition;
    }

    LKCOMMON_INLINE const lkCommon::Math::Matrix4& GetTransform() const
    {
        return mTransform;
    }

    LKCOMMON_INLINE bool ToRender() const
    {
        return mToRender;
    }

    LKCOMMON_INLINE const Math::AABB& GetAABB() const
    {
        return mAABB;
    }

    ComponentType GetType() const override
    {
        return ComponentType::Model;
    }
};

} // namespace Scene
} // namespace Krayo
