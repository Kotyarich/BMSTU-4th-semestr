#include "model_loader.h"

namespace uploading {

ModelLoader::ModelLoader(BaseLoader *loader): _loader(loader) {}

void ModelLoader::loadModel(objects::Model &model, std::string source_name)
{
    _loader->open(source_name);
    _loader->readModelInfo(model);
    _loader->readModel(model);
    _loader->close();
}

} // namespace uploading
