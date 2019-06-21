#ifndef CAMERA_LACK_EXCEPTION_H
#define CAMERA_LACK_EXCEPTION_H

#include "base_viewer_exeption.h"

namespace exceptions {

class CameraLackException: public BaseViewerException {
public:
    CameraLackException() = default;
    explicit CameraLackException(std::string message): BaseViewerException(message) {}

    const char *what() const noexcept override {
        return "Camera wasn't set";
    }
};

} // namespace exceptions

#endif // CAMERA_LACK_EXCEPTION_H
