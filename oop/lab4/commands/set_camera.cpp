#include "set_camera.h"

namespace commands {

SetCameraCommand::SetCameraCommand(std::string name): _name(name) {}

void SetCameraCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->setCamera(_name);
}

} // namespace commands
