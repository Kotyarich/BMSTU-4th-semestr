#ifndef VIEWER_FACADE_H
#define VIEWER_FACADE_H

#include "../intermediary/intermediary.h"
#include "../commands/basecommand.h"

namespace facade {

using intermediary::Intermediary;
using commands::BaseCommand;

class ViewerFacade {
public:
    ViewerFacade();
    void execCommand(BaseCommand *command);
private:
    std::shared_ptr<Intermediary> _intermediary;
};

} // namespace facade

#endif // VIEWER_FACADE_H
