#include "remove_object_command.h"

namespace commands {

RemoveObjectCommand::RemoveObjectCommand(std::string object_name):
    _object_name(object_name) {}

void RemoveObjectCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->removeObject(_object_name);
}

} // namespace commands
