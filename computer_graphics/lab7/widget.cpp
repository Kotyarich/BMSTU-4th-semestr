#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <QMessageBox>
#include <QMutex>
#include <QWaitCondition>
#include <cmath>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    _is_clicked(false),
    _is_right_clicked(false)
{
    ui->setupUi(this);

    _clipper = QRect(1, 1, 1, 1);
    _draw_label = new DrawLabel(_pixmap);
    _draw_label->resize(width() - 0, height());
    layout()->addWidget(_draw_label);
    _pixmap = QPixmap(_draw_label->width(), _draw_label->height());
    _pixmap.fill();
}

Widget::~Widget()
{
    delete ui;
}


int Widget::getCode(const QPoint &p, int xl, int xr, int yt, int yb)
{
    int code = 0;

    if (p.x() < xl) {
        code |= 1 << 3;
    }
    if (p.x() > xr) {
        code |= 1 << 2;
    }
    if (p.y() > yb) {
        code |= 1 << 1;
    }
    if (p.y() < yt) {
        code |= 1;
    }

    return code;
}

QPoint Widget::getIntersection(QLine &line, int x_left, int x_right, int y_top, int y_down, bool first) {
    QPoint p1 = line.p1();
    QPoint p2 = line.p2();
    double m = 2000000; // just very big value
    QPoint p = first? line.p1() : line.p2();

    if (p2.x() - p1.x() != 0) { // line isnt vertical
        // looking for intersection with left side
        m = double(p2.y() - p1.y()) / (p2.x() - p1.x());
        if (x_left >= p.x()) {
            int y = int(std::round(m * (x_left - p.x()) + p.y()));
            if (y >= y_top and y <= y_down) {
                p.setY(y);
                p.setX(x_left);
                return p; // intersection was found
            }
        }
        // loking for intersection with right side
        if (x_right <= p.x()){
            int y = int(std::round(m * (x_right - p.x()) + p.y()));
            if (y >= y_top and y <= y_down) {
                p.setX(x_right);
                p.setY(y);
                return p; // intersection was found
            }
        }
    }
    // check if line is horisontal
    if (m == 0.) {
        return QPoint(-1000, -1000);
    }
    // looking for intersection with top side
    if (y_top >= p.y()) {
        int x = int(std::round((1 / m) * (y_top - p.y()) + p.x()));
        if (x >= x_left and x <= x_right) {
            p.setX(x);
            p.setY(y_top);
            return p; // intersection was found
        }
    }
    if (y_down > p.y()) {
        return QPoint(-1000, -1000); // error situation here, assert(false) can be used here
    }
    // looking for intersection with bottom side
    int x = int(std::round((1 / m) * (y_down - p.y()) + p.x()));
    if (x >= x_left and x <= x_right) {
        p.setX(x);
        p.setY(y_down);
        return p; // intersection was found
    }
    // line is invisiable
    return QPoint(-1000, -1000);
}

void Widget::clipLine(QLine &line, int x_left, int x_right, int y_top, int y_down, QPainter &painter) {
    QPoint p1 = line.p1();
    QPoint p2 = line.p2();

    int t1 = getCode(p1, x_left, x_right, y_top, y_down);
    int t2 = getCode(p2, x_left, x_right, y_top, y_down);

    QPoint p1_res = p1;
    QPoint p2_res = p2;

    if (!t1 and !t2) { // line is fully visiable
        painter.drawLine(line);
        return;
    }

    if ((t1 & t2) != 0) {
        return; // line is fully invisiable
    }

    if (t1 != 0) { // first point is inside the window
        p1_res = getIntersection(line, x_left, x_right, y_top, y_down, true);
    }
    if (t2 != 0) { // second point is inside the window
        p2_res = getIntersection(line, x_left, x_right, y_top, y_down, false);
    }

    if (p1_res.x() == -1000 || p2_res.x() == -1000) {
        return;
    }
    painter.drawLine(p1_res, p2_res);
}

void Widget::clip()
{
    QPainter painter(&_pixmap);
    painter.setPen(getClippedLinesColor());

    const int x_left = _clipper.x();
    const int x_right = x_left + _clipper.width();
    const int y_top = _clipper.y();
    const int y_down = y_top + _clipper.height();

    for (auto &line: _lines) {
        clipLine(line, x_left, x_right, y_top, y_down, painter);
        _draw_label->update();
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->x() < 350) {
        return;
    }

    bool h = QApplication::keyboardModifiers() == Qt::ShiftModifier? true: false;
    bool v = QApplication::keyboardModifiers() == Qt::ControlModifier? true: false;

    int x = event->x() - 350;
    int y = event->y() - 12;

    if (event->button() == Qt::RightButton) {
        if (!_is_right_clicked) {
            ui->x_5->setValue(event->x() - 350);
            ui->y_4->setValue(event->y() - 12);
            _is_right_clicked = true;
        } else {
            ui->x_4->setValue(event->x() - 350 - ui->x_5->value());
            ui->y_5->setValue(event->y() - 12 - ui->y_4->value());
            changeClipper();
        }
    } else {
        if (_is_clicked) {
            if (h) {
                y = _first_point.y();
            } else if (v) {
                x = _first_point.x();
            }
            _lines.emplace_back(_first_point, QPoint(x, y));
            render();
        } else {
            _first_point.setX(x);
            _first_point.setY(y);
        }
        _is_clicked = !_is_clicked;
    }
}

void Widget::render()
{
    _pixmap.fill();

    QPainter painter(&_pixmap);

    painter.setPen(getClipperColor());
    painter.drawRect(_clipper);

    painter.setPen(getLinesColor());
    for (auto &line: _lines) {
        painter.drawLine(line);
    }

    _draw_label->update();
}

QColor Widget::getLinesColor()
{
    int r = ui->r->value();
    int g = ui->g->value();
    int b = ui->b->value();

    return QColor(r, g, b);
}

QColor Widget::getClipperColor()
{
    int r = ui->r_2->value();
    int g = ui->g_2->value();
    int b = ui->b_2->value();

    return QColor(r, g, b);
}

QColor Widget::getClippedLinesColor()
{
    int r = ui->r_3->value();
    int g = ui->g_3->value();
    int b = ui->b_3->value();

    return QColor(r, g, b);
}

void Widget::clear()
{
    _pixmap.fill();
    _is_clicked = false;
    _draw_label->update();
    _clipper = QRect(1, 1, 1, 1);
    _lines.clear();
}

void Widget::addLine()
{
    const int x1 = ui->x->value();
    const int y1 = ui->y->value();
    const int x2 = ui->x_3->value();
    const int y2 = ui->y_3->value();

    _lines.emplace_back(x1, y1, x2, y2);
    render();
}

void Widget::changeClipper()
{
    const int x = ui->x_5->value();
    const int y = ui->y_4->value();
    const int w = ui->x_4->value();
    const int h = ui->y_5->value();

    _clipper = QRect(x, y, w, h);
    _is_right_clicked = false;
    render();
}
