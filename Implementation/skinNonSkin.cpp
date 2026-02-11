#include "../Headers/skinNonSkin.h"
#include "../Headers/utility.h"
#include <iomanip>
#include <limits>
#include <clocale>
#define nl '\n'
using namespace std;
namespace fs = std::filesystem;

Model getModel(bool modelNo)
{
    string model_path;

    if (modelNo == 0)
        model_path = "Models/model_1.txt";
    else if (modelNo == 1)
        model_path = "Models/model_2.txt";
    else
        throw invalid_argument("Model no. is not specified appropriately.");

    ifstream model_file(model_path);
    Model model;

    if (!model_file.is_open())
    {
        throw invalid_argument("The Required Model is not trained yet.");
    }

    long double x;

    int i = 0;
    while (model_file >> x)
    {
        model.pixelProb[i++] = x;
    }

    return model;
}

bool writeBMP(fs::path filePath, int height, int width, vector<vector<Pixel>> &image)
{
    fs::create_directories(filePath.parent_path());
    ofstream file(filePath, ios::binary);
    if (!file)
    {
        cout << "Can't write the file for bmp image" << nl;
        return false;
    }

    int rowPadded = (width * 3 + 3) & (~3);
    int imageSize = rowPadded * height;

    BMPFileHeader fileHeader{};
    BMPInfoHeader infoHeader{};

    fileHeader.fileType = 0x4D42;
    fileHeader.offsetData = 54;
    fileHeader.fileSize = fileHeader.offsetData + imageSize;

    infoHeader.size = 40;
    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.planes = 1;
    infoHeader.bitCount = 24;
    infoHeader.compression = 0;
    infoHeader.imageSize = imageSize;

    file.write((char *)&fileHeader, sizeof(fileHeader));
    file.write((char *)&infoHeader, sizeof(infoHeader));

    vector<uint8_t> row(rowPadded);

    for (int i = height - 1; i >= 0; i--)
    {
        int idx = 0;
        for (int j = 0; j < width; j++)
        {
            row[idx++] = image[i][j].b;
            row[idx++] = image[i][j].g;
            row[idx++] = image[i][j].r;
        }
        while (idx < rowPadded)
            row[idx++] = 0;
        file.write((char *)row.data(), rowPadded);
    }
    return true;
}

bool hasSkin(fs::path filePath, Model &model)
{
    // If it has already a cache
    if (fs::exists(getExtractedSkinPath(filePath)))
    {
        print_text_fading("Retrieving from cache...\n\n");
        return 1;
    }

    ifstream file(filePath, ios::binary);
    if (!file)
    {
        cout << "Cannot open bmp file - " << fs::absolute(filePath) << nl;
        return 0;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    file.read((char *)&fileHeader, sizeof(fileHeader));
    file.read((char *)&infoHeader, sizeof(infoHeader));

    // Check BMP format
    if (fileHeader.fileType != 0x4D42)
    { // 'BM'
        // cout << "Not a BMP file\n";
        return 0;
    }

    if (infoHeader.bitCount != 24)
    {
        // cout << "Only 24-bit BMP supported\n";
        return 0;
    }

    int width = infoHeader.width;
    int height = infoHeader.height;

    file.seekg(fileHeader.offsetData, ios::beg);

    int rowPadded = (width * 3 + 3) & (~3);

    vector<vector<Pixel>> image(height, vector<Pixel>(width));

    for (int i = height - 1; i >= 0; i--)
    {
        vector<uint8_t> row(rowPadded);
        file.read((char *)row.data(), rowPadded);

        for (int j = 0; j < width; j++)
        {
            image[i][j].b = row[j * 3];
            image[i][j].g = row[j * 3 + 1];
            image[i][j].r = row[j * 3 + 2];
        }
    }

    file.close();

    // BMP image loaded right now
    print_text_fading("Predicting skin segment from the Image!\n");

    bool gotSkin = 0;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            long long pixel_ind = 256 * 256 * image[i][j].r + 256 * image[i][j].g + image[i][j].b;
            if (model.pixelProb[pixel_ind] >= 0.4)
            {
                gotSkin = 1;
                image[i][j].b = 255;
                image[i][j].g = 255;
                image[i][j].r = 255;
            }
            else
            {
                image[i][j].b = 0;
                image[i][j].g = 0;
                image[i][j].r = 0;
            }
        }

    if (gotSkin)
    {
        fs::path path = getExtractedSkinPath(filePath);
        writeBMP(path, height, width, image);
        return 1;
    }
    else
        return 0;
}

fs::path getExtractedSkinPath(fs::path ogFilePath)
{
    ogFilePath = fs::absolute(ogFilePath);

    string ogFilePath_str = ogFilePath;
    ogFilePath_str.erase(ogFilePath_str.begin(), ogFilePath_str.begin() + 1);

    fs::path extractedSkinPath = "ExtractedSkin/" + ogFilePath_str;

    string newFileName = to_string(convert_to_seconds_since_epoch(fs::last_write_time(ogFilePath))) + "_" + (string)ogFilePath.filename();
    extractedSkinPath.replace_filename(newFileName);

    return extractedSkinPath;
}

void searchSkin(bool modelNo, fs::path root, bool isRecursive)
{
    if (!fs::exists(root))
    {
        cout << "" << root << " this root path doesnt exist..." << nl;
        throw invalid_argument("Root directory invalid");
    }

    Model model = getModel(modelNo);
    bool searchResult;

    vector<fs::path> foundSkinPaths;

    if (isRecursive)
        for (const auto &entry : fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied))
        {
            if (entry.path().extension() != ".bmp")
                continue;

            searchResult = hasSkin(entry.path(), model);
            if (!searchResult)
                continue;
            else
                foundSkinPaths.push_back(entry.path());
        }
    else
        for (const auto &entry : fs::directory_iterator(root, fs::directory_options::skip_permission_denied))
        {
            if (entry.path().extension() != ".bmp")
                continue;

            searchResult = hasSkin(entry.path(), model);
            if (!searchResult)
                continue;
            else
                foundSkinPaths.push_back(entry.path());
        }

    cout << nl;
    if (foundSkinPaths.size())
    {
        for (int i = 0; i < foundSkinPaths.size(); i++)
        {
            print_text_magenta_fade('(' + to_string(i + 1) + ')' + " ");
            // cout << '(' << i + 1 << ')';
            print_text_magenta_underline(foundSkinPaths[i]);
            cout << nl;
            // cout << foundSkinPaths[i] << nl;
        }
        cout << nl;

        int select;
        while (1)
        {
            cout << "Which one do you want to open? (0 to exit)" << nl;
            print_text_yellow_fade("=> ");
            cin >> select;

            if (!select)
                break;

            print_text_blue_fade("Opening the " + to_string(select) + "th image...." + nl);
            cout << nl;
            // cout << "Opening the " << select << "th image...." << nl;
            openImage(getExtractedSkinPath(foundSkinPaths[select - 1]));
        }
    }
    else
        cout << "Nothing is found" << nl;
}

void clearImageCache()
{
    fs::path cacheRoot = "ExtractedSkin";

    uintmax_t count = 0;

    if (fs::exists(cacheRoot))
        for (const auto &entry : fs::directory_iterator(cacheRoot, fs::directory_options::skip_permission_denied))
            count = fs::remove_all(entry.path());

    cout << "Deleted total '" << count << "' image caches." << nl;
}

void updateImageCache()
{
    fs::path cacheRoot = "ExtractedSkin";

    uintmax_t count = 0;
    if (fs::exists(cacheRoot))
        for (fs::recursive_directory_iterator it(cacheRoot, fs::directory_options::skip_permission_denied), end; it != end; ++it)
        {
            auto entry = *it;

            string entryPath = entry.path();
            string ogPathStr = entryPath.erase(0, 13);
            fs::path ogPath = ogPathStr;

            if (entry.path().extension() != ".bmp" && !fs::exists(ogPath))
            {
                count += remove_all(entry.path());
                it.disable_recursion_pending();
            }
            else if (entry.path().extension() == ".bmp")
            {
                string newFileName = ogPath.filename();
                reverse(newFileName.begin(), newFileName.end());

                long long last_modification_time = 0;
                int ind = newFileName.size();
                while (newFileName[--ind] != '_')
                {
                    last_modification_time = last_modification_time * 10 + (newFileName[ind] - '0');
                    newFileName.pop_back();
                }

                newFileName.pop_back();
                reverse(newFileName.begin(), newFileName.end());

                ogPath.replace_filename(newFileName);

                if (!fs::exists(ogPath) || convert_to_seconds_since_epoch(fs::last_write_time(ogPath)) != last_modification_time)
                {
                    count++;
                    remove(entry.path());
                }
            }
        }

    cout << "Deleted total '" << count << "' image caches." << nl;
}