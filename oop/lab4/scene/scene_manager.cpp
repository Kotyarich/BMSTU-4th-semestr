#include "scene_manager.h"

namespace scene {

SceneManager::SceneManager(): _scene(new Scene()){}

std::shared_ptr<Scene> SceneManager::getScene() {
    return _scene;
}

void SceneManager::setCurrentCamera(std::string cam_name) {
    _current_camera = _scene->getCamera(cam_name);
}

std::shared_ptr<Camera> SceneManager::getCurrentCamera() {
    if (!_current_camera) {
        throw exceptions::CameraLackException();
    }
    return _current_camera;
}

} // namespace scene
