#include "model.h"

namespace objects {

Model::Model(std::string name): Object(name), _points_number(0), _edges_number(0) {}

void Model::addEdge(Point p1, Point p2) {
    _edges.emplace_back(p1, p2);
}

void Model::transform(const math::Matrix &matrix) {
    for (auto &edge: _edges) {
        edge.first.multMatrix(matrix);
        edge.second.multMatrix(matrix);
    }
}

const std::vector<std::pair<Point, Point>> Model::getEdges() const {
    return _edges;
}

} // namespace objects
