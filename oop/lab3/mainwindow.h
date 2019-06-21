#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "cabin.h"
#include "controlpanel.h"
#include "CabinWatcher.h"

using lift::Cabin;
using lift::Controller;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    QWidget* initWidgets();

signals:
    void buttonPressedSignal(int floor);

private slots:
    void buttonSlot(bool);

private:
    Cabin *_cabin;
    Controller *_controller;
    CabinWatcher *_watcher;
    std::vector<QLabel*> _labels;
    std::vector<QPushButton*> _buttons;
    const int _floors_number = 9;
};

#endif // MAINWINDOW_H
