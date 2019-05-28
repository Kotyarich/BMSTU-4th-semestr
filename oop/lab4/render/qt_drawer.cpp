#include "qt_drawer.h"

namespace render {

QtDrawer::QtDrawer(QPainter *painter): _painter(painter) {}

void QtDrawer::drawLine(const math::Point &p1, const math::Point &p2) {
    auto w = _painter->window().width() / 2;
    auto h = _painter->window().height() / 2;
    _painter->drawLine(w + std::round(p1.x()), h - std::round(p1.y()),
                       w + std::round(p2.x()), h - std::round(p2.y()));
}

} // namespace render
