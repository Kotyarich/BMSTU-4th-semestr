#ifndef LAB4_RENDERER_H
#define LAB4_RENDERER_H

#include <memory>
#include "base_manager.h"
#include "base_drawer.h"
#include "../exceptions/drawer_exception.h"
#include "../visitor/visitor.h"
#include "../objects/composite.h"
#include "../objects/model.h"
#include "../objects/camera.h"
#include "../math/point.h"
#include "../math/transform_matrix.h"

namespace render {

using namespace math;
using namespace objects;

class Renderer: public Visitor, public BaseManager {
public:
    void setDrawer(std::shared_ptr<BaseDrawer> drawer);
    void setCamera(std::shared_ptr<Camera> camera);
    void visit(Model &model) override;
    void visit(Composite &composite) override;
    void visit(Camera &camera) override;

private:
    std::shared_ptr<BaseDrawer> _drawer;
    std::shared_ptr<Camera> _camera;

    void proectPoint(Point &point);
};

} // namespace render;

#endif //LAB4_RENDERER_H
