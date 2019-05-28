#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "object.h"
#include "../math/point.h"

namespace objects {

using math::Point;

class Model: public Object
{
public:
    Model(std::string);

    void addEdge(Point p1, Point p2);
    bool isVisible() const override { return true; }
    void transform(const math::Matrix &matrix) override;

    const std::vector<std::pair<Point, Point>> getEdges() const;
    void setPointsNumber(size_t n) { _points_number = n; }
    void setEdgesNumber(size_t n) { _edges_number = n; }
    size_t getPointsNumber() const { return _points_number; }
    size_t getEdgesNumber() const {return _edges_number; }

private:
    size_t _points_number;
    size_t _edges_number;
    std::vector<std::pair<Point, Point>> _edges;
};

} // namespace objects

#endif // MODEL_H
