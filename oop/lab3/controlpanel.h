#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QPushButton>
#include <queue>
#include "cabin.h"

namespace lift {

class Controller: public QObject {
Q_OBJECT

public:
    enum ControllerState {
        BUSY,
        WAITING
    };

    explicit Controller(std::vector<QPushButton*> &buttons, QObject *parent = nullptr);
    ~Controller() override = default;

    void setCabin(Cabin *cabin);
    void addFloor(int floor);

private slots:
    void waitingSlot();
    void ridesSlot();

private:
    std::queue<int> _floors_queue;
    Cabin *_cabin;
    ControllerState _state;
    std::vector<QPushButton*> _buttons;
    int _last;
};

} // namespace lift

#endif // CONTROLPANEL_H
