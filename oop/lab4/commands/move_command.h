#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "basecommand.h"
#include "../intermediary/intermediary.h"

namespace commands {

using intermediary::Intermediary;

class MoveCommand: public BaseCommand {
public:
    MoveCommand(std::string object_name, int x, int y, int z);
    void execute(std::shared_ptr<Intermediary> intermediary) override;
private:
    std::string _object_name;
    int _x;
    int _y;
    int _z;
};

} // namespace commands

#endif // MOVE_COMMAND_H
