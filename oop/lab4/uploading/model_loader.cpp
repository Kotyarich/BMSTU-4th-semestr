#include "model_loader.h"

namespace uploading {

ModelLoader::ModelLoader(BaseLoader *loader): _loader(loader), _builder(new ModelBuilder()) {}

std::shared_ptr<objects::Model> ModelLoader::loadModel(std::string source_name, std::string model_name) {
    _loader->open(source_name);
    _builder->reset(model_name);

    std::pair<size_t, size_t> model_info = _loader->readModelInfo();

    for (size_t i = 0; i < model_info.first; i++) {
        math::Point p = _loader->readPoint();
        _builder->addPoint(p);
    }

    for (size_t i = 0; i < model_info.second; i++) {
        std::pair<size_t, size_t> model_info = _loader->readEdgeInfo();
        _builder->addEdge(model_info.first, model_info.second);
    }

    _loader->close();
    return _builder->getResult();
}

} // namespace uploading
