#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include "basecommand.h"
#include "../math/point.h"
#include "../intermediary/intermediary.h"

namespace commands {

using intermediary::Intermediary;
using math::Point;

class TransformModelCommand: public BaseCommand {
public:
    TransformModelCommand(std::string object_name, Point move, Point scale, Point rotate);
    void execute(std::shared_ptr<Intermediary> intermediary) override;
private:
    std::string _object_name;
    Point _move;
    Point _scale;
    Point _rotate;
};

} // namespace commands

#endif // MOVE_COMMAND_H
