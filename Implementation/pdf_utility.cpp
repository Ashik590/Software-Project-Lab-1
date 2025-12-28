#include "../Headers/pdf_utility.h"
using namespace std;
namespace fs = std::filesystem;

bool isPDF(fs::path filepath)
{
    // Check if is a regular file
    if (!fs::is_regular_file(filepath))
    {
        return false;
    }

    // Open file in binary mode
    ifstream file(filepath, ios::binary);
    if (!file)
        return false;

    char header[5];
    file.read(header, 5); // read first 5 bytes
    if (!file)
        return false;
    file.close();

    // Check PDF magic number
    return header[0] == '%' && header[1] == 'P' && header[2] == 'D' &&
           header[3] == 'F' && header[4] == '-';
}

void openPDF(string path, int page)
{
    string command = "firefox \"" + path + "#page=" + to_string(page) + "\" &";
    system(command.c_str());
}