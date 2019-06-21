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
    start_point_number(-1)
{
    ui->setupUi(this);

    ui->r->setRange(0, 255);
    ui->g->setRange(0, 255);
    ui->b->setRange(0, 255);
    ui->r->setValue(255);
    ui->g->setValue(0);
    ui->b->setValue(0);

    _draw_label = new DrawLabel(_pixmap);
    _draw_label->resize(width() - 0, height());
    layout()->addWidget(_draw_label);
    _pixmap = QPixmap(_draw_label->width(), _draw_label->height());
    QColor color(60, 60, 60);
    _pixmap.fill(color);
}

Widget::~Widget()
{
    delete ui;
}



void Widget::fill()
{
    auto bg_color = QColor(60, 60, 60);
    auto fill_color = getColor();

    QPainter painter(&_pixmap);
    int x_max = _getMaxX();

    for (auto edge: _edges) {
        // skip horisontal edges
        if (edge.first.y() == edge.second.y()) {
            continue;
        }
        // watching all edges from top to down
        if (edge.first.y() > edge.second.y()) {
            std::swap(edge.first, edge.second);
        }

        double x_start = edge.first.x();
        double dx = double(edge.second.x() - edge.first.x())
                / double(edge.second.y() - edge.first.y());

        auto image = _pixmap.toImage();
        for (int y = edge.first.y(); y < edge.second.y(); y++) {
            for (int x = int(std::round(x_start + 0.5)); x < x_max; x++) {
                auto color = image.pixelColor(x, y);
                if (color == bg_color) {
                    painter.setPen(fill_color);               
                } else {
                    painter.setPen(bg_color);
                }

                painter.drawPoint(x, y);
            }

            x_start += dx;
            if (ui->delay->isChecked()) {
                _draw_label->update();
                _pause(3);
            }
        }
    }

    _draw_label->update();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->x() < 330) {
        return;
    }

    bool h = QApplication::keyboardModifiers() == Qt::ShiftModifier? true: false;
    _addPoint(event->x() - 330, event->y() - 12, h);
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
        painter.setPen(Qt::white);
        int x1 = ui->table->item(count - 1, 0)->text().toInt();
        int y1 = ui->table->item(count - 1, 1)->text().toInt();
        painter.drawLine(x1, y1, x, y);
        _draw_label->update();

        _edges.emplace_back(QPoint(x, y), QPoint(x1, y1));
    }

    start_point_number = start_point_number == -1? count: start_point_number;
}

int Widget::_getMaxX()
{
    int x_max = -1;
    for (auto edge: _edges) {
        x_max = std::max(x_max, edge.first.x());
        x_max = std::max(x_max, edge.second.x());
    }
    return x_max;
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
    QColor color(60, 60, 60);
    _pixmap.fill(color);
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
    painter.setPen(Qt::white);
    const int count = ui->table->rowCount();
    if (count < 3) {
        QMessageBox::critical(0, "Error", "Необходимо сначала добавить хотя бы 2 ребра");
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
