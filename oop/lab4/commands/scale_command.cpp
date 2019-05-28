#include "scale_command.h"

namespace commands {

ScaleCommand::ScaleCommand(std::string object_name, double x, double y, double z):
    _object_name(object_name), _x(x), _y(y), _z(z) {}

void ScaleCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->scaleObject(_object_name, _x, _y, _z);
}

} // namespace commands
