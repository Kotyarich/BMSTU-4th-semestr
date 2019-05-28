#include "rotate_commands.h"

namespace commands {

RotateXCommand::RotateXCommand(std::string object_name, double angle):
    _object_name(object_name), _angle(angle) {}

void RotateXCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->rotateObjectX(_object_name, _angle);
}

RotateYCommand::RotateYCommand(std::string object_name, double angle):
    _object_name(object_name), _angle(angle) {}

void RotateYCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->rotateObjectY(_object_name, _angle);
}

RotateZCommand::RotateZCommand(std::string object_name, double angle):
    _object_name(object_name), _angle(angle) {}

void RotateZCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->rotateObjectX(_object_name, _angle);
}

} // namespace commands
