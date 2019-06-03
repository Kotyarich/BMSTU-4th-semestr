#include <iostream>
#include "controlpanel.h"

namespace lift {

Controller::Controller(std::vector<QPushButton*> &buttons, QObject *parent)
    : QObject(parent), _buttons(buttons), _last(-1) {
    _state = WAITING;
}

void Controller::setCabin(Cabin *cabin) {
    _cabin = cabin;
    connect(_cabin, SIGNAL(stopClosingSignal(int)), this, SLOT(waitingSlot()));
    connect(_cabin, SIGNAL(movingSignal(int)), this, SLOT(ridesSlot()));
}

void Controller::addFloor(int floor) {
    _buttons[floor - 1]->setEnabled(false);
    if (_state == WAITING) {
        _state = BUSY;
        _cabin->setDestFloor(floor);
        _last = floor;
        _cabin->movingSlot();
    } else {
        _floors_queue.push(floor);
    }
}

void Controller::waitingSlot() {
    if (_state == BUSY) {
        _state = WAITING;
    } else {
        if (!_floors_queue.empty()) {
            int floor = _floors_queue.front();
            _floors_queue.pop();
            _buttons[_last - 1]->setEnabled(true);
            _state = BUSY;
            _cabin->setDestFloor(floor);
            _last = floor;
            _cabin->movingSlot();
        } else {
            _buttons[_last - 1]->setEnabled(true);
        }
    }
}

void Controller::ridesSlot() {
    if (_state == WAITING) {
        _state = BUSY;
    }
}

} // namespace lift