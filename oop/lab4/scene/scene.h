#ifndef LAB4_SCENE_H
#define LAB4_SCENE_H

#include <memory>
#include "../exceptions/camera_lack_exception.h"
#include "../exceptions/object_search_exception.h"
#include "../objects/composite.h"
#include "../objects/model.h"
#include "../objects/camera.h"

namespace scene {

using namespace objects;

class Scene {
 public:
    Scene();

    void addModel(std::shared_ptr<Model> obj);
    void addCamera(std::shared_ptr<Camera> obj);
    void setCamera(std::string cam_name);
    Camera *getCurrentCamera();
    void removeObject(std::shared_ptr<Object> obj);
    std::shared_ptr<Object> getObjectByName(std::string obj_name);

    std::vector<std::shared_ptr<Object>> getObjects();
 private:
    Composite _objects;
    Camera *_current_camera;
};

} // namespace scene

#endif //LAB4_SCENE_H
