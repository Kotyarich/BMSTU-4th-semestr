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
    void changeClipper();

protected:
    void mousePressEvent(QMouseEvent *event);  

private:
    Ui::Widget *ui;
    DrawLabel *_draw_label;
    QPixmap _pixmap;
    bool _is_clicked;
    bool _is_right_clicked;
    QPoint _first_point;
    QRect _clipper;
    std::vector<QLine> _lines;

    int getCode(const QPoint &p, int xl, int xr, int yt, int yb);
    QPoint getIntersection(QLine &line, int x_left, int x_right, int y_top, int y_down, bool first);
    void clipLine(QLine &line, int x_left, int x_right, int y_top, int y_down, QPainter &painter);
    void checkNumber(int &number, QPoint &p1r, QPoint &p2r, QPoint &p1, QPoint &p2, QPoint &p);

    void render();
    QColor getClipperColor();
    QColor getLinesColor();
    QColor getClippedLinesColor();
};

#endif // WIDGET_H
