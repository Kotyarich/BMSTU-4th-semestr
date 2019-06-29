#include "signalhandling.h"
#include "command.h"

int handleSignal(const Command &command)
{
    static Model model = initModel();
    int errCode = 0;

    switch (command.commandType) {
        case Command::save:
            errCode = saveModel(model, command.saveParams);
            break;
        case Command::load:            
            errCode = loadModel(model, command.loadParams);
            break;
        case Command::move:
            errCode = moveModel(model, command.moveParams);
            break;
        case Command::scale:
            errCode = scaleModel(model, command.scaleParams);
            break;
        case Command::rotate:
            errCode = rotateModel(model, command.rotateParams);
            break;
        case Command::draw:
            errCode = drawModel(model,
                                command.drawParams.painter, command.drawParams.widget);
            break;
        case Command::free:
            errCode = freeModel(model);
            break;
        default:
            errCode = UNKNOWN_COMMAND_ERR;
    }

    return errCode;
}
