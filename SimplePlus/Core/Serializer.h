#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <string.h>

struct Serializer {
  static int ReadInt(const char *data, int *readCtx);
  static float ReadFloat(const char *data, int *readCtx);
  static double ReadDouble(const char *data, int *readCtx);
  static const char *ReadString(const char *data, int *readCtx);

  static void WriteInt(int value, const char *data, int *writeCtx);
  static void WriteFloat(float value, const char *data, int *writeCtx);
  static void WriteDouble(double value, const char *data, int *writeCtx);
  static void WriteString(const char *value, char *data, int *writeCtx);
};
#endif
