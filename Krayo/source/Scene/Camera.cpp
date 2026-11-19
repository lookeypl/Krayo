#include "Scene/Camera.hpp"


namespace Krayo {
namespace Scene {

void Camera::Update(const CameraDesc& desc)
{
    mView = lkCommon::Math::Matrix4::CreateRHLookAt(desc.pos, desc.at, desc.up);
    mPosition = desc.pos;
    mAtPosition = desc.at;
    mUpVector = desc.up;
}

} // namespace Scene
} // namespace Krayo
