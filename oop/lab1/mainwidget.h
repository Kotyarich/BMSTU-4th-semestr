#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include "signalhandling.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

public slots:
    void save();
    void load();
    void move();
    void scale();
    void rotate();
private:
    Ui::MainWidget *ui;

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWIDGET_H
