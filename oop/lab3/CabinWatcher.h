#ifndef LIFT_CABINWATCHER_H
#define LIFT_CABINWATCHER_H

#include <QLabel>
#include <vector>
#include "cabin.h"

class CabinWatcher: public QObject {
    Q_OBJECT

public:
    explicit CabinWatcher(std::vector<QLabel*> &labels, lift::Cabin *cabin);

private slots:
    void cabinMovingSlot(int floor);
    void cabinStopClosingSlot();
    void cabinOpeningSlot();
    void cabinStopOpeningSlot();
    void cabinClosingSlot();

private:
    int _current_label_ind;
    std::vector<QLabel*> _labels;
    lift::Cabin *_cabin;
};


#endif //LIFT_CABINWATCHER_H
