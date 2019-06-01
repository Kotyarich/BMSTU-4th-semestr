#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>

Widget::Widget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::Widget),
    _facade(new ViewerFacade),
    _elements_num(0) {
    ui->setupUi(this);
    _draw_label = new DrawLabel(_pixmap);
    _draw_label->resize(width() - ui->frame->width(), height());
    layout()->addWidget(_draw_label);
    _pixmap = QPixmap(_draw_label->width(), _draw_label->height());
    _pixmap.fill();
}

Widget::~Widget() {
    delete _draw_label;
    delete ui;
}

void Widget::moveObject() {
    if (ui->comboBox->count() == 0) {
        return;
    }
    std::string obj_name = ui->comboBox->currentText().toStdString();
    int x = ui->spinBox->value();
    int y = ui->spinBox_2->value();
    int z = ui->spinBox_3->value();

    Point moving(x, y, z);
    if (obj_name[0] == 'm') {
        Point rotateing(0, 0, 0);
        Point scale(1, 1, 1);
        commands::TransformModelCommand command(obj_name, moving, scale, rotateing);
        _facade->execCommand(&command);
    } else {
        commands::MoveCameraCommand command(obj_name, moving);
        _facade->execCommand(&command);
    }

    render();
}

void Widget::scaleObject() {
    if (ui->comboBox->count() == 0) {
        return;
    }
    std::string obj_name = ui->comboBox->currentText().toStdString();
    if (obj_name[0] == 'c') {
        QMessageBox::critical(nullptr, "Error", "Couldn't scal camera");
        return;
    }

    double x = ui->doubleSpinBox_7->value();
    double y = ui->doubleSpinBox_6->value();
    double z = ui->doubleSpinBox_5->value();

    Point moving(0, 0, 0);
    Point rotateing(0, 0, 0);
    Point scale(x, y, z);
    commands::TransformModelCommand command(obj_name, moving, scale, rotateing);
    _facade->execCommand(&command);

    render();
}

void Widget::rotateObjectX() {
    if (ui->comboBox->count() == 0) {
        return;
    }
    std::string obj_name = ui->comboBox->currentText().toStdString();
    double angle = ui->doubleSpinBox_4->value();

    if (obj_name[0] == 'c') {
        commands::RollCameraCommand command(obj_name, angle);
        _facade->execCommand(&command);
    } else {
        Point moving(0, 0, 0);
        Point rotateing(angle, 0, 0);
        Point scale(1, 1, 1);
        commands::TransformModelCommand command(obj_name, moving, scale, rotateing);
        _facade->execCommand(&command);
    }

    render();
}

void Widget::rotateObjectY() {
    if (ui->comboBox->count() == 0) {
        return;
    }
    std::string obj_name = ui->comboBox->currentText().toStdString();
    double angle = ui->doubleSpinBox_4->value();
    if (obj_name[0] == 'c') {
        commands::PitchCameraCommand command(obj_name, angle);
        _facade->execCommand(&command);
    } else {
        Point moving(0, 0, 0);
        Point rotateing(0, angle, 0);
        Point scale(1, 1, 1);
        commands::TransformModelCommand command(obj_name, moving, scale, rotateing);
        _facade->execCommand(&command);
    }

    render();
}

void Widget::rotateObjectZ() {
    if (ui->comboBox->count() == 0) {
        return;
    }
    std::string obj_name = ui->comboBox->currentText().toStdString();
    double angle = ui->doubleSpinBox_4->value();
    if (obj_name[0] == 'c') {
        commands::YawCameraCommand command(obj_name, angle);
        _facade->execCommand(&command);
    } else {
        Point moving(0, 0, 0);
        Point rotateing(0, 0, angle);
        Point scale(1, 1, 1);
        commands::TransformModelCommand command(obj_name, moving, scale, rotateing);
        _facade->execCommand(&command);
    }

    render();
}

void Widget::addModel() {
    QString file = QFileDialog::getOpenFileName(this,
                                                QString::fromUtf8("Открыть файл"),
                                                QDir::currentPath(),
                                                "text files (*.txt)");
    if (file.isEmpty())
        return;

    std::string model_name = std::string("model_") + std::to_string(++_elements_num);
    std::string file_name = file.toLocal8Bit().constData();
    commands::AddModelCommand command(file_name, model_name);
    try {
        _facade->execCommand(&command);
        render();
        ui->comboBox->addItem(model_name.c_str());
    } catch(exceptions::FileOpenException e) {
        QMessageBox::critical(nullptr, "Error", "Couldn't open file");
    } catch(std::exception e) {
        QMessageBox::critical(nullptr, "Error", "Some problems with reading file");
    }
}

void Widget::addCamera() {
    std::string cam_name = std::string("camera_") + std::to_string(++_elements_num);
    commands::AddCameraCommand command(cam_name);
    _facade->execCommand(&command);

    ui->comboBox->addItem(cam_name.c_str());
}

void Widget::removeObject() {
    if (ui->comboBox->count() == 0) {
        return;
    }
    std::string obj_name = ui->comboBox->currentText().toStdString();
    if (obj_name[0] == 'm') {
        commands::RemoveModelCommand command(obj_name);
        _facade->execCommand(&command);
    } else {
        commands::RemoveCameraCommand command(obj_name);
        _facade->execCommand(&command);
    }

    render();
}

void Widget::setCamera() {
    std::string cam_name = ui->comboBox->currentText().toStdString();
    if (cam_name[0] == 'c') {
        commands::SetCameraCommand command(cam_name);
        _facade->execCommand(&command);

        render();
    } else {
        QMessageBox::critical(nullptr, "Error", "You should choose camera");
    }
}

void Widget::render() {
    QPainter painter(&_pixmap);
    _pixmap.fill();
    auto drawer = new render::QtDrawer(&painter);
    auto command = new commands::RenderCommand(std::shared_ptr<render::BaseDrawer>(drawer));
    try {
        _facade->execCommand(command);
        _draw_label->update();
    } catch(exceptions::CameraLackException e) {
        QMessageBox::critical(nullptr, "Error", "Choose camera to render smth");
    }
    delete command;
}
