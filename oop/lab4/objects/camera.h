#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"
#include "../math/point.h"

namespace objects {

class Camera: public Object
{
public:
    Camera(std::string name);

    const math::Point &getPosition() const;
    void setPosition(const math::Point &_position);
    double getXAngle() const;
    void setXAngle(double _x_angle);
    double getYAngle() const;
    void setYAngle(double _y_angle);
    double getZAngle() const;
    void setZAngle(double _z_angle);

    bool isVisible() const override { return false; }
    void transform(const math::Matrix &matrix) override;
private:
    math::Point _position;
    double _x_angle;
    double _y_angle;
    double _z_angle;
};

} // namespace objects

#endif // CAMERA_H
