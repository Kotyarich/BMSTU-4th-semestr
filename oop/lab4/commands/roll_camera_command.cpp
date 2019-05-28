#include "roll_camera_command.h"

namespace commands {

RollCameraCommand::RollCameraCommand(std::string object_name, double angle):
    _object_name(object_name), _angle(angle) {}

void RollCameraCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->rollCamera(_object_name, _angle);
}

} // namespace commands
