#include "pitch_camera_command.h"

namespace commands {

PitchCameraCommand::PitchCameraCommand(std::string object_name, double angle):
    _object_name(object_name), _angle(angle) {}

void PitchCameraCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->pitchCamera(_object_name, _angle);
}

} // namespace commands
