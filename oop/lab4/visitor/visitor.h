#ifndef VISITOR_H
#define VISITOR_H

namespace objects {

class Model;
class Camera;
class Composite;

class Visitor {
public:
    Visitor() = default;
    virtual ~Visitor() = default;

    virtual void visit(Camera &camera) = 0;
    virtual void visit(Model &model) = 0;
    virtual void visit(Composite &composite) = 0;
};

}

#endif
