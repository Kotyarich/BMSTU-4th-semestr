#include "move_camera_command.h"

namespace commands {

MoveCameraCommand::MoveCameraCommand(std::string object_name, math::Point move):
    _object_name(object_name), _move(move) {}

void MoveCameraCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    math::Point rotation(0, 0, 0);
    intermediary->transformCamera(_object_name, _move, rotation);
}

} // namespace commands
