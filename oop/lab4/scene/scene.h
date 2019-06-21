#ifndef LAB4_SCENE_H
#define LAB4_SCENE_H

#include <unordered_map>
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
    void removeModel(std::shared_ptr<Object> obj);
    void removeCamera(std::string cam_name);

    std::shared_ptr<Composite> getModelsComposite();
    std::shared_ptr<Camera> getCamera(std::string cam_name);
    std::shared_ptr<Object> getObject(std::string model_name);
    std::vector<std::shared_ptr<Object>> getObjects();
private:
    std::shared_ptr<Composite> _objects;
    std::unordered_map<std::string, std::shared_ptr<Camera>> _cameras;
};

} // namespace scene

#endif //LAB4_SCENE_H
