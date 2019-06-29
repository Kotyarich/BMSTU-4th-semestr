#ifndef LAB1_COMMAND_H
#define LAB1_COMMAND_H

#include <QWidget>
#include <QPoint>
#include <string>

struct SaveParams {
    char * path;
};

struct LoadParams {
    char * path;
};

struct MoveParams {
    int dx;
    int dy;
    int dz;
};

struct ScaleParams {
    double kx;
    double ky;
    double kz;
};

struct RotateParams {
    int xAngle;
    int yAngle;
    int zAngle;
};

struct DrawParams {
    QWidget * widget;
    QPainter * painter;
};

struct Command {
    enum CommandType {
        save,
        load,
        move,
        scale,
        rotate,
        draw,
        free
    };

    CommandType commandType;
    union {
        SaveParams saveParams;
        LoadParams loadParams;
        MoveParams moveParams;
        ScaleParams scaleParams;
        RotateParams rotateParams;
        DrawParams drawParams;
    };
};

#endif //LAB1_COMMAND_H
