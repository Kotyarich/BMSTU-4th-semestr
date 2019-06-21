#ifndef ABS_MODEL_LOADER_H
#define ABS_MODEL_LOADER_H

#include "memory"
#include "base_loader_imp.h"
#include "objects/model.h"

namespace uploading {

using objects::Model;

class AbsModelLoader {
public:
    AbsModelLoader() = default;
    virtual ~AbsModelLoader() = default;
    virtual std::shared_ptr<Model> loadModel(std::string source_name, std::string model_name) = 0;
};

} // namespace uploading

#endif // ABS_MODEL_LOADER_H
