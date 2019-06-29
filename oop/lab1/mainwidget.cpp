#include <QtWidgets/QMessageBox>
#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::save()
{
    auto path = ui->saveFileEdit->text().toStdString();
    Command command{Command::save, (char *) path.c_str()};
    auto errCode = handleSignal(command);
    if (errCode) {
        QMessageBox::critical(nullptr,
                              "Ошибка",
                              "Неверный путь или некорректный файл",
                              QMessageBox::Ok | QMessageBox::Default
        );
        return;
    }
}

void MainWidget::load()
{
    auto path = ui->loadFileEdit->text().toStdString();
    Command command{Command::load};
    command.loadParams = {(char *) path.c_str()};
    auto errCode = handleSignal(command);    
    if (errCode) {
        QMessageBox::critical(nullptr,
                              "Ошибка",
                              "Неверный путь или некорректный файл",
                              QMessageBox::Ok | QMessageBox::Default
        );
        return;
    }
    update();
}

void MainWidget::move()
{
    bool ok = true;
    auto dx = ui->moveXEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::critical(nullptr,
                              "Ошибка",
                              "Сдвиг по оси Х должен быть целым числом",
                              QMessageBox::Ok | QMessageBox::Default
        );
        return;
    }
    auto dy = ui->moveYEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::critical(nullptr,
                              "Ошибка",
                              "Сдвиг по оси Y должен быть целым числом",
                              QMessageBox::Ok | QMessageBox::Default
        );
        return;
    }
    auto dz = ui->moveZEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::critical(nullptr,
                              "Ошибка",
                              "Сдвиг по оси Z должен быть целым числом",
                              QMessageBox::Ok | QMessageBox::Default
        );
        return;
    }

    Command command{Command::move};
    command.moveParams = {dx, dy, dz};
    handleSignal(command);
    update();
}

void MainWidget::scale()
{
    bool ok = true;
    auto kx = ui->xCoefEdit->text().toFloat(&ok);
    if (!ok) {
        QMessageBox::critical(
            nullptr, "Ошибка",
            "Коэффициент масштабирования по Х должен быть вещественным числом "
            "(разделитель - точка)",
            QMessageBox::Ok | QMessageBox::Default
        );
        return;
    }
    auto ky = ui->yCoefEdit->text().toFloat(&ok);
    if (!ok) {
        QMessageBox::critical(
            nullptr, "Ошибка",
            "Коэффициент масштабирования по Y должен быть вещественным числом "
            "(разделитель - точка)",
            QMessageBox::Ok | QMessageBox::Default
        );
        return;
    }
    auto kz = ui->zCoefEdit->text().toFloat(&ok);
    if (!ok) {
        QMessageBox::critical(
            nullptr, "Ошибка",
            "Коэффициент масштабирования по Z должен быть вещественным числом "
            "(разделитель - точка)",
            QMessageBox::Ok | QMessageBox::Default
        );
        return;
    }

    Command command{Command::scale};
    command.scaleParams = {kx, ky, kz};
    handleSignal(command);
    update();
}

void MainWidget::rotate()
{
    bool ok = true;
    auto xAngle = ui->xAngleEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::critical(
            nullptr, "Ошибка",
            "Угол поворота вокруг оси Х должен быть целым числом",
            QMessageBox::Ok | QMessageBox::Default
        );
        return;
    }
    auto yAngle = ui->yAngleEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::critical(
            nullptr, "Ошибка",
            "Угол поворота вокруг оси Y должен быть целым числом",
            QMessageBox::Ok | QMessageBox::Default
        );
        return;
    }
    auto zAngle = ui->zAngleEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::critical(
            nullptr, "Ошибка",
            "Угол поворота вокруг оси Z должен быть целым числом",
            QMessageBox::Ok | QMessageBox::Default
        );
        return;
    }

    Command command{Command::rotate};
    command.rotateParams = {xAngle, yAngle, zAngle};
    handleSignal(command);
    update();
}

void MainWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.drawLine(width() / 2, 0, width() / 2, height());
    painter.drawLine(0, height() / 2, width(), height() / 2);
    
    Command command{Command::draw};
    command.drawParams = {this, &painter};
    handleSignal(command);
}

void MainWidget::resizeEvent(QResizeEvent *)
{
    update();
}

void MainWidget::closeEvent(QCloseEvent * event)
{
    handleSignal({Command::free});
    QWidget::closeEvent(event);
}
