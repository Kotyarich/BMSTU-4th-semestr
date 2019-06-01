#include "renderer.h"

namespace render {

void Renderer::setDrawer(std::shared_ptr<BaseDrawer> drawer) {
    _drawer = drawer;
}

void Renderer::setCamera(std::shared_ptr<Camera> camera) {
    _camera = camera;
}

void Renderer::visit(Model &model) {
    if (!_drawer || !_camera) {
        throw exceptions::DrawerInitException("Drawer or camera wasn't set");
    }
    auto mesh = model.getMesh();
    auto lines = mesh->getLines();
    for (auto &line: lines) {
        proectPoint(line.first);
        proectPoint(line.second);
        _drawer->drawLine(line.first, line.second);
    }
}

void Renderer::visit(Composite&) {}

void Renderer::visit(Camera&) {}

void Renderer::proectPoint(math::Point &p) {
    auto cam_pos = _camera->getPosition();
    std::shared_ptr<Matrix> transform_matrix(new MoveMatrix(-cam_pos.x(), -cam_pos.y(), 0));
    p.transform(transform_matrix);
    transform_matrix.reset(new RotateOxMatrix(-_camera->getXAngle()));
    p.transform(transform_matrix);
    transform_matrix.reset(new RotateOyMatrix(-_camera->getYAngle()));
    p.transform(transform_matrix);
    transform_matrix.reset(new RotateOzMatrix(-_camera->getZAngle()));
    p.transform(transform_matrix);

    double eps = 1e-10;
    double znam = p.z() + cam_pos.z();
    if (znam < eps && znam > -eps) {
        znam = eps;
    }
    double distCoef = cam_pos.z() / znam;
    p.setX(p.x() * distCoef);
    p.setY(p.y() * distCoef);
}

} // namespace render
