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
    void clip();
    void clear();
    void addLine();
    void closePolygon();
    void removePolygon();
    void changeClipper();
    void closeClipper();
    void removeClipper();

protected:
    void mousePressEvent(QMouseEvent *event);  

private:
    Ui::Widget *ui;
    DrawLabel *_draw_label;
    QPixmap _pixmap;
    bool _is_clicked;
    bool _is_right_clicked;
    bool _first;
    QPoint _first_point;
    std::vector<QPoint> _polygon_points;
    std::vector<QPoint> _clipper_points;
    bool _is_closed;
    bool _is_polygon_closed;

    int checkClipper();
    std::vector<QPoint> clipPolygon(int);
    bool checkIntersection(QPoint &sp, QPoint &ep, QPoint &p0, QPoint &p1);
    int isVisible(QPoint &p, QPoint &p1, QPoint &p2);
    QPoint intersect(QPoint &p1, QPoint &p2, QPoint &cp1, QPoint &cp2);
    std::vector<QLine> findExtraLines(std::vector<QPoint> points);

    void render();
    QColor getClipperColor();
    QColor getLinesColor();
    QColor getClippedLinesColor();
};

#endif // WIDGET_H
