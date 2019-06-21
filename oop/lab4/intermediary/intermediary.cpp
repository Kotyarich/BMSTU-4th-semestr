#include "intermediary.h"

namespace intermediary {

Intermediary::Intermediary():
    _renderer(new render::Renderer),
    _loader(new uploading::FileLoader) {}

void Intermediary::addModel(std::string file_name, std::string name) {    
    std::shared_ptr<Model> model = _loader.loadModel(file_name, name);
    _scene_manager.getScene()->addModel(std::shared_ptr<objects::Model>(model));
}

void Intermediary::addCamera(std::string name) {
    auto camera = new objects::Camera(name);
    _scene_manager.getScene()->addCamera(std::shared_ptr<objects::Camera>(camera));
}

void Intermediary::setCamera(std::string cam_name) {
    _scene_manager.setCurrentCamera(cam_name);
}

void Intermediary::removeCamera(std::string cam_name) {
    _scene_manager.getScene()->removeCamera(cam_name);
}

void Intermediary::removeModel(std::string model_name) {
    auto scene = _scene_manager.getScene();
    scene->removeModel(scene->getObject(model_name));
}

void Intermediary::transformCamera(std::string cam_name, math::Point &move, math::Point &rotate) {
    auto camera = _scene_manager.getScene()->getCamera(cam_name);
    _transformer.moveObject(camera, move.x(), move.y(), move.z());
    _cam_manager.roll(camera, rotate.x());
    _cam_manager.pitch(camera, rotate.y());
    _cam_manager.yaw(camera, rotate.z());
}

void Intermediary::transformModel(std::string model_name, math::Point &move, math::Point &scale, math::Point &rotate) {
    auto obj = _scene_manager.getScene()->getObject(model_name);
    _transformer.moveObject(obj, move.x(), move.y(), move.z());
    _transformer.scaleObject(obj, scale.x(), scale.y(), scale.z());
    _transformer.rotateObjectX(obj, rotate.x());
    _transformer.rotateObjectY(obj, rotate.y());
    _transformer.rotateObjectZ(obj, rotate.z());
}

void Intermediary::draw(std::shared_ptr<BaseDrawer> drawer) {
    _renderer->setDrawer(drawer);
    _renderer->setCamera(_scene_manager.getCurrentCamera());
    _scene_manager.getScene()->getModelsComposite()->accept(_renderer);
}

} // namespace intermediary
