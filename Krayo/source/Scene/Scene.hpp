#pragma once

#include "Prerequisites.hpp"
#include "Utils/FBXFile.hpp"
#include "Scene/Object.hpp"
#include "Scene/Model.hpp"
#include "Scene/Light.hpp"
#include "Scene/Emitter.hpp"
#include "Scene/Material.hpp"

#include <lkCommon/Utils/Pixel.hpp>

#include <unordered_map>


namespace Krayo {
namespace Scene {

template <typename T>
using Callback = std::function<bool(const T*)>; // return false to stop iterating

template <typename T>
using GetResult = std::pair<T*, bool>; // .second is true when new material was created

class Scene
{
    template <typename T>
    using ResourceMap = std::unordered_map<std::string, std::unique_ptr<T>>;

    Krayo::Utils::FBXFile mFBXFile;
    std::vector<Object> mObjects;
    ResourceMap<Model> mModelComponents;
    ResourceMap<Light> mLightComponents;
    ResourceMap<Emitter> mEmitterComponents;
    ResourceMap<Material> mMaterials;

    // getters per each component type
    GetResult<Component> GetModelComponent(const std::string& name);
    GetResult<Component> GetLightComponent(const std::string& name);
    GetResult<Component> GetEmitterComponent(const std::string& name);

    // helpers
    FbxFileTexture* FileTextureFromMaterial(FbxSurfaceMaterial* material, const std::string& propertyName);
    lkCommon::Utils::PixelFloat4 ColorVector4FromMaterial(FbxSurfaceMaterial* material);

public:
    Scene();
    ~Scene();

    bool Init(const std::string& fbxFile = "");
    Object* CreateObject();
    GetResult<Component> GetComponent(ComponentType type, const std::string& name);
    GetResult<Material> GetMaterial(const std::string& name);
    void ForEachLight(Callback<Light> func) const;
    void ForEachEmitter(Callback<Emitter> func) const;
    void ForEachObject(Callback<Object> func) const;

    LKCOMMON_INLINE uint32_t GetEmitterCount() const
    {
        return static_cast<uint32_t>(mEmitterComponents.size());
    }
};

} // namespace Scene
} // namespace Krayo
