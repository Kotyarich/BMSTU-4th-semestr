#ifndef ALGS_H
#define ALGS_H

#include <QPoint>
#include <vector>
#include <cassert>
#include <cmath>

const double PI = 3.14159265358979323846;

std::vector<QPoint> getCanonCircle(const int cx, const int cy, const int r);
std::vector<QPoint> getParamCircle(const int cx, const int cy, const int r);
std::vector<QPoint> getBrezCircle(const int cx, const int cy, const int r);
std::vector<QPoint> getMiddleCircle(const int cx, const int cy, const int r);

std::vector<QPoint> getCanonEllipse(const int cx, const int cy, const int a, const int b);
std::vector<QPoint> getParamEllipse(const int cx, const int cy, const int a, const int b);
std::vector<QPoint> getBrezEllipse(const int cx, const int cy, const int a, const int b);
std::vector<QPoint> getMiddleEllipse(const int cx, const int cy, const int a, const int b);

#endif // ALGS_H
