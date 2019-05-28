#ifndef CAMERAROTATOR_H
#define CAMERAROTATOR_H

#include "objects/camera.h"

namespace transformations {

using objects::Camera;

class CameraRotator {
public:
    void yaw(Camera &camera, double angle);
    void roll(Camera &camera, double angle);
    void pitch(Camera &camera, double angle);
};

} // namespace transformations

#endif // CAMERAROTATOR_H
