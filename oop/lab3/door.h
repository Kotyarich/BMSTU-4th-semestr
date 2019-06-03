#ifndef DOOR_H
#define DOOR_H

#include <QObject>
#include <QTimer>

namespace lift {

class Door : public QObject {
    Q_OBJECT

public:
    enum DoorState {
        OPENED,
        CLOSED,
        OPENING,
        CLOSING
    };

    explicit Door(QObject* parent = nullptr);
    ~Door() override;

signals:
    void openedSignal();
    void closedSignal();
    void openingSignal();
    void closingSignal();

public slots:
    void closingSlot();
    void openingSlot();

private slots:
    void closedSlot();
    void openedSlot();

private:
    DoorState _state;
    QTimer* _timer;

    static const int _opening_time = 1000;
    static const int _closing_time = 1000;
};


} // namespace lift

#endif // DOOR_H
