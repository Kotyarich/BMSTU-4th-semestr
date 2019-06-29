#ifndef LAB1_POINT_H
#define LAB1_POINT_H

#include <cmath>
#include "source.h"

const int PARSE_POINT_ERR = -2;

struct Point {
    double x;
    double y;
    double z;
};

struct Point2D {
    double x;
    double y;
};

void movePoint(Point & point, const int dx, const int dy, const int dz);
void scalePoint(Point & point, const double kx, const double ky, const double kz);
void rotatePoint(Point & point, const double xAngle, const double yAngle, const double zAngle);
void getPointProjection(Point2D & point2D, const Point & point, const int w, const int h, const int dist);

int scanDots(Point * points, Source source, const int dotsNumber);
void writePoint(Source & source, const Point & point);
void free2DPoints(Point2D * points);

#endif //LAB1_POINT_H
