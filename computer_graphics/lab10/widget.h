#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include "drawlabel.h"

struct Point
{
    Point(double x_, double y_, double z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }
    double x;
    double y;
    double z;
};

struct NeededParams
{
    NeededParams(double X1, double X2, double X3, double Z1, double Z2, double Z3)
    {
        xB = X1;
        xE = X2;
        xD = X3;
        zB = Z1;
        zE = Z2;
        zD = Z3;
    }
    double (*f)(double, double);
    double xB;
    double xE;
    double xD;
    double zB;
    double zE;
    double zD;
};

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void render();

private:
    Ui::Widget *ui;
    DrawLabel *_draw_label;
    QPixmap _pixmap;
    double k;

    std::vector<std::vector<Point>> getDots(int xs, int xe, int dx, int zs, int ze, int dz, double (*func)(int x, int z));
    void floatingHorisont(QPainter &painter, double (*func)(int x, int z));
    QColor getLinesColor();
    void clear();
    void rotatePoint(double &x, double &y, double &z, double x_angle, double y_angle, double z_angle);
    void drawLine(int x1, int y1, int x2, int y2, QPainter &painter);
    void HorizonAlgo(NeededParams Params, QPainter &painter, double tetax, double tetay, double tetaz);
    void transform(double x, double y, double z, double tetax, double tetay, double tetaz, int &res_x, int &res_y, int width, int height, int *r);
    std::vector<std::vector<Point>> getDots(double (*f)(double, double), int gx, int gy, int gz, int xs, int xe, int dx, int zs, int ze, double dz, int, int);
};

#endif // WIDGET_H
