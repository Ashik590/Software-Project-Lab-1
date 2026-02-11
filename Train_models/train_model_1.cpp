#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb-image.h"
using namespace std;
namespace fs = std::filesystem;
#define nl "\n"
#define ll long long

const fs::path dataset_path = "/home/ashik/Desktop/Sem 3/Software-Project-Lab/Dataset/Sample_mask_images";
const fs::path model_1_path = "/home/ashik/Desktop/Sem 3/Software-Project-Lab/Models/model_1.txt";

/*
    Title : Training Model using Sample Mask Images

    Feature :

    It trains model-1 using the dataset of raw images with proper skin
    labelling.

    Requirements for Model-1 training :

    (1) There must be two directories of Images in the dataset (Who are going
    to be used to train the model) - One will have normal images and another
    will have the SKIN LABELLED images.

    (2) Must have to be same fileName (extension name is not included) of
        a normal image in Image directory and its skin image in Skin directory.
        as well as,
            - Width, Height
    (3) Assuming Skin Images (Labelled) are all pngs
*/

int main()
{
    fs::path imagePath = dataset_path / "Image";

    int width1, height1, channels1, width2, height2, channels2;
    ofstream model_1_file(model_1_path);

    if (!model_1_file.is_open())
    {
        cout << "ERROR : Cannot open model-1 file for training..." << nl;
        return 1;
    }

    long double ε = 1.0e-8;
    vector<long double> pixel_given_skin_arr(256 * 256 * 256, 0), pixel_given_nonSkin_arr(256 * 256 * 256, ε);
    long double totalSkin = 0, totalNonSkin = 0;

    for (const auto &entry : fs::directory_iterator(imagePath, fs::directory_options::skip_permission_denied))
    {
        string fileName = entry.path().filename().stem();
        string corresponding_skinFileName = fileName + ".png";

        fs::path corresponding_skinPath = dataset_path / "Skin" / corresponding_skinFileName;

        if (!fs::exists(corresponding_skinPath))
        {
            cout << "Cannot find corresponding extracted skin image for '" << entry.path() << "'" << nl;

            continue;
        }

        unsigned char *data_og = stbi_load(
            entry.path().string().c_str(), // file path
            &width1,                       // image width
            &height1,                      // image height
            &channels1,                    // number of channels
            3                              // 3 for forcing pixel in RGB and 0 for keeping original channels (ie. RGBA)
        );

        unsigned char *data_label = stbi_load(
            corresponding_skinPath.string().c_str(),
            &width2,
            &height2,
            &channels2,
            3);

        if (data_og == nullptr || data_label == nullptr)
        {
            cout << "ERROR: Failed to decode image data for '" << fileName << "'" << nl;
            if (data_og)
                stbi_image_free(data_og);
            if (data_label)
                stbi_image_free(data_label);
            continue;
        }

        if (height1 != height2 || width1 != width2)
        {
            cout << "Cannot find proper HEIGHT_WIDTH corresponding extracted skin image for '" << entry.path() << "'" << nl;
            if (data_og)
                stbi_image_free(data_og);
            if (data_label)
                stbi_image_free(data_label);
            continue;
        }

        for (int i = 0; i < height1; i++)
            for (int j = 0; j < width1; j++)
            {
                int ind = (i * width1 + j) * 3;

                unsigned int R1 = data_label[ind];
                unsigned int G1 = data_label[ind + 1];
                unsigned int B1 = data_label[ind + 2];

                unsigned int R2 = data_og[ind];
                unsigned int G2 = data_og[ind + 1];
                unsigned int B2 = data_og[ind + 2];

                long long pixelIndex = 256 * 256 * R2 + 256 * G2 + B2;

                if (R1 >= 230 && G1 >= 230 && B1 >= 230)
                {
                    pixel_given_skin_arr[pixelIndex]++;
                    totalSkin++;
                }
                else
                {
                    pixel_given_nonSkin_arr[pixelIndex]++;
                    totalNonSkin++;
                }
            }

        stbi_image_free(data_og);
        stbi_image_free(data_label);
    }

    for (int i = 0; i < 256 * 256 * 256; i++)
    {
        pixel_given_skin_arr[i] = pixel_given_skin_arr[i] / totalSkin;
        pixel_given_nonSkin_arr[i] = pixel_given_nonSkin_arr[i] / totalNonSkin;

        long double ratio = pixel_given_skin_arr[i] / pixel_given_nonSkin_arr[i];

        model_1_file << fixed << setprecision(20) << ratio << nl;
    }

    cout << "Model-1 is trained to fight !!" << nl;
    model_1_file.close();

    return 0;
}