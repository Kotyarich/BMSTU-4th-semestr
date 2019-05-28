#include "yaw_camera_command.h"

namespace commands {

YawCameraCommand::YawCameraCommand(std::string object_name, double angle):
    _object_name(object_name), _angle(angle) {}

void YawCameraCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->yawCamera(_object_name, _angle);
}

} // namespace commands
