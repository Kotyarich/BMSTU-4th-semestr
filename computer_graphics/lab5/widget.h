#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include "drawlabel.h"

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
    void fill();
    void clear();
    void addDot();
    void closeEdges();

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::Widget *ui;
    DrawLabel *_draw_label;
    QPixmap _pixmap;
    std::vector<std::pair<QPoint, QPoint>> _edges;
    QPoint _last_point;
    int start_point_number;

    QColor getColor();
    void _addPoint(int x, int y, bool horizontal);
    int _getMaxX();
    void _pause(int times);
};

#endif // WIDGET_H
