#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "memory"
#include "base_loader.h"
#include "objects/model.h"

namespace uploading {

using objects::Model;

class ModelLoader
{
public:
    explicit ModelLoader(BaseLoader *loader);
    void loadModel(Model &model, std::string source_name);
private:
    std::unique_ptr<BaseLoader> _loader;
};

} // namespace uploading

#endif // MODEL_LOADER_H
