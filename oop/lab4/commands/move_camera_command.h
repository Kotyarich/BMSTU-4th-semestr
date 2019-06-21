#ifndef MOVE_CAMERA_COMMAND_H
#define MOVE_CAMERA_COMMAND_H

#include "basecommand.h"
#include "../math/point.h"
#include "../intermediary/intermediary.h"

namespace commands {

using intermediary::Intermediary;
using math::Point;

class MoveCameraCommand: public BaseCommand {
public:
    MoveCameraCommand(std::string object_name, Point move);
    void execute(std::shared_ptr<Intermediary> intermediary) override;
private:
    std::string _object_name;
    Point _move;
};

} // namespace commands

#endif // MOVE_CAMERA_COMMAND_H
