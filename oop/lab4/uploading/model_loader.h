#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "model_builder.h"
#include "abs_model_loader.h"
#include "memory"
#include "base_loader_imp.h"

namespace uploading {

using objects::Model;

class ModelLoader: public AbsModelLoader {
public:
    explicit ModelLoader(BaseLoader *loader);
    std::shared_ptr<Model> loadModel(std::string source_name, std::string model_name) override;
private:
    std::unique_ptr<BaseLoader> _loader;
    std::unique_ptr<ModelBuilder> _builder;
};

} // namespace uploading

#endif // MODEL_LOADER_H
