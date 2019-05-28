#ifndef ROTATE_COMMAND_H
#define ROTATE_COMMAND_H

#include "basecommand.h"
#include "../intermediary/intermediary.h"

namespace commands {

using intermediary::Intermediary;

class RotateXCommand: public BaseCommand {
public:
    RotateXCommand(std::string object_name, double angle);
    void execute(std::shared_ptr<Intermediary> intermediary) override;
private:
    std::string _object_name;
    double _angle;
};

class RotateYCommand: public BaseCommand {
public:
    RotateYCommand(std::string object_name, double angle);
    void execute(std::shared_ptr<Intermediary> intermediary) override;
private:
    std::string _object_name;
    double _angle;
};

class RotateZCommand: public BaseCommand {
public:
    RotateZCommand(std::string object_name, double angle);
    void execute(std::shared_ptr<Intermediary> intermediary) override;
private:
    std::string _object_name;
    double _angle;
};

} // namespace commands

#endif // ROTATE_COMMAND_H
