#ifndef LAB4_RENDERER_H
#define LAB4_RENDERER_H

#include <memory>
#include "base_drawer.h"
#include "../math/point.h"

namespace render {

using math::Point;

class Renderer {
public:
    void render(std::vector<std::pair<Point, Point>> lines, std::shared_ptr<BaseDrawer> drawer);
};

} // namespace render;

#endif //LAB4_RENDERER_H
