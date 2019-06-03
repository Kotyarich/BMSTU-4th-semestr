#include "CabinWatcher.h"

void CabinWatcher::cabinMovingSlot(int floor) {
    _labels[_current_label_ind]->setStyleSheet("QLabel { background-color: white; border: 1px solid black; }");
    _current_label_ind = floor - 1;
    _labels[_current_label_ind]->setStyleSheet("QLabel { background-color: red; border: 1px solid black; }");
}

void CabinWatcher::cabinStopClosingSlot() {
    _labels[_current_label_ind]->setStyleSheet("QLabel { background-color: red; border: 1px solid black; }");
}

void CabinWatcher::cabinOpeningSlot() {
    _labels[_current_label_ind]->setStyleSheet("QLabel { background-color: yellow; border: 1px solid black; }");
}

void CabinWatcher::cabinStopOpeningSlot() {
    _labels[_current_label_ind]->setStyleSheet("QLabel { background-color: green; border: 1px solid black; }");
}

void CabinWatcher::cabinClosingSlot() {
    _labels[_current_label_ind]->setStyleSheet("QLabel { background-color: orange; border: 1px solid black; }");
}

CabinWatcher::CabinWatcher(std::vector<QLabel *> &labels, lift::Cabin *cabin):
    _labels(labels), _current_label_ind(0), _cabin(cabin) {
    connect(_cabin, SIGNAL(movingSignal(int)), this, SLOT(cabinMovingSlot(int)));
    connect(_cabin, SIGNAL(stopOpeningSignal(int)), this, SLOT(cabinStopOpeningSlot()));
    connect(_cabin, SIGNAL(closingSignal()), this, SLOT(cabinClosingSlot()));
    connect(_cabin, SIGNAL(stopClosingSignal(int)), this, SLOT(cabinStopClosingSlot()));
    connect(_cabin, SIGNAL(openingSignal()), this, SLOT(cabinOpeningSlot()));
}
