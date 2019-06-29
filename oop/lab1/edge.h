#ifndef LAB1_EDGE_H
#define LAB1_EDGE_H

#include <QPainter>
#include <fstream>
#include <cassert>
#include "source.h"
#include "point.h"

const int PARSE_EDGE_ERR = -2;

struct Edge {
    int firstPoint;
    int secondPoint;
};

int scanEdges(Edge * edges, Source & source, const int edgesNum, const int pointsNum);
void writeEdge(Edge & edge, Source & source);
void drawEdge(QPainter * painter, const Edge & edge, const Point2D * points);

#endif //LAB1_EDGE_H
