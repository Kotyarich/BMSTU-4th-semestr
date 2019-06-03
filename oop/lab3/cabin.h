#ifndef CABIN_H
#define CABIN_H

#include <QLabel>
#include "door.h"

namespace lift {

class Cabin : public QObject {
    Q_OBJECT

public:
    enum CabinState {
        MOVING,
        STAYING_OPENED,
        OPENING,
        STAYING_CLOSED,
        CLOSING
    };

    explicit Cabin(QObject* parent = nullptr);
    ~Cabin() override;
    void setDestFloor(int floor);
public slots:
    void movingSlot();

private slots:
    void stopClosingSlot();
    void openingSlot();
    void stopOpeningSlot();
    void closingSlot();

signals:
    void movingSignal(int floor);
    void stopClosingSignal(int floor);
    void openingSignal();
    void stopOpeningSignal(int floor);
    void closingSignal();

private:
    Door* _door;
    CabinState _state;
    int _curr_floor;
    int _dest_floor;
    QTimer* _timer;

    static const int _moving_time = 800;
    static const int _staying_open_time = 2500;
    static const int _staying_closing_time = 800;
};

} // namespace lift

#endif // CABIN_H
