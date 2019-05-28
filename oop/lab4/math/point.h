#ifndef POINT_H
#define POINT_H

#include "matrix.h"

namespace math {

class Point {
public:
    Point();
    Point(double x, double y, double z);

    double x() const;
    double y() const;
    double z() const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);

    void multMatrix(const Matrix &matrix);
private:
    double _x;
    double _y;
    double _z;
};

} // namespace math

#endif // POINT_H
