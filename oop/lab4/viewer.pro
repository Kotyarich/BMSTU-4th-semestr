#-------------------------------------------------
#
# Project created by QtCreator 2019-05-19T00:24:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = viewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        commands/add_camera_command.cpp \
        commands/add_model_command.cpp \
        commands/move_command.cpp \
        commands/pitch_camera_command.cpp \
        commands/remove_object_command.cpp \
        commands/render_command.cpp \
        commands/roll_camera_command.cpp \
        commands/rotate_commands.cpp \
        commands/scale_command.cpp \
        commands/set_camera.cpp \
        commands/yaw_camera_command.cpp \
        drawlabel.cpp \
        facade/viewer_facade.cpp \
        intermediary/intermediary.cpp \
        main.cpp \
        math/matrix.cpp \
        math/point.cpp \
        objects/camera.cpp \
        objects/composite.cpp \
        objects/model.cpp \
        render/qt_drawer.cpp \
        render/renderer.cpp \
        scene/scene.cpp \
        transformations/camerarotator.cpp \
        transformations/transformer.cpp \
        uploading/file_loader.cpp \
        uploading/model_loader.cpp \
        widget.cpp

HEADERS += \
        commands/add_camera_command.h \
        commands/add_model_command.h \
        commands/basecommand.h \
        commands/move_command.h \
        commands/pitch_camera_command.h \
        commands/remove_object_command.h \
        commands/render_command.h \
        commands/roll_camera_command.h \
        commands/rotate_commands.h \
        commands/scale_command.h \
        commands/set_camera.h \
        commands/yaw_camera_command.h \
        drawlabel.h \
        exceptions/base_viewer_exeption.h \
        exceptions/camera_lack_exception.h \
        exceptions/file_open_exception.h \
        exceptions/object_search_exception.h \
        exceptions/object_type_exception.h \
        facade/viewer_facade.h \
        intermediary/intermediary.h \
        math/matrix.h \
        math/point.h \
        objects/camera.h \
        objects/composite.h \
        objects/model.h \
        objects/object.h \
        render/base_drawer.h \
        render/qt_drawer.h \
        render/renderer.h \
        scene/scene.h \
        transformations/camerarotator.h \
        transformations/transformer.h \
        uploading/base_loader.h \
        uploading/file_loader.h \
        uploading/model_loader.h \
        widget.h

FORMS += \
        widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
