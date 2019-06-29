#include "model.h"
#include <QDebug>
#include <iostream>

Model initModel() {
    return {nullptr, nullptr, 0, 0, false};
}

void initModelPoints(Model & model) {
    model.points = new Point[model.pointsNumber];
}

void initModelEdges(Model & model) {
    model.edges = new Edge[model.edgesNumber];
}

void writeModelHeader(const Model & model, const Source & source) {
    writeInt(source, model.pointsNumber);
    writeString(source, " ");
    writeInt(source, model.edgesNumber);
    writeString(source, "\n");
}

int saveModel(const Model & model, const SaveParams & params) {
    Source source = initSource();
    int errCode = openWriteSource(source, params);

    if (!errCode) {
        writeModelHeader(model, source);
        for (int i = 0; i < model.pointsNumber; i++) {
            writePoint(source, model.points[i]);
        }
        for (int i = 0; i < model.edgesNumber; i++) {
            writeEdge(model.edges[i], source);
        }
    }

    closeSource(source);
    return errCode;
}

int scanModelHeader(Model & model, Source source) {
    return readInt(model.pointsNumber, source) || readInt(model.edgesNumber, source);
}

int readModel(Model & model, const LoadParams & params) {
    Source source = initSource();
    int errCode = openReadSource(source, params);

    if (!errCode) {
        errCode = scanModelHeader(model, source);
    }
    if (!errCode) {
        model.isLoaded = true;
        initModelPoints(model);
        errCode = scanDots(model.points, source, model.pointsNumber);
    }
    if (!errCode) {
        initModelEdges(model);
        errCode = scanEdges(model.edges, source, model.edgesNumber, model.pointsNumber);
    }

    closeSource(source);
    return errCode;
}

int validateLoadParams(const LoadParams & params) {
    return !(params.path && strcmp(params.path, ""));
}

int loadModel(Model & model, const LoadParams & params) {
    int errCode = validateLoadParams(params);

    if (!errCode) {
        Model loadingModel = initModel();
        errCode = readModel(loadingModel, params);
        if (!errCode) {
            freeModel(model);
            model = loadingModel;
        } else {
            freeModel(loadingModel);
        }
    }

    return errCode;
}

int moveModel(Model & model, const MoveParams & params)
{
    for (int i = 0; i < model.pointsNumber; i++) {
        movePoint(model.points[i], params.dx, params.dy, params.dz);
    }

    return 0;
}

int scaleModel(Model & model, const ScaleParams & params) {
    for (int i = 0; i < model.pointsNumber; i++) {
        scalePoint(model.points[i], params.kx, params.ky, params.kz);
    }

    return 0;
}

int rotateModel(Model & model, const RotateParams & params) {
    double xAngleRad = params.xAngle * M_PI / 180;
    double yAngleRad = params.yAngle * M_PI / 180;
    double zAngleRad = params.zAngle * M_PI / 180;

    for (int i = 0; i < model.pointsNumber; i++) {
        rotatePoint(model.points[i], xAngleRad, yAngleRad, zAngleRad);
    }

    return 0;
}


Point2D * get2DProjection(Model & model, const int camDist, const int width, const int height) {
    auto points = new Point2D[model.pointsNumber];

    for (int i = 0; i < model.pointsNumber; i++) {
        getPointProjection(points[i], model.points[i], width, height, camDist);
    }

    return points;
}

int drawModel(Model & model, QPainter * painter, const QWidget * widget) {
    auto points = get2DProjection(model, 300, widget->width(), widget->height());

    painter->setPen(Qt::white);
    for (int i = 0; i < model.edgesNumber; i++) {
        drawEdge(painter, model.edges[i], points);
    }

    free2DPoints(points);
    return 0;
}

int freeModel(Model & model) {
    if (model.isLoaded) {
        delete[] model.points;
        delete[] model.edges;

        model.edges = nullptr;
        model.points = nullptr;
        model.isLoaded = false;
    }

    return 0;
}
