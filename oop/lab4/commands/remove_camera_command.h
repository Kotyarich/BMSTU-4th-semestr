#ifndef REMOVE_CAMERA_COMMAND_H
#define REMOVE_CAMERA_COMMAND_H

#include "basecommand.h"
#include "intermediary/intermediary.h"

namespace commands {

class RemoveCameraCommand: public BaseCommand {
public:
    RemoveCameraCommand(std::string object_name);
    void execute(std::shared_ptr<Intermediary> intermediary) override;
private:
    std::string _object_name;
};

} // namespace commands

#endif // REMOVE_CAMERA_COMMAND_H
