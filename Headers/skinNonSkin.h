#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <filesystem>
#include <algorithm>
using namespace std;
namespace fs = std::filesystem;
#ifndef SKINNONSKIN_H
#define SKINNONSKIN_H

struct Model
{
    double totalRecords;
    double skinRecords;
    double non_skinRecords;

    map<long long, int> skinMapping;
    map<long long, int> NonSkinMapping;

    Model()
    {
        totalRecords = 0;
        skinRecords = 0;
        non_skinRecords = 0;
    }
};

#pragma pack(push, 1)
struct BMPFileHeader
{
    uint16_t fileType; // 'BM'
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offsetData;
};

struct BMPInfoHeader
{
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

struct Pixel
{
    uint8_t b, g, r;
};

Model trainModel();
bool isHumanSkin(int B, int G, int R, Model &model);
void searchSkin(fs::path root, bool isRecursive);
fs::path getExtractedSkinPath(fs::path ogFilePath);
void clearImageCache();
void updateImageCache();

#endif