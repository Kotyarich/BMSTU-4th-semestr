#include "file_loader.h"

namespace uploading {

FileLoader::~FileLoader() {
    close();
}

void FileLoader::open(std::string source_name) {
    if (_file.is_open()) {
        close();
    }

    _file.open(source_name);
    if (!_file) {
        throw exceptions::FileOpenException();
    }
}

void FileLoader::close() {
    if (_file.is_open()) {
        _file.close();
    }
}

std::pair<size_t, size_t> FileLoader::readModelInfo() {
    if (!_file.is_open()) {
        throw exceptions::FileOpenException();
    }

    size_t points = 0;
    size_t edges = 0;
    _file >> points >> edges;

    return std::make_pair(points, edges);
}

Point FileLoader::readPoint() {
    double x = -1, y = -1, z = -1;
    _file >> x >> y >> z;
    return {x, y, z};
}

std::pair<size_t, size_t> FileLoader::readEdgeInfo() {
    int num1 = -1, num2 = -1;
    _file >> num1 >> num2;
    return std::make_pair(num1, num2);
}

} // namespace uploading
