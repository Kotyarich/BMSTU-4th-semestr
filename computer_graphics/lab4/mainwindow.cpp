#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spinBox->setRange(-1000000, 1000000);
    ui->spinBox_2->setRange(-1000000, 1000000);
    ui->spinBox_3->setRange(0, 1000000);
    ui->spinBox_4->setRange(0, 1000000);
    ui->spinBox_5->setRange(0, 1000000);
    ui->spinBox_6->setRange(0, 1000000);
    ui->spinBox_7->setRange(0, 1000000);
    ui->spinBox_8->setRange(-1000000, 1000000);
    ui->spinBox_9->setRange(-1000000, 1000000);
    ui->spinBox_10->setRange(-1000000, 1000000);
    ui->spinBox_11->setRange(-1000000, 1000000);
    ui->spinBox_12->setRange(0, 255);
    ui->spinBox_13->setRange(0, 255);
    ui->spinBox_14->setRange(0, 255);
    ui->spinBox_15->setRange(0, 1000000);
    ui->spinBox_16->setRange(0, 1000000);
    ui->spinBox_17->setRange(0, 1000000);
    ui->spinBox_18->setRange(0, 1000000);
    ui->spinBox_19->setRange(0, 1000000);
    ui->spinBox_20->setRange(0, 1000000);

    ui->radioButton_5->setChecked(true);

    _drawLabel = new DrawLabel(_pixmap);
    _drawLabel->resize(width() - 650, height());
    ui->centralWidget->layout()->addWidget(_drawLabel);
    _pixmap = QPixmap(_drawLabel->width(), _drawLabel->height());
    _pixmap.fill();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _drawLabel;
}

void MainWindow::drawCircle()
{
    int x = ui->spinBox->value();
    int y = ui->spinBox_2->value();
    int r = ui->spinBox_3->value();

    _drawCircle(x, y, r);
}

void MainWindow::drawEllipse()
{
    int x = ui->spinBox_8->value();
    int y = ui->spinBox_9->value();
    int a = ui->spinBox_10->value();
    int b = ui->spinBox_11->value();

    _drawEllipse(x, y, a, b);
}

void MainWindow::drawSpectr()
{
    int r, dr, n;
    int err = getParams(r, dr, n);
    if (err < 0) {
       QMessageBox::critical(nullptr, "Error", "Введите 3 значения из 4х. 4ое оставьте ранвым 0");
       return;
    }

    auto color = getColor();
    auto alg = getAlgType();

    QPainter painter(&_pixmap);
    painter.setPen(color);

    int x_c = _pixmap.width() / 2;
    int y_c = _pixmap.height() / 2;
    if (alg == AlgType::standard) {
        for (int i = 0; i < n; i++, r += dr) {
            painter.drawEllipse(QPoint(x_c, y_c), r, r);
        }
    } else {
        painter.end();
        for (int i = 0; i < n; i++, r += dr) {
            _drawCircle(x_c, y_c, r);
        }
    }

    _drawLabel->update();
}

void MainWindow::drawEllipseSpectr()
{
    int a, b, da, db, n;
    int err = getEllipseParams(a, b, da, db, n);
    if (err < 0) {
        QMessageBox::critical(nullptr, "Error", "Неиспользуемое значения должны остаться равными 0");
        return;
    }

    auto color = getColor();
    auto alg = getAlgType();


    QPainter painter(&_pixmap);
    painter.setPen(color);

    int x_c = _pixmap.width() / 2;
    int y_c = _pixmap.height() / 2;
    if (alg == AlgType::standard) {
        for (int i = 0; i < n; i++, a += da, b += db) {
            painter.drawEllipse(QPoint(x_c, y_c), a, b);
        }
    } else {
        painter.end();
        for (int i = 0; i < n; i++, a += da, b += db) {
            _drawEllipse(x_c, y_c, a, b);
        }
    }

    _drawLabel->update();
}

int MainWindow::getParams(int & r, int & dr, int & n) {
    int rMin = ui->spinBox_4->value();
    int rMax = ui->spinBox_5->value();
    int dR = ui->spinBox_7->value();
    int N = ui->spinBox_6->value();

    if (rMax <= 0) {
        if (!(rMin && dR && N)) {
            return -1;
        }

        r = rMin;
        dr = dR;
        n = N;
    } else if (rMin <= 0) {
        if (!(rMax && dR && N)) {
            return -1;
        }

        r = rMax - dR * N;
        dr = dR;
        n = N;
    } else if (dR <= 0) {
        if (!(rMin && rMax && N)) {
            return -1;
        }

        r = rMin;
        dr = int(std::ceil(double(rMax - rMin) / N));
        n = N;
    } else if (N <= 0) {
        if (!(rMin && dR && rMax)) {
            return -1;
        }

        r = rMin;
        dr = dR;
        n = (rMax - rMin) / dR;
    } else {
        return -1;
    }

    return 0;
}

int MainWindow::getEllipseParams(int & a, int &b, int & dA, int & dB, int & n) {
    int aMin = ui->spinBox_15->value();
    int aMax = ui->spinBox_17->value();
    int bMin = ui->spinBox_19->value();
    int bMax = ui->spinBox_20->value();
    int da = ui->spinBox_16->value();
    int N = ui->spinBox_18->value();

    if (aMax <= 0 && bMax <= 0) {
        if (!(aMin && da && N && bMin)) {
            return -1;
        }

        a = aMin;
        dA = da;
        b = bMin;
        dB = da * b / a;
        n = N;
    } else if (aMin <= 0) {
        if (!(aMax && da && N && bMax)) {
            return -1;
        }

        a = aMax - da * N;
        dB = da * bMax / aMax;
        b = bMax - dB * N;
        dA = da;
        n = N;
    } else if (da <= 0) {
        if (!(aMin && aMax && N && bMin)) {
            return -1;
        }

        a = aMin;
        b = bMin;
        dA = (aMax - aMin) / N;
        dB = dA * b / a;
        n = N;
    } else if (N <= 0) {
        if (!(aMin && da && aMax)) {
            return -1;
        }

        a = aMin;
        b = bMin;
        dA = da;
        n = (aMax - aMin) / da;
        dB = da * b / a;
    } else {
        return -1;
    }

    return 0;
}

void MainWindow::_drawCircle(const int x, const int y, const int r)
{
    auto color = getColor();
    auto alg = getAlgType();

    QPainter painter(&_pixmap);
    painter.setPen(color);

    std::vector<QPoint> points;

    switch (alg) {
    case AlgType::canon:
        points = getCanonCircle(x, y, r);
        break;
    case AlgType::param:
        points = getParamCircle(x, y, r);
        break;
    case AlgType::brez:
        points = getBrezCircle(x, y, r);
        break;
    case AlgType::midPoint:
        points = getMiddleCircle(x, y, r);
        break;
    case AlgType::standard:
        painter.drawEllipse(QPoint(x, y), r, r);
    }

    if (alg != AlgType::standard) {
        for (auto point: points) {
            painter.drawPoint(point);
        }
    }

    _drawLabel->update();
}

void MainWindow::_drawEllipse(const int x, const int y, const int a, const int b)
{
    auto color = getColor();
    auto alg = getAlgType();


    QPainter painter(&_pixmap);
    painter.setPen(color);

    std::vector<QPoint> points;

    switch (alg) {
    case AlgType::canon:
        points = getCanonEllipse(x, y, a, b);
        break;
    case AlgType::param:
        points = getParamEllipse(x, y, a, b);
        break;
    case AlgType::brez:
        points = getBrezEllipse(x, y, a, b);
        break;
    case AlgType::midPoint:
        points = getMiddleEllipse(x, y, a, b);
        break;
    case AlgType::standard:
        painter.drawEllipse(QPoint(x, y), a, b);
    }

    if (alg != AlgType::standard) {
        for (auto point: points) {
            painter.drawPoint(point);
        }
    }

    _drawLabel->update();
}

void MainWindow::clear()
{
    _pixmap.fill();
    _drawLabel->update();
}

AlgType MainWindow::getAlgType()
{
    if (ui->radioButton->isChecked()) {
        return AlgType::canon;
    }
    if (ui->radioButton_2->isChecked()) {
        return AlgType::param;
    }
    if (ui->radioButton_3->isChecked()) {
        return AlgType::brez;
    }
    if (ui->radioButton_4->isChecked()) {
        return AlgType::midPoint;
    }
    if (ui->radioButton_5->isChecked()) {
        return AlgType::standard;
    }

    assert(true);
}

QColor MainWindow::getColor()
{
    int r = ui->spinBox_12->value();
    int g = ui->spinBox_13->value();
    int b = ui->spinBox_14->value();

    return QColor(r, g, b);
}

void MainWindow::getBgColor()
{
    ui->spinBox_12->setValue(255);
    ui->spinBox_13->setValue(255);
    ui->spinBox_14->setValue(255);
}
