#include "transform_model_command.h"

namespace commands {

TransformModelCommand::TransformModelCommand(std::string object_name, math::Point move, math::Point scale, math::Point rotate):
    _object_name(object_name), _move(move), _scale(scale), _rotate(rotate) {}

void TransformModelCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->transformModel(_object_name, _move, _scale, _rotate);
}

} // namespace commands
