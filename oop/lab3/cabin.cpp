#include "cabin.h"

namespace lift {

Cabin::Cabin(QObject* parent)
    : QObject(parent) {
    _state = CabinState::STAYING_CLOSED;
    _curr_floor = 1;
    _dest_floor = 1;
    _door = new Door();
    _timer = nullptr;

    connect(_door, SIGNAL(openingSignal()), this, SLOT(openingSlot()));
    connect(_door, SIGNAL(openedSignal()), this, SLOT(stopOpeningSlot()));
    connect(_door, SIGNAL(closingSignal()), this, SLOT(closingSlot()));
    connect(_door, SIGNAL(closedSignal()), this, SLOT(stopClosingSlot()));
}

Cabin::~Cabin() {
    delete _door;
    delete _timer;
}

void Cabin::setDestFloor(int floor) {
    _dest_floor = floor;
}

void Cabin::movingSlot() {
    if (_state == MOVING || _state == STAYING_CLOSED) {
        _state = MOVING;
        bool isEndingMove = false;
        if (_curr_floor < _dest_floor) {
            emit movingSignal(_curr_floor++);
        } else if (_curr_floor > _dest_floor) {
            emit movingSignal(_curr_floor--);
        } else {
            isEndingMove = true;
            emit movingSignal(_curr_floor);
        }

        delete _timer;
        _timer = new QTimer();
        _timer->setSingleShot(true);
        if (isEndingMove) {
            QObject::connect(_timer, SIGNAL(timeout()), this, SLOT(stopClosingSlot()));
        } else {
            QObject::connect(_timer, SIGNAL(timeout()), this, SLOT(movingSlot()));
        }
        _timer->start(_moving_time);
    }
}

void Cabin::stopClosingSlot() {
    if (_state == MOVING && _curr_floor == _dest_floor) {
        _state = STAYING_CLOSED;
        emit stopClosingSignal(_curr_floor);

        delete _timer;
        _timer = new QTimer();
        _timer->setSingleShot(true);
        QObject::connect(_timer, SIGNAL(timeout()), _door, SLOT(openingSlot()));
        _timer->start(_staying_closing_time);
    } else if (_state == CLOSING) {
        _state = STAYING_CLOSED;
        emit stopClosingSignal(_curr_floor);
    }
}

void Cabin::openingSlot() {
    if (_state == STAYING_CLOSED) {
        _state = OPENING;
        emit openingSignal();
    }
}

void Cabin::stopOpeningSlot() {
    if (_state == OPENING) {
        _state = STAYING_OPENED;
        emit stopOpeningSignal(_curr_floor);

        delete _timer;
        _timer = new QTimer();
        _timer->setSingleShot(true);
        QObject::connect(_timer, SIGNAL(timeout()), _door, SLOT(closingSlot()));
        _timer->start(_staying_open_time);
    }
}

void Cabin::closingSlot() {
    if (_state == STAYING_OPENED) {
        _state = CLOSING;
        emit closingSignal();
    }
}


} // namespace lift
