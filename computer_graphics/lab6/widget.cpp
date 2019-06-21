#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <QMessageBox>
#include <QMutex>
#include <QWaitCondition>
#include <cmath>
#include <stack>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    start_point_number(-1)
{
    ui->setupUi(this);

    ui->r->setRange(0, 255);
    ui->g->setRange(0, 255);
    ui->b->setRange(0, 255);
    ui->r->setValue(0);
    ui->g->setValue(255);
    ui->b->setValue(0);

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

void Widget::fill()
{
    auto edge_color = QColor(0, 0, 0);
    auto fill_color = getColor();

    QPainter painter(&_pixmap);
    painter.setPen(fill_color);
    std::stack<QPoint> stack;
    stack.push(QPoint(ui->x_2->value(), ui->y_2->value()));

    while (!stack.empty()) {
        auto image = _pixmap.toImage();
        auto z_point = stack.top();
        stack.pop();

        bool f = false;
        int x = z_point.x();
        int y = z_point.y();
        painter.drawPoint(x, y);

        x--;
        while (image.pixelColor(x, y) != edge_color) {
            painter.drawPoint(x, y);
            x--;
        }

        int x_left = x + 1;

        x = z_point.x() + 1;
        while (image.pixelColor(x, y) != edge_color) {
            painter.drawPoint(x, y);
            x++;
        }
        int x_right = x - 1;

        x = x_left;
        y++;
        while (x <= x_right) {
            f = false;
            auto pixColor = image.pixelColor(x, y);
            while (pixColor != edge_color && pixColor != fill_color) {
                f = true;
                pixColor = image.pixelColor(++x, y);
            }

            if (f) {
                pixColor = image.pixelColor(x, y);
                if (x == x_right && pixColor != edge_color && pixColor != fill_color) {
                    stack.push(QPoint(x, y));
                } else {
                    stack.push(QPoint(x - 1, y));
                }
                f = false;
            }

            int x_enter = x;
            pixColor = image.pixelColor(x, y);
            while ((pixColor == edge_color || pixColor == fill_color) && x < x_right) {
                x++;
                pixColor = image.pixelColor(x, y);
            }
            x = x_enter == x ? x + 1: x;
        }

        x = x_left;
        y -= 2;
        while (x <= x_right) {
            f = false;
            auto pixColor = image.pixelColor(x, y);
            while (pixColor != edge_color && pixColor != fill_color) {
                f = true;
                pixColor = image.pixelColor(++x, y);
            }

            if (f) {
                pixColor = image.pixelColor(x, y);
                if (x == x_right && pixColor != edge_color && pixColor != fill_color) {
                    stack.push(QPoint(x, y));
                } else {
                    stack.push(QPoint(x - 1, y));
                }
                f = false;
            }

            int x_enter = x;
            pixColor = image.pixelColor(x, y);
            while ((pixColor == edge_color || pixColor == fill_color) && x++ < x_right) {
                x++;
                pixColor = image.pixelColor(x, y);
            }
            x = x_enter == x ? x + 1: x;
        }

        if (ui->delay->isChecked()) {
            _draw_label->update();
            _pause(3);
        }
    }

    _draw_label->update();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->x() < 350) {
        return;
    }

    if (event->button() == Qt::RightButton) {
        ui->x_2->setValue(event->x() - 350);
        ui->y_2->setValue(event->y() - 12);
    } else {
        bool h = QApplication::keyboardModifiers() == Qt::ShiftModifier? true: false;
        _addPoint(event->x() - 350, event->y() - 12, h);
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->x() < 350) {
        return;
    }

    _addPoint(event->x() - 350, event->y() - 12, false);
}

QColor Widget::getColor()
{
    int r = ui->r->value();
    int g = ui->g->value();
    int b = ui->b->value();

    return QColor(r, g, b);
}

void Widget::_addPoint(int x, int y, bool horizontal)
{
    const int count = ui->table->rowCount();
    ui->table->insertRow(count);

    if (horizontal && count) {
        y = ui->table->item(count - 1, 1)->text().toInt();
    }
    auto *x_item = new QTableWidgetItem(QString("").sprintf("%d", x));
    ui->table->setItem(count, 0, x_item);
    auto *y_item = new QTableWidgetItem(QString("").sprintf("%d", y));
    ui->table->setItem(count, 1, y_item);

    if (count && start_point_number != -1) {
        QPainter painter(&_pixmap);
        int x1 = ui->table->item(count - 1, 0)->text().toInt();
        int y1 = ui->table->item(count - 1, 1)->text().toInt();
        painter.drawLine(x1, y1, x, y);
        _draw_label->update();

        _edges.emplace_back(QPoint(x, y), QPoint(x1, y1));
    }

    start_point_number = start_point_number == -1? count: start_point_number;
}

void Widget::_pause(int times)
{
    // I don't know more adequate way to do it ¯\_(ツ)_/¯
    for (int i = 0; i < times; i++) {
        repaint();
        resize(width(), height());
    }
}

void Widget::clear()
{
    _pixmap.fill();
    start_point_number = -1;
    _edges.clear();
    _draw_label->update();
    ui->table->clear();
    while (ui->table->rowCount()) {
        ui->table->removeRow(0);
    }
}

void Widget::addDot()
{
    const int x = ui->x->value();
    const int y = ui->y->value();
    _addPoint(x, y, false);
}

void Widget::closeEdges()
{
    QPainter painter(&_pixmap);
    const int count = ui->table->rowCount();
    if (count < 3) {
        QMessageBox::critical(nullptr, "Error", "Необходимо сначала добавить хотя бы 2 ребра");
        return;
    }
    if (start_point_number < 0) {
        return;
    }

    int x0 = ui->table->item(start_point_number, 0)->text().toInt();
    int y0 = ui->table->item(start_point_number, 1)->text().toInt();
    int x1 = ui->table->item(count - 1, 0)->text().toInt();
    int y1 = ui->table->item(count - 1, 1)->text().toInt();
    painter.drawLine(x0, y0, x1, y1);

    start_point_number = -1;
    _edges.emplace_back(QPoint(x0, y0), QPoint(x1, y1));
    _draw_label->update();
}
