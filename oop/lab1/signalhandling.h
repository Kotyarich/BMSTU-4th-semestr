#ifndef LAB1_SIGNALHANDLING_H
#define LAB1_SIGNALHANDLING_H

#include <QDebug>
#include "model.h"
#include "command.h"

const int UNKNOWN_COMMAND_ERR = -1;

int handleSignal(const Command & command);

#endif //LAB1_SIGNALHANDLING_H
