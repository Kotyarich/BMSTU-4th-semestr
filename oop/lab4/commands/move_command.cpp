#include "move_command.h"

namespace commands {

MoveCommand::MoveCommand(std::string object_name, int x, int y, int z):
    _object_name(object_name), _x(x), _y(y), _z(z) {}

void MoveCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->moveObject(_object_name, _x, _y, _z);
}

} // namespace commands
