#include "add_camera_command.h"

namespace commands {

AddCameraCommand::AddCameraCommand(std::string name): _name(name) {}

void AddCameraCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->addCamera(_name);
}

} // namespace commands
