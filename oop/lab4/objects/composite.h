#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <vector>
#include <memory>
#include "object.h"

namespace objects {

class Composite: public Object
{
public:
    Composite() = default;

    void add(std::shared_ptr<Object> obj);
    void remove(size_t ind);
    ssize_t getIndex(std::shared_ptr<Object> obj);

    std::vector<std::shared_ptr<Object>> getObjects();

    bool isVisible() const override { return false; }
    void transform(const math::Matrix &matrix) override;
private:
    std::vector<std::shared_ptr<Object>> _objects;
};

} // namespace objects
#endif // COMPOSITE_H
