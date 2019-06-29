#include "point.h"

void movePoint(Point & point, const int dx, const int dy, const int dz) {
    point.x += dx;
    point.y += dy;
    point.z += dz;
}

void scalePoint(Point & point, const double kx, const double ky, const double kz) {
    point.x *= kx;
    point.y *= ky;
    point.z *= kz;
}

void rotatePoint(Point & point, const double xAngle, const double yAngle, const double zAngle) {
    double new_y = point.y * cos(xAngle) - point.z * sin(xAngle);
    double new_z = point.z * cos(xAngle) + point.y * sin(xAngle);
    point.y = new_y;
    point.z = new_z;

    double  new_x = point.x * cos(yAngle) + point.z * sin(yAngle);
    new_z = point.z * cos(yAngle) - point.x * sin(yAngle);
    point.x = new_x;
    point.z = new_z;

    new_x = point.x * cos(zAngle) - point.y * sin(zAngle);
    new_y = point.y * cos(zAngle) + point.x * sin(zAngle);
    point.x = new_x;
    point.y = new_y;
}

void getPointProjection(Point2D & point2D, const Point & point, const int w, const int h, const int dist) {
    double distCoef = dist / (point.z + dist);
    point2D.x = static_cast<double>(w) / 2 + point.x * distCoef;
    point2D.y = static_cast<double>(h) / 2 - point.y * distCoef;
}

int scanDot(Point & point, Source source) {
    return readDouble(point.x, source) ||
           readDouble(point.y, source) ||
           readDouble(point.z, source);
}

int scanDots(Point * points, Source source, const int dotsNumber) {
    int errCode = 0;

    int i = 0;
    for (; i < dotsNumber && !errCode; i++) {
        errCode = scanDot(points[i], source);
    }
    if (i < dotsNumber) {
        errCode = PARSE_POINT_ERR;
    }

    return errCode;
}

void writePoint(Source & source, const Point & point)
{
    writeDouble(source, point.x);
    writeString(source, " ");
    writeDouble(source, point.y);
    writeString(source, " ");
    writeDouble(source, point.z);
    writeString(source, "\n");
}

void free2DPoints(Point2D * points) {
    delete[] points;
}
