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
    std::vector<QLine> _lines;
    std::vector<QPoint> _clipper_points;
    bool _is_closed;

    int checkClipper();
    std::vector<QLine> pointsToEdges();
    void clipLine(QLine &line, int direction, QPainter &painter);

    void render();
    QColor getClipperColor();
    QColor getLinesColor();
    QColor getClippedLinesColor();
};

#endif // WIDGET_H
