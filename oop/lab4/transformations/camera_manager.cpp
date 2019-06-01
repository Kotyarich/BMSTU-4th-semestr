#include "camera_manager.h"

namespace transformations {

void CameraManager::yaw(std::shared_ptr<Camera> camera, double angle) {
    double cur_angle = camera->getZAngle();
    camera->setZAngle(angle / 180. * 3.1415926535 + cur_angle);
}

void CameraManager::roll(std::shared_ptr<objects::Camera> camera, double angle) {
    double cur_angle = camera->getXAngle();
    camera->setXAngle(angle / 180. * 3.1415926535 + cur_angle);
}

void CameraManager::pitch(std::shared_ptr<objects::Camera> camera, double angle) {
    double cur_angle = camera->getYAngle();
    camera->setYAngle(angle / 180. * 3.1415926535 + cur_angle);
}

} // namespace transformations
