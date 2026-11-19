#pragma once

namespace Krayo {
namespace Utils {
namespace Traits {

//// TypeName trait ////

template <typename T>
class TypeName
{
public:
    static const char* Get()
    {
        return typeid(T).name();
    }
};

} // namespace Traits
} // namespace Utils
} // namespace Krayo
