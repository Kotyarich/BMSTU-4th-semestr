#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QLabel>
#include <cassert>
#include "drawlabel.h"
#include "algs.h"

enum class AlgType
{
    canon,
    param,
    brez,
    midPoint,
    standard
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void drawCircle();
    void drawEllipse();
    void drawSpectr();
    void drawEllipseSpectr();
    void clear();
    void getBgColor();

private:
    QPixmap _pixmap;
    Ui::MainWindow *ui;
    QLabel * _drawLabel;

    AlgType getAlgType();
    QColor getColor();
    int getParams(int & r, int & dr, int & n);
    int getEllipseParams(int & a, int &b, int & dA, int & dB, int & n);

    void _drawCircle(const int x, const int y, const int r);
    void _drawEllipse(const int x, const int y, const int a, const int b);
};

#endif // MAINWINDOW_H
