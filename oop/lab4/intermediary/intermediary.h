#ifndef INTERMEDIARY_H
#define INTERMEDIARY_H

#include <memory>
#include "../exceptions/object_type_exception.h"
#include "../transformations/transformer.h"
#include "../transformations/camerarotator.h"
#include "../render/renderer.h"
#include "../uploading/file_loader.h"
#include "../uploading/model_loader.h"
#include "../scene/scene.h"
#include "../objects/model.h"

namespace intermediary {

using objects::Object;
using objects::Model;
using objects::Camera;
using render::BaseDrawer;

class Intermediary {
public:
    Intermediary();

    void addModel(std::string file_name, std::string model_name);
    void addCamera(std::string cam_name);
    void setCamera(std::string cam_name);
    void removeObject(std::string obj_name);

    void moveObject(std::string obj_name, int x, int y, int z);
    void scaleObject(std::string obj_name, double x, double y, double z);
    void rotateObjectX(std::string obj_name, double angle);
    void rotateObjectY(std::string obj_name, double angle);
    void rotateObjectZ(std::string obj_name, double angle);

    void yawCamera(std::string cam_name, double angle);
    void rollCamera(std::string cam_name, double angle);
    void pitchCamera(std::string cam_name, double angle);

    void draw(std::shared_ptr<BaseDrawer> drawer);
private:
    uploading::ModelLoader _loader;
    transformations::Transformer _transformer;
    render::Renderer _renderer;
    scene::Scene _scene;
    transformations::CameraRotator _cam_rotator;
};

}

#endif // INTERMEDIARY_H
