#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
using namespace std;
namespace fs = std::filesystem;
#define nl "\n"
#define ll long long

const fs::path dataset_src = "/home/ashik/Desktop/Sem 3/Software-Project-Lab/Dataset/Sample_labelled_pixels/data_kaggle.txt";
const fs::path model_2_FilePath = "/home/ashik/Desktop/Sem 3/Software-Project-Lab/Models/model_2.txt";

/*
    Title : Training Model using Sample labelled pixels dataset

    Feature :
    It trains the model-2 using the dataset where there are thousands of
    pixels data with label 0/1.
    0 refers NON_SKIN and 1 refers SKIN
*/

int main()
{
    ofstream model_2_File(model_2_FilePath);

    if (!model_2_File.is_open())
    {
        cout << "ERROR : Cannot open the model file for training..." << nl;
        return 1;
    }

    long double ε = 1.0e-8;
    vector<long double> pixel_given_skin_arr(256 * 256 * 256, 0), pixel_given_nonSkin_arr(256 * 256 * 256, ε);
    long double totalSkin = 0, totalNonSkin = 0;

    ifstream dataset(dataset_src);

    if (!dataset.is_open())
    {
        cout << "Can't open the dataset file for model-2" << nl;
        return 1;
    }

    string line;

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
            totalSkin++;

            pixel_given_skin_arr[colorInd]++;
        }
        else
        {
            totalNonSkin++;

            pixel_given_nonSkin_arr[colorInd]++;
        }
    }

    dataset.close();

    for (int i = 0; i < 256 * 256 * 256; i++)
    {
        pixel_given_skin_arr[i] = pixel_given_skin_arr[i] / totalSkin;
        pixel_given_nonSkin_arr[i] = pixel_given_nonSkin_arr[i] / totalNonSkin;

        long double ratio = pixel_given_skin_arr[i] / pixel_given_nonSkin_arr[i];

        model_2_File << fixed << setprecision(20) << ratio << nl;
    }

    cout << "Model-2 is trained to fight !!" << nl;
    model_2_File.close();

    return 0;
}