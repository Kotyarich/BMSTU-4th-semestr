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
    _is_right_clicked(false),
    _first(false),
    _is_closed(false)
{
    ui->setupUi(this);

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

double scalarMultiplication(QPointF p1, QPointF p2) {
    return p1.x() * p2.x() + p1.y() * p2.y();
}

void Widget::clipLine(QLine &line, int direction, QPainter &painter) {
    QPoint p1 = line.p1();
    QPoint p2 = line.p2();

    QPoint d(p2.x() - p1.x(), p2.y() - p1.y());
    double tb = 0;
    double tu = 1;

    auto edges = pointsToEdges();

    for (size_t i = 0; i < edges.size(); i++) {
        QPoint w(p1.x() - edges[i].p1().x(), p1.y() - edges[i].p1().y());
        QPointF n;
        n.setX(-direction * (edges[i].p2().y() - edges[i].p1().y()));
        n.setY(direction * (edges[i].p2().x() - edges[i].p1().x()));

        auto dScalar = scalarMultiplication(d, n);
        auto wScalar = scalarMultiplication(w, n);

        if (dScalar == 0.) { // line || edge
            if (wScalar < 0) {
                return; // line is invisible
            }
        } else {
            double t = - wScalar / dScalar;
            if (dScalar > 0) {
                if (t > 1) {
                    return;
                }
                tb = std::max(tb, t);
            } else {
                if (t < 0) {
                    return;
                }
                tu = std::min(tu, t);
            }
        }
    }

    if (tb <= tu) {
        painter.drawLine(int(std::round(p1.x() + (p2.x() - p1.x()) * tu)),
                         int(std::round(p1.y() + (p2.y() - p1.y()) * tu)),
                         int(std::round(p1.x() + (p2.x() - p1.x()) * tb)),
                         int(std::round(p1.y() + (p2.y() - p1.y()) * tb)));
    }
}

void Widget::clip()
{
    int direction = checkClipper();
    if (!direction) {
        QMessageBox::critical(nullptr, "Error", "Отсекатель должен быть выпуклым");
        return;
    }

    QPainter painter(&_pixmap);
    painter.setPen(getClippedLinesColor());

    for (auto &line: _lines) {
        clipLine(line, direction, painter);
    }

    _draw_label->update();
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
        ui->x_5->setValue(event->x() - 350);
        ui->y_4->setValue(event->y() - 12);
        changeClipper();
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

int sign(int x) {
    if (!x) {
        return 0;
    }
    if (x < 0) {
        return -1;
    }
    return 1;
}

int Widget::checkClipper()
{
    int f = 1;

    auto v0 = _clipper_points[_clipper_points.size() - 1];
    auto vi = _clipper_points[0];
    auto vn = _clipper_points[1];

    int x1 = vi.x() - v0.x();
    int y1 = vi.y() - v0.y();
    int x2 = vn.x() - vi.x();
    int y2 = vn.y() - vi.y();

    int r = x1 * y2 - x2 * y1;
    int prev = sign(r);
    int cur = 0;

    std::cout << _clipper_points.size() << std::endl;
    for (size_t i = 1; i < _clipper_points.size() - 1 && f; i++) {
        v0 = _clipper_points[i - 1];
        vi = _clipper_points[i];
        vn = _clipper_points[i + 1];

        int x1 = vi.x() - v0.x();
        int y1 = vi.y() - v0.y();
        int x2 = vn.x() - vi.x();
        int y2 = vn.y() - vi.y();

        int r = x1 * y2 - x2 * y1;
        cur = sign(r);
        std::cout << i - 1 << "; " << i << "; " << i + 1 << std::endl;
        std::cout <<  prev << " " << cur << std::endl;

        if (cur != prev) {
            f = 0;
        }
        prev = cur;
    }

    v0 = _clipper_points[_clipper_points.size() - 2];
    vi = _clipper_points[_clipper_points.size() - 1];
    vn = _clipper_points[0];

    x1 = vi.x() - v0.x();
    y1 = vi.y() - v0.y();
    x2 = vn.x() - vi.x();
    y2 = vn.y() - vi.y();

    r = x1 * y2 - x2 * y1;
    cur = sign(r);

    if (cur != prev) {
        f = 0;
    }

    return f * cur;
}

std::vector<QLine> Widget::pointsToEdges()
{
    std::vector<QLine> result;

    for (size_t i = 1; i < _clipper_points.size(); i++) {
        result.emplace_back(_clipper_points[i - 1], _clipper_points[i]);
    }
    result.emplace_back(_clipper_points[_clipper_points.size() - 1], _clipper_points[0]);

    return result;
}

void Widget::render()
{
    _pixmap.fill();

    QPainter painter(&_pixmap);

    painter.setPen(getClipperColor());
    for (size_t i = 1; i < _clipper_points.size(); i++) {
        painter.drawLine(_clipper_points[i - 1], _clipper_points[i]);
    }
    if (_is_closed) {
        painter.drawLine(_clipper_points[0], _clipper_points[_clipper_points.size() - 1]);
    }

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
    _is_closed = false;
    _first = false;
    _draw_label->update();
    _clipper_points.clear();
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
    const int x1 = ui->x_5->value();
    const int y1 = ui->y_4->value();

    if (_is_closed) {
        removeClipper();
    }

    _clipper_points.emplace_back(x1, y1);
    render();
}

void Widget::closeClipper()
{
    if (!_is_closed) {
        _is_closed = true;
        render();
    }
}

void Widget::removeClipper()
{
    _is_closed = false;
    _first = false;
    _clipper_points.clear();
    render();
}
