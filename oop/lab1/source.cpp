#include "source.h"

Source initSource() {
    return nullptr;
}

int openReadSource(Source & source, const LoadParams & params) {
    source = fopen(params.path, "r");
    if (!source) {
        return SOURCE_OPEN_ERR;
    }
    return 0;
}

int openWriteSource(Source & source, const SaveParams & params) {
    source = fopen(params.path, "w");
    if (!source) {
        return SOURCE_OPEN_ERR;
    }
    return 0;
}

void closeSource(Source & source) {
    if (source) {
        fclose(source);
        source = nullptr;
    }
}

int readInt(int & value, Source source) {
    return fscanf(source, "%d", &value) != 1;
}

int readDouble(double & value, Source source) {
    return fscanf(source, "%lf", &value) != 1;
}

void writeInt(Source source, const int value) {
    fprintf(source, "%d", value);
}

void writeString(Source source, const char * value) {
    fprintf(source, value);
}

void writeDouble(Source source, const double value) {
    fprintf(source, "%lf", value);
}
