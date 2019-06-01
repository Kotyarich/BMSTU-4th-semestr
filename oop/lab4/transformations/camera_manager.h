#ifndef CAMERAROTATOR_H
#define CAMERAROTATOR_H

#include "base_manager.h"
#include "objects/camera.h"

namespace transformations {

using objects::Camera;

class CameraManager: public BaseManager {
public:
    void yaw(std::shared_ptr<Camera> camera, double angle);
    void roll(std::shared_ptr<Camera> camera, double angle);
    void pitch(std::shared_ptr<Camera> camera, double angle);
};

} // namespace transformations

#endif // CAMERAROTATOR_H
