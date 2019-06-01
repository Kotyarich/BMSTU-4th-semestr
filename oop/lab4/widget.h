#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <memory>
#include "drawlabel.h"
#include "math/point.h"
#include "commands/add_model_command.h"
#include "commands/render_command.h"
#include "commands/set_camera.h"
#include "commands/add_camera_command.h"
#include "commands/transform_model_command.h"
#include "commands/yaw_camera_command.h"
#include "commands/roll_camera_command.h"
#include "commands/pitch_camera_command.h"
#include "commands/transform_model_command.h"
#include "commands/move_camera_command.h"
#include "commands/remove_model_command.h"
#include "commands/remove_camera_command.h"
#include "exceptions/file_open_exception.h"
#include "exceptions/camera_lack_exception.h"
#include "facade/viewer_facade.h"
#include "render/qt_drawer.h"

namespace Ui {
class Widget;
}

using facade::ViewerFacade;
using math::Point;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void moveObject();
    void scaleObject();
    void rotateObjectX();
    void rotateObjectY();
    void rotateObjectZ();

    void addModel();
    void addCamera();
    void removeObject();
    void setCamera();

private:
    Ui::Widget *ui;
    DrawLabel *_draw_label;
    QPixmap _pixmap;
    std::unique_ptr<ViewerFacade> _facade;
    size_t _elements_num;

    void render();
};

#endif // WIDGET_H
