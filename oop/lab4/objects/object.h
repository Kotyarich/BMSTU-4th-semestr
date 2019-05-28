#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "../math/point.h"
#include "../math/matrix.h"

namespace objects {

class Object {
public:
    Object() = default;
    Object(std::string name): _name(name) {}
    virtual ~Object() = default;
    virtual bool isVisible() const = 0;
    virtual void transform(const math::Matrix &matrix) = 0;
    std::string getNmae() { return _name; }
private:
    std::string _name;
};

} // namespace objects

#endif // OBJECT_H
