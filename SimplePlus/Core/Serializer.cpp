#include "Serializer.h"

int Serializer::ReadInt(const char *data, int *readCtx) {
  int val = *((int *)&data[*readCtx]);
  *readCtx += sizeof(int);
  return val;
}

float Serializer::ReadFloat(const char *data, int *readCtx) {
  int val = *((float *)&data[*readCtx]);
  *readCtx += sizeof(float);
  return val;
}

double Serializer::ReadDouble(const char *data, int *readCtx) {
  double val = *((double *)&data[*readCtx]);
  *readCtx += sizeof(double);
  return val;
}

const char *Serializer::ReadString(const char *data, int *readCtx) {
  int size = ReadInt(data, readCtx);
  const char *value = &data[*readCtx];
  *readCtx += size;
  return value;
}

void Serializer::WriteInt(int value, const char *data, int *writeCtx) {
  *((int *)&data[*writeCtx]) = value;
  *writeCtx += sizeof(int);
}

void Serializer::WriteFloat(float value, const char *data, int *writeCtx) {
  *((float *)&data[*writeCtx]) = value;
  *writeCtx += sizeof(float);
}

void Serializer::WriteDouble(double value, const char *data, int *writeCtx) {
  *((double *)&data[*writeCtx]) = value;
  *writeCtx += sizeof(double);
}

void Serializer::WriteString(const char *value, char *data, int *writeCtx) {
  int len = strlen(value);
  WriteInt(len, data, writeCtx);
  memcpy(&data[*writeCtx], value, len);
  *writeCtx += len;
}
