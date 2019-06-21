#ifndef LAB4_TRANSFORMER_H
#define LAB4_TRANSFORMER_H

#include <memory>
#include <cmath>
#include "../math/point.h"
#include "../math/transform_matrix.h"
#include "../objects/model.h"
#include "../objects/camera.h"

namespace transformations {

using namespace math;
using objects::Object;
using objects::Model;
using std::shared_ptr;

class Transformer {
public:   
    void rotateObjectX(std::shared_ptr<Object> obj, double angle);
    void rotateObjectY(std::shared_ptr<Object> obj, double angle);
    void rotateObjectZ(std::shared_ptr<Object> obj, double angle);
    void moveObject(std::shared_ptr<Object> obj, int x, int y, int z);
    void scaleObject(std::shared_ptr<Object> obj, double x, double y, double z);
    void projectPoint(Point &p, const objects::Camera &viewer);

private:
    double degreesToRadians(double angle);
};

} // namespace transformations

#endif //LAB4_TRANSFORMER_H
