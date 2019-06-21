#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <set>
#include <map>
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
    _is_closed(false),
    _is_polygon_closed(false) {
    ui->setupUi(this);

    _draw_label = new DrawLabel(_pixmap);
    _draw_label->resize(width() - 0, height());
    layout()->addWidget(_draw_label);
    _pixmap = QPixmap(_draw_label->width(), _draw_label->height());
    _pixmap.fill();
}

Widget::~Widget() {
    delete ui;
}

double scalarMultiplication(QPointF p1, QPointF p2) {
    return p1.x() * p2.x() + p1.y() * p2.y();
}

void printVector(std::vector<QPoint> &v) {
    for (auto &p: v) {
        std::cout << p.x() << "; " << p.y() << std::endl;
    }
    std::cout << "--------------------------" << std::endl;
}

std::vector<QLine> convertToLines(std::vector<QPoint> points) {
    std::vector<QLine> res;
    for (int i = 1; i < points.size(); i++) {
        res.emplace_back(points[i - 1], points[i]);
    }
    res.emplace_back(points[points.size() - 1], points[0]);
    return res;
}

std::vector<QLine> Widget::findExtraLines(std::vector<QPoint> points) {
    auto edges = convertToLines(points);
    std::vector<QLine> res;
    for (int i = 0; i < edges.size(); i++) {
        for (int j = 0; j < edges.size(); j++) {
            if (i == j) {
                continue;
            }
            auto p1 = edges[i].p1();
            auto p2 = edges[i].p2();
            auto q1 = edges[j].p1();
            auto q2 = edges[j].p2();

            if (p1.x() > p2.x()) {
                std::swap(p1, p2);
            }
            if (q1.x() > q2.x()) {
                std::swap(q1, q2);
            }

            double dy = p2.y() - p1.y();
            if (dy == 0) {
                dy = 1e-15;
            }
            double tg1 = (p2.x() - p1.x()) / dy;
            dy = q2.y() - q1.y();
            if (dy == 0) {
                dy = 1e-15;
            }
            double tg2 = (q2.x() - q1.x()) / dy;
            std::cout << tg1 << " " << tg2 << std::endl;
            if (std::abs(tg2 - tg1) > 1e-1) {
                continue;
            }

            if (q1.x() - p1.x() == 0 && q2.x() - p2.x() == 0) {
                if (q1.y() >= p1.y() && q2.y() <= p2.y()) {
                    res.push_back(edges[i]);
                } else if (p1.y() >= q1.y() && p2.y() <= q2.y()) {
                    res.push_back(edges[j]);
                }
            } else {
                if (q1.x() >= p1.x() && q2.x() <= p2.x()) {
                    res.push_back(edges[j]);
                } else if (p1.x() >= q1.x() && p2.x() <= q2.x()) {
                    res.push_back(edges[i]);
                }
            }
        }
    }

    return res;
}

std::vector<QPoint> Widget::clipPolygon(int direction) {
    std::vector<QPoint> result;
    std::vector<QPoint> polygon_points(_polygon_points.size());  
    std::copy(_polygon_points.begin(), _polygon_points.end(), polygon_points.begin());
    std::vector<QPoint> qlipper_points(_clipper_points.size());
    std::copy(_clipper_points.begin(), _clipper_points.end(), qlipper_points.begin());
    qlipper_points.push_back(qlipper_points[0]);
    QPoint first_point;
    QPoint start_point;

    for (size_t i = 0; i < qlipper_points.size() - 1; i++) {
        result.clear();
        for (size_t j = 0; j < polygon_points.size(); j++) {
            if (j == 0) {
                first_point = polygon_points[j];
            } else {
                bool check_intersection = checkIntersection(start_point, polygon_points[j],
                                                            qlipper_points[i], qlipper_points[i + 1]);
                ;
                if (check_intersection) {
                    QPoint intersection = intersect(start_point, polygon_points[j],
                                                    qlipper_points[i], qlipper_points[i + 1]);
                    result.push_back(intersection);
                }
            }

            start_point = polygon_points[j];
            int is_visible = isVisible(start_point, qlipper_points[i], qlipper_points[i + 1]);
            if ((is_visible > 0 && direction == -1) || (is_visible < 0 && direction == 1)) {
                result.push_back(start_point);
            }
        }

        if (result.size() > 0) {
            bool check_intersection = checkIntersection(start_point, first_point,
                                                        qlipper_points[i], qlipper_points[i + 1]);
            if (check_intersection) {
                QPoint intersection = intersect(start_point, first_point,
                                                qlipper_points[i], qlipper_points[i + 1]);
                result.push_back(intersection);
            }
        }

        polygon_points.resize(result.size());
        std::copy(result.begin(), result.end(), polygon_points.begin());
        printVector(polygon_points);
        result.clear();
    }

    return polygon_points;
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

bool Widget::checkIntersection(QPoint &sp, QPoint &ep, QPoint &p0, QPoint &p1) {
    int is_sp_visible = isVisible(sp, p0, p1);
    int is_ep_visible = isVisible(ep, p0, p1);

    if ((is_sp_visible < 0 and is_ep_visible > 0)
        or (is_sp_visible > 0 and is_ep_visible < 0)) {
        return true;
    }

    return false;
}

int Widget::isVisible(QPoint &p, QPoint &p1, QPoint &p2) {
    return sign(((p.x() - p1.x()) * (p2.y() - p1.y())) - ((p.y() - p1.y()) * (p2.x() - p1.x())));
}

QPoint Widget::intersect(QPoint &p1, QPoint &p2, QPoint &cp1, QPoint &cp2) {
    int k_1_1 = p2.x() - p1.x();
    int k_1_2 = cp1.x() - cp2.x();
    int k_2_1 = p2.y() - p1.y();
    int k_2_2 = cp1.y() - cp2.y();
    int right1 = cp1.x() - p1.x();
    int right2 = cp1.y() - p1.y();

    int det = k_1_1 * k_2_2 - k_1_2 * k_2_1;
    double t = double(right1 * k_2_2 - k_1_2 * right2) / det;

    int x = int(std::round(p1.x() + (p2.x() - p1.x()) * t));
    int y = int(std::round(p1.y() + (p2.y() - p1.y()) * t));

    return { x, y };
}

void Widget::clip() {
    int direction = checkClipper();
    if (!direction) {
        QMessageBox::critical(nullptr, "Error", "Отсекатель должен быть выпуклым");
        return;
    }

    if (_polygon_points.size() < 3 || _clipper_points.size() < 3) {
        return;
    }

    QPainter painter(&_pixmap);
    painter.setPen(QPen(getClippedLinesColor(), 2));
    std::vector<QPoint> result = clipPolygon(direction);

    for (size_t i = 1; i < result.size(); i++) {
        painter.drawLine(result[i - 1], result[i]);
    }
    if (result.size() > 1) {
        painter.drawLine(result[result.size() - 1], result[0]);
    }

//    painter.setPen(QPen(Qt::green, 3));
//    auto lines = findExtraLines(result);
//    for (auto line : lines) {
//        painter.drawLine(line);
//    }

    _draw_label->update();
}

void Widget::mousePressEvent(QMouseEvent *event) {
    if (event->x() < 350) {
        return;
    }

    bool h = QApplication::keyboardModifiers() == Qt::ShiftModifier? true: false;
    bool v = QApplication::keyboardModifiers() == Qt::ControlModifier? true: false;

    int x = event->x() - 350;
    int y = event->y() - 12;

    if (event->button() == Qt::RightButton) {
        ui->x_5->setValue(x);
        ui->y_4->setValue(y);
        changeClipper();
    } else {
        if (h) {
            y = _first_point.y();
        } else if (v) {
            x = _first_point.x();
        }
        _first_point.setX(x);
        _first_point.setY(y);
        ui->x->setValue(x);
        ui->y->setValue(y);
        addLine();
    }
}

int Widget::checkClipper() {
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

void Widget::render() {
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
    for (size_t i = 1; i < _polygon_points.size(); i++) {
        painter.drawLine(_polygon_points[i - 1], _polygon_points[i]);
    }
    if (_is_polygon_closed) {
        painter.drawLine(_polygon_points[0], _polygon_points[_polygon_points.size() - 1]);
    }

    _draw_label->update();
}

QColor Widget::getLinesColor() {
    int r = ui->r->value();
    int g = ui->g->value();
    int b = ui->b->value();

    return QColor(r, g, b);
}

QColor Widget::getClipperColor() {
    int r = ui->r_2->value();
    int g = ui->g_2->value();
    int b = ui->b_2->value();

    return QColor(r, g, b);
}

QColor Widget::getClippedLinesColor() {
    int r = ui->r_3->value();
    int g = ui->g_3->value();
    int b = ui->b_3->value();

    return QColor(r, g, b);
}

void Widget::clear() {
    _pixmap.fill();
    _is_clicked = false;
    _is_closed = false;
    _is_polygon_closed = false;
    _first = false;
    _draw_label->update();
    _clipper_points.clear();
    _polygon_points.clear();
}

void Widget::addLine() {
    const int x = ui->x->value();
    const int y = ui->y->value();

    if (_is_polygon_closed) {
        removePolygon();
    }

    _polygon_points.emplace_back(x, y);
    render();
}

void Widget::closePolygon() {
    if (!_is_polygon_closed) {
        _is_polygon_closed = true;
        render();
    }
}

void Widget::removePolygon() {
    _is_polygon_closed = false;
    _first = false;
    _polygon_points.clear();
    render();
}

void Widget::changeClipper() {
    const int x1 = ui->x_5->value();
    const int y1 = ui->y_4->value();

    if (_is_closed) {
        removeClipper();
    }

    _clipper_points.emplace_back(x1, y1);
    render();
}

void Widget::closeClipper() {
    if (!_is_closed) {
        _is_closed = true;
        render();
    }
}

void Widget::removeClipper() {
    _is_closed = false;
    _first = false;
    _clipper_points.clear();
    render();
}
