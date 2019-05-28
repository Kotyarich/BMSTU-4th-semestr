#ifndef LAB4_BASEDRAWER_H
#define LAB4_BASEDRAWER_H

#include "../math/point.h"

namespace render {

using math::Point;

class BaseDrawer {
public:
    virtual ~BaseDrawer() = default;
    virtual void drawLine(const Point &p1, const Point &p2) = 0;
};

} // namespace render

#endif //LAB4_BASEDRAWER_H
