#include "camerarotator.h"

namespace transformations {

void CameraRotator::yaw(objects::Camera &camera, double angle) {
    double cur_angle = camera.getZAngle();
    camera.setZAngle(angle + cur_angle);
}

void CameraRotator::roll(objects::Camera &camera, double angle) {
    double cur_angle = camera.getXAngle();
    camera.setXAngle(angle + cur_angle);
}

void CameraRotator::pitch(objects::Camera &camera, double angle) {
    double cur_angle = camera.getYAngle();
    camera.setYAngle(angle + cur_angle);
}

} // namespace transformations
