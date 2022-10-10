#include "Serializer.h"

const int Serializer::ReadInt(const char* data, int* readCtx) {
    int val = *((int*)&data[*readCtx]);
    *readCtx += sizeof(int);
    return val;
}

const float Serializer::ReadFloat(const char* data, int* readCtx) {
    int val = *((float*)&data[*readCtx]);
    *readCtx += sizeof(float);
    return val;
}

const double Serializer::ReadDouble(const char* data, int* readCtx) {
    double val = *((double*)&data[*readCtx]);
    *readCtx += sizeof(double);
    return val;
}

const char* Serializer::ReadString(const char* data, int* readCtx) {
    int len = ReadInt(data, readCtx);

    if (len <= 0 && len > 128) {
        printf("[Serializer]: reading string lenght was outside of bonds: %d", len);
        return NULL;
    }

    const char* value = &data[*readCtx];
    *readCtx += len;
    return value;
}

void Serializer::WriteInt(int value, const char* data, int* writeCtx) {
    *((int*)&data[*writeCtx]) = value;
    *writeCtx += sizeof(int);
}

void Serializer::WriteFloat(float value, const char* data, int* writeCtx) {
    *((float*)&data[*writeCtx]) = value;
    *writeCtx += sizeof(float);
}

void Serializer::WriteDouble(double value, const char* data, int* writeCtx) {
    *((double*)&data[*writeCtx]) = value;
    *writeCtx += sizeof(double);
}

void Serializer::WriteString(const char* value, char* data, int* writeCtx) {
    int len = strlen(value);

    if (len <= 0 && len > 128) {
        printf("[Serializer]: writing string lenght was outside of bonds: %d", len);
        return;
    }

    WriteInt(len, data, writeCtx);
    memcpy(&data[*writeCtx], value, len);
    *writeCtx += len;

    return;
}
