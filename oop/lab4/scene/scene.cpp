#include "scene.h"

namespace scene {

Scene::Scene(): _objects(new Composite) {}

void Scene::removeModel(std::shared_ptr<Object> obj) {
    ssize_t i = _objects->getIndex(obj);
    if (i > -1) {
        _objects->remove(size_t(i));
    }
}

void Scene::removeCamera(std::string cam_name) {
    _cameras.erase(cam_name);
}

std::shared_ptr<Composite> Scene::getModelsComposite() {
    return _objects;
}

std::shared_ptr<Camera> Scene::getCamera(std::string cam_name) {
    return _cameras.at(cam_name);
}

std::shared_ptr<Object> Scene::getObject(std::string model_name) {
    auto objects = _objects->getObjects();
    for (auto &obj: objects) {
        if (obj->getNmae() == model_name) {
            return obj;
        }
    }

    throw exceptions::ObjectSearchException();
}

void Scene::addModel(std::shared_ptr<Model> obj) {
    _objects->add(obj);
}

void Scene::addCamera(std::shared_ptr<Camera> obj) {
    _cameras.emplace(obj->getNmae(), obj);
}

std::vector<std::shared_ptr<Object>> Scene::getObjects() {
    return _objects->getObjects();
}

} // namespace scene
