#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "object.h"
#include "mesh/mesh.h"
#include "../math/point.h"

namespace objects {

using math::Point;

class Model: public Object {
public:
    Model(std::string);

    void addPoint(Point &p);
    void addEdge(size_t first, size_t second);
    void transform(const std::shared_ptr<Matrix> matrix) override;
    void accept(std::shared_ptr<Visitor> visitor) override;
    std::shared_ptr<Mesh> getMesh() { return _mesh; }

private:
    std::shared_ptr<Mesh> _mesh;
};

} // namespace objects

#endif // MODEL_H
