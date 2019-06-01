#include "transformer.h"

namespace transformations {

void Transformer::rotateObjectX(std::shared_ptr<Object> obj, double angle) {
    angle = degreesToRadians(angle);
    std::shared_ptr<Matrix> transform_matrix(new RotateOxMatrix(angle));
    obj->transform(transform_matrix);
}

void Transformer::rotateObjectY(std::shared_ptr<Object> obj, double angle) {
    angle = degreesToRadians(angle);
    std::shared_ptr<Matrix> transform_matrix(new RotateOyMatrix(angle));
    obj->transform(transform_matrix);
}

void Transformer::rotateObjectZ(std::shared_ptr<Object> obj, double angle) {
    angle = degreesToRadians(angle);
    std::shared_ptr<Matrix> transform_matrix(new RotateOzMatrix(angle));
    obj->transform(transform_matrix);
}

void Transformer::moveObject(std::shared_ptr<Object> obj, int x, int y, int z) {
    std::shared_ptr<Matrix> transform_matrix(new MoveMatrix(x, y, z));
    obj->transform(transform_matrix);
}

void Transformer::scaleObject(std::shared_ptr<Object> obj, double x, double y, double z) {
    std::shared_ptr<Matrix> transform_matrix(new ScaleMatrix(x, y, z));
    obj->transform(transform_matrix);
}

double Transformer::degreesToRadians(double angle) {
    return angle / 180. * 3.1415926535;
}

} // namespace transformations
