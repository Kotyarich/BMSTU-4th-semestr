#ifndef TRANSFORM_MATRIX_H
#define TRANSFORM_MATRIX_H

#include <cmath>
#include "matrix.h"

namespace math {

class MoveMatrix: public Matrix {
public:
    MoveMatrix(int x, int y, int z);
};

class ScaleMatrix: public Matrix {
public:
    ScaleMatrix(double x, double y, double z);
};

class RotateOxMatrix: public Matrix {
public:
    RotateOxMatrix(double angle);
};

class RotateOyMatrix: public Matrix {
public:
    RotateOyMatrix(double angle);
};

class RotateOzMatrix: public Matrix {
public:
    RotateOzMatrix(double angle);
};

} // namespace math

#endif // TRANSFORM_MATRIX_H
