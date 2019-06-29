#ifndef LAB1_MODEL_H
#define LAB1_MODEL_H

#include <QWidget>
#include <QPainter>
#include <string>
#include <fstream>
#include <cmath>
#include "command.h"
#include "source.h"
#include "edge.h"

struct Model {
    Edge *edges;
    Point *points;
    int edgesNumber;
    int pointsNumber;
    bool isLoaded;
};


Model initModel();
int loadModel(Model & model, const LoadParams & params);
int saveModel(const Model & model, const SaveParams & params);
int freeModel(Model & model);

int scaleModel(Model & model, const ScaleParams & params);
int rotateModel(Model & model, const RotateParams & params);
int moveModel(Model & model, const MoveParams & params);
Point2D *get2DProjection(Model & model, const int camDist, const int width, const int height);
int drawModel(Model & model, QPainter * painter, const QWidget * widget);

#endif //LAB1_MODEL_H
