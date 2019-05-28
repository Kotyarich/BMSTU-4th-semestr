#include "renderer.h"

namespace render {

void Renderer::render(std::vector<std::pair<Point, Point>> lines, std::shared_ptr<BaseDrawer> drawer) {
    for (auto &line: lines) {
        drawer->drawLine(line.first, line.second);
    }
}

} // namespace render
