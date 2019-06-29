#ifndef LAB1_SOURCE_H
#define LAB1_SOURCE_H

#include <cstdio>
#include "command.h"

const int SOURCE_OPEN_ERR = -1;
const int FILE_PARSE_ERR = -2;
const int WRONG_ARGS_ERR = -3;

typedef FILE * Source;

Source initSource();
int openReadSource(Source & source, const LoadParams & params);
int openWriteSource(Source & source, const SaveParams & params);
void closeSource(Source & source);
int readInt(int & value, Source source);
int readDouble(double & value, Source source);
void writeInt(Source source, const int value);
void writeString(Source source, const char * value);
void writeDouble(Source source, const double value);

#endif //LAB1_SOURCE_H
