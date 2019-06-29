#include "edge.h"

int checkEdge(const Edge & edge, const int pointsNum) {
    return edge.firstPoint >= pointsNum || edge.firstPoint < 0
           || edge.secondPoint >= pointsNum || edge.secondPoint < 0;
}

int scanEdge(Edge & edge, Source & source) {
    return readInt(edge.firstPoint, source)  || readInt(edge.secondPoint, source);
}

int scanEdges(Edge * edges, Source & source, const int edgesNum, const int pointsNum) {
    int errCode = 0;

    int i = 0;
    for (; i < edgesNum && !errCode; i++) {
        errCode = scanEdge(edges[i], source);
        if (checkEdge(edges[i], pointsNum)) {
            errCode = PARSE_EDGE_ERR;
        }
    }
    if (i < edgesNum) {
        errCode = PARSE_EDGE_ERR;
    }

    return errCode;
}

void writeEdge(Edge & edge, Source & source) {
    writeInt(source, edge.firstPoint);
    writeString(source, " ");
    writeInt(source, edge.secondPoint);
    writeString(source, "\n");
}


void drawEdge(QPainter * painter, const Edge & edge, const Point2D * points) {
    painter->drawLine(points[edge.firstPoint].x, points[edge.firstPoint].y,
                      points[edge.secondPoint].x, points[edge.secondPoint].y);
}