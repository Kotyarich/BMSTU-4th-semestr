#ifndef SCALE_COMMAND_H
#define SCALE_COMMAND_H

#include "basecommand.h"
#include "../intermediary/intermediary.h"

namespace commands {

using intermediary::Intermediary;

class ScaleCommand: public BaseCommand {
public:
    ScaleCommand(std::string object_name, double x, double y, double z);
    void execute(std::shared_ptr<Intermediary> intermediary) override;
private:
    std::string _object_name;
    double _x;
    double _y;
    double _z;
};

} // namespace commands

#endif // SCALE_COMMAND_H
