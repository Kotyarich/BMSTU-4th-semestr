#ifndef MODEL_BUILD_EXCEPTION_H
#define MODEL_BUILD_EXCEPTION_H

#include "base_viewer_exeption.h"

namespace exceptions {

class ModelBuildException: public BaseViewerException {
public:
    ModelBuildException() = default;
    explicit ModelBuildException(std::string message): BaseViewerException(message) {}

    const char *what() const noexcept override {
        return "Error while building model";
    }
};

} // namespace exceptions

#endif // MODEL_BUILD_EXCEPTION_H
