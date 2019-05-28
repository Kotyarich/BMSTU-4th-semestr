#include "intermediary.h"

namespace intermediary {

Intermediary::Intermediary(): _loader(new uploading::FileLoader), _current_camera(nullptr) {}

void Intermediary::addModel(std::string file_name, std::string name) {
    auto model = new objects::Model(name);
    _loader.loadModel(*model, file_name);
    _scene.addModel(std::shared_ptr<objects::Model>(model));
}

void Intermediary::addCamera(std::string name) {
    auto camera = new objects::Camera(name);
    _scene.addCamera(std::shared_ptr<objects::Camera>(camera));
}

void Intermediary::setCamera(std::string cam_name) {
    _scene.setCamera(cam_name);
}

void Intermediary::removeObject(std::string obj_name) {
    _scene.removeObject(_scene.getObjectByName(obj_name));
}

void Intermediary::moveObject(std::string obj_name, int x, int y, int z) {
    auto obj = _scene.getObjectByName(obj_name);
    if (obj != nullptr) {
        _transformer.moveObject(obj, x, y, z);
    }
}

void Intermediary::scaleObject(std::string obj_name, double x, double y, double z) {
    auto obj = _scene.getObjectByName(obj_name);
    if (obj != nullptr) {
        _transformer.scaleObject(obj, x, y, z);
    }
}

void Intermediary::rotateObjectX(std::string obj_name, double angle) {
    auto obj = _scene.getObjectByName(obj_name);
    if (obj != nullptr) {
        _transformer.rotateObjectX(obj, angle);
    }
}

void Intermediary::rotateObjectY(std::string obj_name, double angle) {
    auto obj = _scene.getObjectByName(obj_name);
    if (obj != nullptr) {
        _transformer.rotateObjectY(obj, angle);
    }
}

void Intermediary::rotateObjectZ(std::string obj_name, double angle) {
    auto obj = _scene.getObjectByName(obj_name);
    if (obj != nullptr) {
        _transformer.rotateObjectZ(obj, angle);
    }
}

void Intermediary::yawCamera(std::string cam_name, double angle) {
    auto obj = _scene.getObjectByName(cam_name);
    if (obj->isVisible()) {
        throw exceptions::ObjectTypeException();
    }

    Camera *camera = dynamic_cast<Camera*>(obj.get());
    _cam_rotator.yaw(*camera, angle);
}

void Intermediary::rollCamera(std::string cam_name, double angle) {
    auto obj = _scene.getObjectByName(cam_name);
    if (obj->isVisible()) {
        throw exceptions::ObjectTypeException();
    }

    Camera *camera = dynamic_cast<Camera*>(obj.get());
    _cam_rotator.roll(*camera, angle);
}

void Intermediary::pitchCamera(std::string cam_name, double angle) {
    auto obj = _scene.getObjectByName(cam_name);
    if (obj->isVisible()) {
        throw exceptions::ObjectTypeException();
    }

    Camera *camera = dynamic_cast<Camera*>(obj.get());
    _cam_rotator.roll(*camera, angle);
}

void Intermediary::draw(std::shared_ptr<BaseDrawer> drawer) {
    auto objects = _scene.getObjects();
    auto camera = _scene.getCurrentCamera();

    for (auto &obj: objects) {
        if (obj->isVisible()) {
            Model model = *(dynamic_cast<Model*>(obj.get()));
            auto edges = model.getEdges();
            for (auto &edge: edges) {
                _transformer.projectPoint(edge.first, *camera);
                _transformer.projectPoint(edge.second, *camera);
            }
            _renderer.render(edges, drawer);
        }
    }
}

} // namespace intermediary
