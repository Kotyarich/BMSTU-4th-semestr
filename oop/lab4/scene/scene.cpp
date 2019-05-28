#include "scene.h"

namespace scene {

Scene::Scene(): _current_camera(nullptr) {}

void Scene::removeObject(std::shared_ptr<Object> obj) {
    ssize_t i = _objects.getIndex(obj);
    if (obj->getNmae() == _current_camera->getNmae()) {
        _current_camera = nullptr;
    }
    if (i > -1) {
        _objects.remove(size_t(i));
    }
}

std::shared_ptr<Object> Scene::getObjectByName(std::string obj_name) {
    for (auto obj: _objects.getObjects()) {
        if (obj->getNmae() == obj_name) {
            return obj;
        }
    }

    throw exceptions::ObjectSearchException();
}

void Scene::addModel(std::shared_ptr<Model> obj) {
    _objects.add(obj);
}

void Scene::addCamera(std::shared_ptr<Camera> obj) {
    _objects.add(obj);
}

void Scene::setCamera(std::string cam_name) {
    auto obj = getObjectByName(cam_name);
    _current_camera = dynamic_cast<Camera*>(obj.get());
}

Camera *Scene::getCurrentCamera() {
    if (!_current_camera) {
        throw exceptions::CameraLackException();
    }
    return _current_camera;
}
std::vector<std::shared_ptr<Object>> Scene::getObjects() {
    return _objects.getObjects();
}

} // namespace scene
