#include "mainwindow.h"
#include <QFormLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setCentralWidget(initWidgets());

    _cabin = new Cabin();
    _controller = new Controller(_buttons);
    _controller->setCabin(_cabin);
    _watcher = new CabinWatcher(_labels, _cabin);
}

MainWindow::~MainWindow() {
    delete _cabin;
    delete _controller;
    delete _watcher;
}

QWidget* MainWindow::initWidgets() {
    auto layout = new QFormLayout();

    for (unsigned i = 0; i < _floors_number; ++i) {
        auto label = new QLabel(QString::number(i + 1));
        label->setStyleSheet("QLabel { background-color: white; border: 1px solid black; }");
        auto button = new QPushButton(QString("Вызвать ") + QString::number(i + 1));
        layout->addRow(label, button);
        _labels.push_back(label);
        _buttons.push_back(button);
        connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonSlot(bool)));
    }

    _labels[0]->setStyleSheet("QLabel { background-color: red; border: 1px solid black; }");

    auto wg = new QWidget();
    wg->setLayout(layout);
    return wg;
}

void MainWindow::buttonSlot(bool) {
    auto button = dynamic_cast<QPushButton*>(sender());
    const int buttonNumber = button->text()[button->text().size() - 1].digitValue();
    emit buttonPressedSignal(buttonNumber);

    _controller->addFloor(buttonNumber);
}
