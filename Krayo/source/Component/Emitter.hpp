#pragma once

#include "Prerequisites.hpp"
#include "IComponent.hpp"

#include <lkCommon/Math/Vector4.hpp>
#include <lkCommon/System/Memory.hpp>

#include <vector>


namespace Krayo {
namespace Component {
namespace Internal {

struct alignas(16) EmitterData
{
    lkCommon::Math::Vector4 pos;
    uint32_t particleLimit;
    float spawnPeriod;
    float spawnTimer;
    float lifeTime;
    mutable uint32_t lastSpawnedParticle;
    mutable uint32_t particleDataOffset;

    EmitterData()
        : pos()
        , particleLimit(0)
        , spawnPeriod(0.0f)
        , spawnTimer(0.0f)
        , lifeTime(0.0f)
        , particleDataOffset(0)
        , lastSpawnedParticle(0)
    {
    }

    EmitterData(const lkCommon::Math::Vector4& p, uint32_t limit, float period, float timer)
        : pos(p)
        , particleLimit(limit)
        , spawnPeriod(period)
        , spawnTimer(timer)
        , particleDataOffset(0)
        , lastSpawnedParticle(0)
    {
    }
};

struct EmitterDesc
{
    lkCommon::Math::Vector4 pos;
    uint32_t particleLimit;
    float spawnPeriod;

    EmitterDesc()
        : pos()
        , particleLimit(0)
        , spawnPeriod(0.0f)
    {
    }

    EmitterDesc(const lkCommon::Math::Vector4& p, uint32_t limit, float period)
        : pos(p)
        , particleLimit(limit)
        , spawnPeriod(period)
    {
    }
};

class Emitter: public IComponent
{
    friend class Renderer::ParticleEngine;

    EmitterData mData;

    LKCOMMON_INLINE void SetParticleDataOffset(uint32_t offset) const
    {
        mData.particleDataOffset = offset;
    }

public:
    Emitter(const std::string& name);

    void Init(const EmitterDesc& desc);
    LKCOMMON_INLINE void* operator new (std::size_t size)
    {
        return lkCommon::System::Memory::AlignedAlloc(size, 16);
    }

    LKCOMMON_INLINE void operator delete(void* ptr)
    {
        lkCommon::System::Memory::AlignedFree(ptr);
    }

    LKCOMMON_INLINE void SetPosition(const lkCommon::Math::Vector4& pos)
    {
        mData.pos = pos;
    }

    LKCOMMON_INLINE void SetSpawnPeriod(float spawnPeriod)
    {
        mData.spawnPeriod = spawnPeriod;
    }

    LKCOMMON_INLINE void SetLifeTime(float time)
    {
        mData.lifeTime = time;
    }

    LKCOMMON_INLINE void SetParticleLimit(uint32_t particleLimit)
    {
        mData.particleLimit = particleLimit;
    }

    LKCOMMON_INLINE const EmitterData& GetData() const
    {
        return mData;
    }

    LKCOMMON_INLINE uint32_t GetParticleLimit() const
    {
        return mData.particleLimit;
    }

    LKCOMMON_INLINE uint32_t GetParticleDataOffset() const
    {
        return mData.particleDataOffset;
    }

    LKCOMMON_INLINE Type GetType() const
    {
        return Type::Emitter;
    }
};

using EmitterCollection = std::vector<const Emitter*>;

} // namespace Internal
} // namespace Component
} // namespace Krayo