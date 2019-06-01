#include "mesh.h"

namespace objects {

void Mesh::addPoint(math::Point &p) {
    _points.push_back(p);
}

void Mesh::addEdge(size_t first, size_t second) {
    if (first < 0 || second < 0 || first >= _points.size() || second >= _points.size()) {
        throw exceptions::ModelBuildException("Wrong point number");
    }
    _edges.emplace_back(first, second);
}

void Mesh::transform(const std::shared_ptr<math::Matrix> matrix) {
    for (auto &point: _points) {
        point.transform(matrix);
    }
}

std::vector<std::pair<math::Point, math::Point> > Mesh::getLines() {
    std::vector<std::pair<Point, Point>> lines;
    for (auto &edge: _edges) {
        lines.emplace_back(_points[edge.first], _points[edge.second]);
    }

    return lines;
}

} // namespace objects
