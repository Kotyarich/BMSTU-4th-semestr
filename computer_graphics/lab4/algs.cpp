#include "algs.h"
#include <iostream>

std::vector<QPoint> getCanonCircle(const int cx, const int cy, const int r)
{
    assert(r > 0);

    std::vector<QPoint> result;
    const int a = r * r;
    int dirivX = int(std::round(r / sqrt(2)));

    for (int x = 0; x <= dirivX; x++) {
        int y = static_cast<int>(round(sqrt(a - x * x)));

        result.emplace_back(cx + x, cy + y);
        result.emplace_back(cx + x, cy - y);
        result.emplace_back(cx - x, cy + y);
        result.emplace_back(cx - x, cy - y);
        result.emplace_back(cx + y, cy + x);
        result.emplace_back(cx + y, cy - x);
        result.emplace_back(cx - y, cy + x);
        result.emplace_back(cx - y, cy - x);
    }

    return result;
}

std::vector<QPoint> getParamCircle(const int cx, const int cy, const int r)
{
    std::vector<QPoint> result;
    const double d = 1.0 / r;

    for (double i = PI / 2; i >= PI / 4; i -= d) {
        int x = static_cast<int>(std::round(r * cos(i)));
        int y = static_cast<int>(std::round(r * sin(i)));

        result.emplace_back(cx + x, cy + y);
        result.emplace_back(cx + x, cy - y);
        result.emplace_back(cx - x, cy + y);
        result.emplace_back(cx - x, cy - y);
        result.emplace_back(cx + y, cy + x);
        result.emplace_back(cx + y, cy - x);
        result.emplace_back(cx - y, cy + x);
        result.emplace_back(cx - y, cy - x);
    }

    return result;
}

std::vector<QPoint> getBrezCircle(const int cx, const int cy, const int r) {
    std::vector<QPoint> result;
    int x = 0;
    int y = r;

    int d = 2 * (1 - r);
    int d1, d2;

    while (y >= 0) {
        result.emplace_back(cx + x, cy + y);
        result.emplace_back(cx + x, cy - y);
        result.emplace_back(cx - x, cy + y);
        result.emplace_back(cx - x, cy - y);

        if (d < 0) {
            d1 = 2 * (d + y) - 1;
            if (d1 > 0) {
                y--;
                x++;
                d += 2 * (x - y + 1);
            } else {
                x++;
                d += 2 * x + 1;
            }
        } else if (!d) {
            x++;
            y--;
            d += 2 * (x - y + 1);
        } else {
            d2 = 2 * (d - x) - 1;
            if (d2 < 0) {
                y--;
                x++;
                d += 2 * (x - y + 1);
            } else {
                y--;
                d += -2 * y + 1;
            }
        }
    }

    return result;
}

std::vector<QPoint> getMiddleCircle(const int cx, const int cy, const int r)
{
    std::vector<QPoint> result;
    int x = 0;
    int y = r;
    int p = 1 - r;

    while (x <= y) {
        result.emplace_back(cx + x, cy + y);
        result.emplace_back(cx + x, cy - y);
        result.emplace_back(cx - x, cy + y);
        result.emplace_back(cx - x, cy - y);

        result.emplace_back(cx + y, cy + x);
        result.emplace_back(cx + y, cy - x);
        result.emplace_back(cx - y, cy + x);
        result.emplace_back(cx - y, cy - x);

        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
    }

    return result;
}

std::vector<QPoint> getCanonEllipse(const int cx, const int cy, const int a, const int b)
{
    std::vector<QPoint> result;
    const int a2 = a * a;
    const int b2 = b * b;

    int derivX = static_cast<int>(round(a2 / sqrt(a2 + b2)));
    double bDivA = double(b) / a;
    for (int x = 0; x <= derivX; x++) {
        int y = static_cast<int>(round(sqrt(double(a2 - x * x)) * bDivA));
        result.emplace_back(cx + x, cy + y);
        result.emplace_back(cx + x, cy - y);
        result.emplace_back(cx - x, cy + y);
        result.emplace_back(cx - x, cy - y);
    }

    int derivY = static_cast<int>(round(b2 / sqrt(a2 + b2)));
    for (int y = 0; y <= derivY; y++) {
        int x = static_cast<int>(round(sqrt(double(b2 - y * y)) / bDivA));
        result.emplace_back(cx + x, cy + y);
        result.emplace_back(cx + x, cy - y);
        result.emplace_back(cx - x, cy + y);
        result.emplace_back(cx - x, cy - y);
    }

    return result;
}

std::vector<QPoint> getParamEllipse(const int cx, const int cy, const int a, const int b)
{
    std::vector<QPoint> result;

    int maxAB = qMax(a, b);
    const double d = 1.0 / maxAB;

    for (double i = PI / 2; i >= 0; i -= d) {
        int x = static_cast<int>(std::round(a * cos(i)));
        int y = static_cast<int>(std::round(b * sin(i)));

        result.emplace_back(cx + x, cy + y);
        result.emplace_back(cx + x, cy - y);
        result.emplace_back(cx - x, cy + y);
        result.emplace_back(cx - x, cy - y);
    }

    return result;
}

std::vector<QPoint> getBrezEllipse(const int cx, const int cy, const int a, const int b) {
    std::vector<QPoint> result;
    int x = 0;
    int y = b;
    int sqr_a = a * a;
    int sqr_b = b * b;
    int d = static_cast<int>(std::round((sqr_a + sqr_b) / 2.0 - 2 * sqr_a * b));

    while (y >= 0) {
        result.emplace_back(cx + x, cy + y);
        result.emplace_back(cx + x, cy - y);
        result.emplace_back(cx - x, cy + y);
        result.emplace_back(cx - x, cy - y);

        if (d < 0) {
            int d2 = 2 * (d + sqr_a * y) - sqr_a;
            x++;
            if (d2 <= 0)
                d += sqr_b * (2 * x + 1);
            else {
                y--;
                d += 2 * (sqr_b * x - sqr_a * y) + sqr_a + sqr_b;
            }
        }
        else if (d > 0) {
            int d2 = 2 * (d - sqr_b * x) - sqr_b;
            y--;
            if (d2 > 0)
                d += sqr_a * (1 - 2 * y);
            else {
                x++;
                d += 2 * (sqr_b * x - sqr_a * y) + sqr_a + sqr_b;
            }
        }
        else {
            x++;
            y--;
            d += 2 * (sqr_b * x - sqr_a * y) + sqr_a + sqr_b;
        }
    }

    return result;
}

std::vector<QPoint> getMiddleEllipse(const int cx, const int cy, const int a, const int b) {
    std::vector<QPoint> result;

    long int a2 = a * a;
    long int b2 = b * b;
    int x = 0;
    int y = b;
    int delta = int(b2 + a2 * (y - 0.5) * (y - 0.5) - a2 * b2);
    int border = int(double(a2) / sqrt (b2 + a2));

    while (x <= border) {
        result.emplace_back(cx + x, cy + y);
        result.emplace_back(cx + x, cy - y);
        result.emplace_back(cx - x, cy + y);
        result.emplace_back(cx - x, cy - y);

        x++;
        if (delta > 0) {
            y--;
            delta += a2 * (-2 * y);
        }
        delta += b2 * (2 * x + 1);
    }

    delta += 0.75 * (a2 - b2) - (b2 * x + a2 * y);
    while (y >= 0){
        result.emplace_back(cx + x, cy + y);
        result.emplace_back(cx + x, cy - y);
        result.emplace_back(cx - x, cy + y);
        result.emplace_back(cx - x, cy - y);

        y--;
        if (delta < 0) {
            x++;
            delta += b2 * (2 * x);
        }
        delta += a2 * (-2 * y + 1);
    }

   return result;
}
