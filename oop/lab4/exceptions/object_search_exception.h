#ifndef OBJECT_SEARCH_EXCEPTION_H
#define OBJECT_SEARCH_EXCEPTION_H

#include "base_viewer_exeption.h"

namespace exceptions {

class ObjectSearchException: public BaseViewerException {
public:
    ObjectSearchException() = default;
    explicit ObjectSearchException(std::string message): BaseViewerException(message) {}

    const char *what() const noexcept override {
        return "Object wasn't found";
    }
};

} // namespace exceptions

#endif // OBJECT_SEARCH_EXCEPTION_H
