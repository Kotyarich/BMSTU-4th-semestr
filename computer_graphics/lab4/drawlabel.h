#ifndef DRAWLABEL_H
#define DRAWLABEL_H

#include <QObject>
#include <QLabel>
#include <QPainter>

class DrawLabel : public QLabel
{
    Q_OBJECT

public:
    DrawLabel(QPixmap & pixmap);
    void paintEvent(QPaintEvent * e);

private:
    QPixmap * _pixmap;
};

#endif // DRAWLABEL_H
