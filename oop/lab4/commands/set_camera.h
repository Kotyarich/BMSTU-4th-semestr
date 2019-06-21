#ifndef SET_CAMERA_H
#define SET_CAMERA_H

#include <string>
#include "basecommand.h"
#include "../intermediary/intermediary.h"

namespace commands {

class SetCameraCommand: public BaseCommand {
public:
    explicit SetCameraCommand(std::string name);
    void execute(std::shared_ptr<Intermediary> intermediary) override;
private:
    std::string _name;
};

} // namespac commands

#endif // SET_CAMERA_H
