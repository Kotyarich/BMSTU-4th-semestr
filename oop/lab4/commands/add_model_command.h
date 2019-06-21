#ifndef ADD_MODEL_COMMAND_H
#define ADD_MODEL_COMMAND_H

#include <string>
#include "basecommand.h"
#include "../intermediary/intermediary.h"

namespace commands {

using intermediary::Intermediary;

class AddModelCommand: public BaseCommand {
public:
    explicit AddModelCommand(std::string file_name, std::string model_name);
    void execute(std::shared_ptr<Intermediary> intermediary) override;
private:
    std::string _file_name;
    std::string _model_name;
};

} // namespace commands

#endif // ADD_MODEL_COMMAND_H
