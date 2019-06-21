#include "roll_camera_command.h"

namespace commands {

RollCameraCommand::RollCameraCommand(std::string object_name, double angle):
    _object_name(object_name), _rotation(angle, 0, 0) {}

void RollCameraCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    math::Point moving(0, 0, 0);
    intermediary->transformCamera(_object_name, moving, _rotation);
}

} // namespace commands
