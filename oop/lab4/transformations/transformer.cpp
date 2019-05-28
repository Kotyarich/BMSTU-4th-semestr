#include "transformer.h"

namespace transformations {

void Transformer::rotateObjectX(std::shared_ptr<Object> obj, double angle) {
    auto transform_matrix = getRotXMatrix(angle);
    obj->transform(transform_matrix);
}

void Transformer::rotateObjectY(std::shared_ptr<Object> obj, double angle) {
    auto transform_matrix = getRotYMatrix(angle);
    obj->transform(transform_matrix);
}

void Transformer::rotateObjectZ(std::shared_ptr<Object> obj, double angle) {
    auto transform_matrix = getRotZMatrix(angle);
    obj->transform(transform_matrix);
}

void Transformer::moveObject(std::shared_ptr<Object> obj, int x, int y, int z) {
    auto transform_matrix = getMoveMatrix(x, y, z);
    obj->transform(transform_matrix);
}

void Transformer::scaleObject(std::shared_ptr<Object> obj, double x, double y, double z) {
    auto transform_matrix = getScaleMatrix(x, y, z);
    obj->transform(transform_matrix);
}

math::Matrix Transformer::getRotXMatrix(double angle) {
    Matrix transform_matrix;
    angle = degreesToRadians(angle);

    transform_matrix.set(0, 0, 1);
    transform_matrix.set(1, 1, cos(angle));
    transform_matrix.set(1, 2, -sin(angle));
    transform_matrix.set(2, 1, sin(angle));
    transform_matrix.set(2, 2, cos(angle));
    transform_matrix.set(3, 3, 1);

    return transform_matrix;
}

math::Matrix Transformer::getRotYMatrix(double angle) {
    Matrix transform_matrix;
    angle = degreesToRadians(angle);

    transform_matrix.set(0, 0, cos(angle));
    transform_matrix.set(1, 1, 1);
    transform_matrix.set(2, 0, -sin(angle));
    transform_matrix.set(0, 2, sin(angle));
    transform_matrix.set(2, 2, cos(angle));
    transform_matrix.set(3, 3, 1);;

    return transform_matrix;
}

math::Matrix Transformer::getRotZMatrix(double angle) {
    Matrix transform_matrix;
    angle = degreesToRadians(angle);

    transform_matrix.set(0, 0, cos(angle));
    transform_matrix.set(0, 1, -sin(angle));
    transform_matrix.set(1, 0, sin(angle));
    transform_matrix.set(1, 1, cos(angle));
    transform_matrix.set(2, 2, 1);
    transform_matrix.set(3, 3, 1);

    return transform_matrix;
}

math::Matrix Transformer::getScaleMatrix(double x, double y, double z) {
    Matrix transform_matrix;

    transform_matrix.set(0, 0, x);
    transform_matrix.set(1, 1, y);
    transform_matrix.set(2, 2, z);
    transform_matrix.set(3, 3, 1);

    return transform_matrix;
}

math::Matrix Transformer::getMoveMatrix(int x, int y, int z) {
    Matrix transform_matrix;

    transform_matrix.set(0, 0, 1);
    transform_matrix.set(1, 1, 1);
    transform_matrix.set(2, 2, 1);
    transform_matrix.set(3, 3, 1);
    transform_matrix.set(0, 3, x);
    transform_matrix.set(1, 3, y);
    transform_matrix.set(2, 3, z);

    return transform_matrix;
}

double Transformer::degreesToRadians(double angle) {
    return angle / 180. * 3.1415926535;
}

void Transformer::projectPoint(Point &p, const objects::Camera &viewer) {
    p.multMatrix(getMoveMatrix(-viewer.getPosition().x(), -viewer.getPosition().y(), 0));
    p.multMatrix(getRotXMatrix(-degreesToRadians(viewer.getXAngle())));
    p.multMatrix(getRotYMatrix(-degreesToRadians(viewer.getYAngle())));
    p.multMatrix(getRotZMatrix(-degreesToRadians(viewer.getZAngle())));

    double eps = 1e-10;
    double znam = p.z() + viewer.getPosition().z();
    if (znam < eps && znam > -eps) {
        znam = eps;
    }
    double distCoef = viewer.getPosition().z() / znam;
    p.setX(p.x() * distCoef);
    p.setY(p.y() * distCoef);
}

} // namespace transformations
