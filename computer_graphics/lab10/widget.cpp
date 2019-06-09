#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <set>
#include <map>
#include <QMessageBox>
#include <QMutex>
#include <QWaitCondition>
#include <cmath>

double func1(double x, double z) {
    return sin(double(x) / 40) * sin(double(z) / 40) * 150;
}

double func2(double x, double z) {
    return 20 * cos(x / 40) * cos(x / 40) - 20 * sin(z / 40) * sin(z / 40);
}

double func3(double x, double z) {
    return x + cos(double(x) / 20) * cos(double(x) / 20) * z;
}

double func4(double x, double z) {
    return x * cos(double(z) / 30) + z * sin(double(x) / 30);
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget) {
    ui->setupUi(this);

    ui->comboBox->addItem("y = 100 * sin(x/40) * cos(x/40)", 0);
    ui->comboBox->addItem("y = 20*cos(x/40)^2 - 20*sin(z/40)^2", 1);
    ui->comboBox->addItem("y = x + cos(x/20) * z", 2);
    ui->comboBox->addItem("y = x * cos(z/30) + z * cos(x/30)", 3);

    _draw_label = new DrawLabel(_pixmap);
    _draw_label->resize(width() - ui->frame->width(), height());
    layout()->addWidget(_draw_label);
    _pixmap = QPixmap(_draw_label->width(), _draw_label->height());
    _pixmap.fill();
}

Widget::~Widget() {
    delete ui;
}

int sign(int x) {
    if (!x) {
        return 0;
    }
    if (x < 0) {
        return -1;
    }
    return 1;
}

void SearchIntersection(int x1, int y1, int x2, int y2, QVector<int> &horizon, int &xi, int &yi)
{
    xi = x1;
    yi = y1;
    int delta_x = x2 - x1;
    int delta_y_c = y2 - y1;
    int delta_y_p = horizon[x2] - horizon[x1];
    double m = delta_y_c / static_cast<double>(delta_x);
    if (delta_x == 0) {
        xi = x2;
        yi = horizon[x2];
    } else if (y1 == horizon[x1] && y2 == horizon[x2]) {
        xi = x1;
        yi = y1;
    } else {
        xi = x1 - static_cast<int>(round(delta_x * (y1 - horizon[x1]) / static_cast<double>(delta_y_c - delta_y_p)));
        yi = static_cast<int>(round((xi - x1) * m + y1));
    }
}



void horizon(int x1, int y1, int x2, int y2, QVector<int> &TOP, QVector<int> &DOWN, QPainter &painter)
{

    if (x2 < x1) {
        qSwap(x1, x2);
        qSwap(y1, y2);
    } if (x2 - x1 == 0) {
        TOP[x2]  = qMax(TOP[x2], y2);
        DOWN[x2] = qMin(DOWN[x2], y2);
        if (x2 >= 0 && x2 <= painter.window().width()) {
            painter.drawLine(x1, y1, x2, y2);
        }
    } else {
        int x_prev = x1;
        int y_prev = y1;
        double m = (y2 - y1) / static_cast<double>(x2 - x1);
        for (int x = x1; x <= x2; x++) {
            int y = static_cast<int>(round(m * (x - x1) + y1));
            TOP[x] = qMax(TOP[x], y);
            DOWN[x] = qMin(DOWN[x], y);
            if (x >= 0 && x <= painter.window().width()) {
                painter.drawLine(x_prev, y_prev, x, y);
            }
        }
    }
}

void EdgeWhatcher(int &x, int &y, int &xEdge, int &yEdge, QVector<int> &TOP, QVector<int> &DOWN, QPainter &painter)
{
    if (xEdge != -1) {
        horizon(xEdge, yEdge, x , y, TOP, DOWN, painter);
    }
    xEdge = x;
    yEdge = y;
}

int Visible(int x, int y, QVector<int> TOP, QVector<int> DOWN) {
    if (y < TOP[x] && y > DOWN[x]) return 0;
    if (y >= TOP[x]) return 1;
    return -1;
}

void rotate_x(double &y, double &z, double tetax) {
    tetax = tetax * M_PI / 180;
    double buf = y;
    y = cos(tetax) * y - sin(tetax) * z;
    z = cos(tetax) * z + sin(tetax) * buf;
}

void rotate_y(double &x, double &z, double tetay) {
    tetay = tetay * M_PI / 180;
    double buf = x;
    x = cos(tetay) * x - sin(tetay) * z;
    z = cos(tetay) * z + sin(tetay) * buf;
}

void rotate_z(double &x, double &y, double tetaz) {
    tetaz = tetaz * M_PI / 180;
    double buf = x;
    x = cos(tetaz) * x - sin(tetaz) * y;
    y = cos(tetaz) * y + sin(tetaz) * buf;
}

void Widget::transform(double x, double y, double z, double tetax, double tetay, double tetaz, int &res_x, int &res_y, int width, int height, int *r = nullptr) {
    double x_center = width / 2;
    double y_center = height / 2;
    double x_tmp = x;
    double y_tmp = y;
    double z_tmp = z;
    rotate_x(y_tmp, z_tmp, tetax);
    rotate_y(x_tmp, z_tmp, tetay);
    rotate_z(x_tmp, y_tmp, tetaz);
    res_x = static_cast<int>(round(x_tmp * k + x_center));
    res_y = static_cast<int>(round(y_tmp * k + y_center));
    if (r) {
        *r = int(round(z_tmp));
    }
}

void Widget::render() {
    int min_x = ui->x0->value();
    int max_x = ui->x_end->value();
    int dx = ui->dx->value();
    int min_z = ui->z0->value();
    int max_z = ui->z_end->value();
    int dz = ui->dz->value();
    // TODO add check here
    int x_angle = ui->x_angle->value();
    int y_angle = ui->y_angle->value();
    int z_angle = ui->z_angle->value();

    clear();
    QPainter painter(&_pixmap);
    painter.setPen(getLinesColor());

    int func_number = *((int*) ui->comboBox->currentData().data());
    double (*f) (double, double) = nullptr;
    switch (func_number) {
    case 0:
        f = func1;
        break;
    case 1:
        f = func2;
        break;
    case 2:
        f = func3;
        break;
    case 3:
        f= func4;
        break;
    }

    NeededParams params(min_x, max_x, dx, min_z, max_z, dz);
    params.f = f;
    HorizonAlgo(params, painter, x_angle, y_angle, z_angle);
    _draw_label->update();
}

QColor Widget::getLinesColor() {
    int r = ui->r->value();
    int g = ui->g->value();
    int b = ui->b->value();

    return QColor(r, g, b);
}

void Widget::clear() {
    _pixmap.fill();
    _draw_label->update();
}

void Widget::drawLine(int x1, int y1, int x2, int y2, QPainter &painter) {
    painter.drawLine(x1 + _pixmap.width() / 2, _pixmap.height() / 2 - y1,
                     x2 + _pixmap.width() / 2, _pixmap.height() / 2 - y2);
}

std::vector<std::vector<Point>> Widget::getDots(double (*f)(double, double), int gx, int gy, int gz, int xs, int xe, int dx, int zs, int ze, double dz, int w, int h) {
    std::vector<std::vector<Point>> result;
    for (double z = ze; z >= zs; z -= dz) {
        std::vector<Point> line;
        for (int x = xs; x < xe; x += dx) {
            int xt = x, yt = f(x, z), zt = z;
            transform(xt, yt, zt, gx, gy, gz, xt, yt, w, h, &zt);
            line.emplace_back(xt, yt, zt);
        }
        result.push_back(line);
    }

    std::sort(result.begin(), result.end(), [](std::vector<Point> &a, std::vector<Point> &b){ return a[a.size() / 2].z > b[b.size() / 2].z; });
    return result;
}

void Widget::HorizonAlgo(NeededParams Params, QPainter &painter, double tetax, double tetay, double tetaz) {
    {
        int dx = Params.xE - Params.xB;
        k = 1;
        if (abs(Params.xB) >= painter.window().width() / 2) {
            k = double(painter.window().width()) / 2 / abs(Params.xB);
        }
        if (abs(Params.xE) * k >= painter.window().width() / 2) {
            k = double(painter.window().width()) / 2 / abs(Params.xE);
        }
        auto dots = getDots(Params.f, tetax, tetay, tetaz, Params.xB, Params.xE, Params.xD, Params.zB, Params.zE, Params.zD, painter.window().width(), painter.window().height());

        QVector<int> TOP;
        QVector<int> DOWN;
        for (int i = 0; i <= painter.window().width(); i++) {
            TOP.push_back(0);
            DOWN.push_back(painter.window().width());
        }
        int x_left = -1;
        int y_left = -1;
        int x_right = -1;
        int y_right = -1;
        int x_prev, y_prev;
//        for (double z = Params.zE; z >= Params.zB; z -= Params.zD) {
        for (int i = 0; i < dots.size(); i++) {
            double z = dots[i][0].z;
            y_prev = dots[i][0].y;
            x_prev = dots[i][0].x;
//            double y_p = Params.f(Params.xB, z);
//            transform(Params.xB, y_p, z, tetax, tetay, tetaz, x_prev, y_prev, painter.window().width(), painter.window().height());
            EdgeWhatcher(x_prev, y_prev, x_left, y_left, TOP, DOWN, painter);
            int Pflag = Visible(x_prev, y_prev, TOP, DOWN);
//            for (double x = Params.xB; x <= Params.xE; x += Params.xD) {
            for (int j = 0; j < dots[i].size(); j++) {
                int x_curr = 0, y_curr = 0;
                int xi;
                int yi;
                x_curr = dots[i][j].x;
                y_curr = dots[i][j].y;
//                y_p = Params.f(x, z);
//                transform(x, y_p, z, tetax, tetay, tetaz, x_curr, y_curr, painter.window().width(), painter.window().height());
                int Tflag = Visible(x_curr, y_curr, TOP, DOWN);
                if (Tflag == Pflag) {
                    if (Pflag) {
                        horizon(x_prev, y_prev, x_curr, y_curr, TOP, DOWN, painter);
                    }
                } else if (Tflag == 0) {
                    if (Pflag == 1) {
                        SearchIntersection(x_prev, y_prev, x_curr, y_curr, TOP, xi, yi);
                    } else {
                        SearchIntersection(x_prev, y_prev, x_curr, y_curr, DOWN, xi, yi);
                    }
                    horizon(x_prev, y_prev, xi, yi, TOP, DOWN, painter);
                }
                else if (Tflag == 1) {
                    if (Pflag == 0) {
                        SearchIntersection(x_prev, y_prev, x_curr, y_curr, TOP, xi, yi);
                        horizon(x_prev, y_prev, xi, yi, TOP, DOWN, painter);
                    } else {
                        SearchIntersection(x_prev, y_prev, x_curr, y_curr, TOP, xi, yi);
                        horizon(x_prev, y_prev, xi, yi, TOP, DOWN, painter);
                        SearchIntersection(x_prev, y_prev, x_curr, y_curr, DOWN, xi, yi);
                        horizon(xi, yi, x_curr, y_curr, TOP, DOWN, painter);
                    }
                } else {
                    if (Pflag == 0) {
                        SearchIntersection(x_prev, y_prev, x_curr, y_curr, DOWN, xi, yi);
                        horizon(x_prev, y_prev, xi, yi, TOP, DOWN, painter);
                    } else {
                        SearchIntersection(x_prev, y_prev, x_curr, y_curr, TOP, xi, yi);
                        horizon(x_prev, y_prev, xi, yi, TOP, DOWN, painter);
                        SearchIntersection(x_prev, y_prev, x_curr, y_curr, DOWN, xi, yi);
                        horizon(xi, yi, x_curr, y_curr, TOP, DOWN, painter);
                    }
                }
                Pflag = Tflag;
                x_prev = x_curr;
                y_prev = y_curr;
            }
            EdgeWhatcher(x_prev, y_prev, x_right, y_right, TOP, DOWN, painter);

        }
    }
}
