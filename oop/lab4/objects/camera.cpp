#include "camera.h"

namespace objects {

Camera::Camera(std::string name): Object(name), _x_angle(0), _y_angle(0), _z_angle(0) {
    _position = math::Point(0, 0, 300);
}

const math::Point &Camera::getPosition() const {
    return _position;
}
void Camera::setPosition(const math::Point &_position) {
    Camera::_position = _position;
}

double Camera::getXAngle() const {
    return _x_angle;
}
void Camera::setXAngle(double _x_angle) {
    Camera::_x_angle = _x_angle;
}
double Camera::getYAngle() const {
    return _y_angle;
}
void Camera::setYAngle(double _y_angle) {
    Camera::_y_angle = _y_angle;
}
double Camera::getZAngle() const {
    return _z_angle;
}
void Camera::setZAngle(double _z_angle) {
    Camera::_z_angle = _z_angle;
}

void Camera::transform(const std::shared_ptr<math::Matrix> matrix) {
    _position.transform(matrix);
}

void Camera::accept(std::shared_ptr<Visitor> visitor) {
    visitor->visit(*this);
}

} // namespace objects
