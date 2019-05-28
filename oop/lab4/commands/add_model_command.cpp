#include "add_model_command.h"

namespace commands {

AddModelCommand::AddModelCommand(std::string file_name, std::string model_name):
    _file_name(file_name), _model_name(model_name) {}

void AddModelCommand::execute(std::shared_ptr<intermediary::Intermediary> intermediary) {
    intermediary->addModel(_file_name, _model_name);
}

} // namespace commands
