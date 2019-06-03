#include "door.h"

namespace lift {

Door::Door(QObject *parent): QObject(parent) {
    _state = CLOSED;
    _timer = nullptr;
}

Door::~Door() {
    delete _timer;
}

void Door::closingSlot() {
    if (_state == OPENED) {
        _state = CLOSING;
        emit closingSignal();
        delete _timer;
        _timer = new QTimer();
        _timer->setSingleShot(true);
        QObject::connect(_timer, SIGNAL(timeout()), this, SLOT(closedSlot()));
        _timer->start(_closing_time);
    }
}

void Door::closedSlot() {
    if (_state == CLOSING) {
        _state = CLOSED;
        emit closedSignal();
    }
}

void Door::openingSlot() {
    if (_state == CLOSED) {
        _state = OPENING;
        emit openingSignal();
        delete _timer;
        _timer = new QTimer();
        _timer->setSingleShot(true);
        QObject::connect(_timer, SIGNAL(timeout()), this, SLOT(openedSlot()));
        _timer->start(_opening_time);
    }
}

void Door::openedSlot() {
    if (_state == OPENING) {
        _state = OPENED;
        emit openedSignal();
    }
}

} // namespace lift
