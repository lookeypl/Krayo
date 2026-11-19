#pragma once

#include <lkCommon/lkCommon.hpp>

#include <fbxsdk.h>

#include <memory>
#include <functional>


namespace Krayo {
namespace Utils {

class FBXFile
{
    using TraverseCallback = std::function<void(FbxNode*)>;

    FbxManager* mFbxManager;
    FbxIOSettings* mFbxIOSettings;
    std::unique_ptr<FbxGeometryConverter> mFbxConverter;
    FbxImporter* mFbxImporter;
    FbxScene* mFbxScene;
    bool mIsOpened;
    uint32_t mNodeCount;
    uint32_t mCurrentNode;
    float mLastHitRatio;

    // debugging info print functions/enum translators
    std::string GetAttributeTypeName(FbxNodeAttribute::EType type);
    std::string GetLightTypeName(FbxLight::EType type);
    void InitialTraverseNode(FbxNode* node, int printTabs);
    void TraverseNode(TraverseCallback func, FbxNode* node);

public:
    FBXFile();
    ~FBXFile();

    bool Open(const std::string& path);
    void Traverse(TraverseCallback func);
    void Close();

    operator bool() const
    {
        return IsOpened();
    }

    LKCOMMON_INLINE bool IsOpened() const
    {
        return mIsOpened;
    }

    LKCOMMON_INLINE FbxGeometryConverter* GetConverter() const
    {
        return mFbxConverter.get();
    }
};

} // namespace Utils
} // namespace Krayo
