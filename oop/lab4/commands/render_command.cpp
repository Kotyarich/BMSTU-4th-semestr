#include "render_command.h"

namespace commands {

RenderCommand::RenderCommand(std::shared_ptr<render::BaseDrawer> drawer):
    _drawer(drawer) {}

void RenderCommand::execute(std::shared_ptr<Intermediary> intermediary) {
    intermediary->draw(_drawer);
}

} // namespace commands
