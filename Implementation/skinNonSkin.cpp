#include "../Headers/skinNonSkin.h"
#include "../Headers/utility.h"
#define nl '\n'
using namespace std;
namespace fs = std::filesystem;

Model trainModel()
{
    ifstream dataset("Dataset/data_kaggle.txt");
    Model model;

    if (!dataset.is_open())
    {
        cout << "Can't open the dataset file" << nl;
        return model;
    }

    string line;
    int SkinRecords = 0, nonSkinRecords = 0;

    while (getline(dataset, line))
    {
        int B = 0, G = 0, R = 0;

        int ind = 0;
        while (line[ind] >= '0' && line[ind] <= '9')
        {
            B = B * 10 + line[ind] - '0';
            ind++;
        }
        ind++;

        while (line[ind] >= '0' && line[ind] <= '9')
        {
            G = G * 10 + line[ind] - '0';
            ind++;
        }
        ind++;

        while (line[ind] >= '0' && line[ind] <= '9')
        {
            R = R * 10 + line[ind] - '0';
            ind++;
        }
        ind++;

        size_t colorInd = R * 256 * 256 + G * 256 + B;
        if (line[ind] == '1')
        {
            SkinRecords++;

            model.skinMapping[colorInd]++;
        }
        else
        {
            nonSkinRecords++;

            model.NonSkinMapping[colorInd]++;
        }
    }

    dataset.close();

    model.totalRecords = SkinRecords + nonSkinRecords;
    model.skinRecords = SkinRecords;
    model.non_skinRecords = nonSkinRecords;

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

bool isHumanSkin(int B, int G, int R, Model &model)
{
    const int LAPLACE = 1;
    const int K = 256;

    double prior_probability_skin = model.skinRecords / model.totalRecords;
    double prior_probability_nonSkin = model.non_skinRecords / model.totalRecords;

    size_t colorInd = R * 256 * 256 + G * 256 + B;
    double P_Skin_given_pixel = prior_probability_skin * (model.skinMapping[colorInd] / model.skinRecords);

    double P_nonSkin_given_pixel = prior_probability_nonSkin * (model.NonSkinMapping[colorInd] / model.non_skinRecords);

    double ratio = P_Skin_given_pixel / P_nonSkin_given_pixel;

    return ratio >= 0.4;
}

bool hasSkin(fs::path filePath, Model &model)
{
    // If it has already a cache
    if (fs::exists(getExtractedSkinPath(filePath)))
    {
        cout << "Retrieving from cache..." << nl << nl;
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

    cout << "BMP Loaded Successfully!" << nl;
    cout << "Width: " << width << ", Height: " << height << nl;

    // Example: print first pixel RGB
    cout << "First pixel RGB: "
         << (int)image[0][0].r << " "
         << (int)image[0][0].g << " "
         << (int)image[0][0].b << nl << nl;

    bool gotSkin = 0;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            if (isHumanSkin(image[i][j].b, image[i][j].g, image[i][j].r, model))
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

void searchSkin(fs::path root, bool isRecursive)
{
    if (!fs::exists(root))
    {
        cout << "" << root << " this root path doesnt exist..." << nl;
        return;
    }

    Model model = trainModel();
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
            cout << '(' << i + 1 << ')' << " " << foundSkinPaths[i] << nl;
        cout << nl;

        int select;
        while (1)
        {
            cout << "Which one do you want to open? (0 to exit)" << nl;
            cout << "=> ";
            cin >> select;

            if (!select)
                break;

            cout << "Opening the " << select << "th image...." << nl;
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