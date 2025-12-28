#include "../Headers/cache.h"
#include "../Headers/utility.h"
using namespace std;
namespace fs = std::filesystem;
#define nl "\n"

fs::path get_corresponding_cache_path(fs::path pathName)
{
    if (pathName.extension() != ".pdf")
        throw invalid_argument("Cannot get corresponding cache path as the file is not pdf");

    pathName = fs::absolute(pathName);

    string temp = pathName;

    string temp_cache_path = temp.substr(1, temp.size() - 4);
    temp_cache_path += "txt";
    fs::path cachePath = "cache/" + temp_cache_path;

    return cachePath;
}

bool has_cache_valid_copy(fs::path pathName)
{
    fs::path cachePath = get_corresponding_cache_path(pathName);

    if (fs::exists(cachePath))
    {
        ifstream cacheFile(cachePath);

        string lastModificationTime_cached_str;
        getline(cacheFile, lastModificationTime_cached_str);
        time_t lastModificationTime_cached = string_to_long_long(lastModificationTime_cached_str);
        cacheFile.close();

        time_t lastModificationTime_OG = convert_to_seconds_since_epoch(fs::last_write_time(pathName));

        if (lastModificationTime_cached == lastModificationTime_OG)
            return true;
        else
        {
            fs::remove(cachePath);
            return false;
        }
    }
    else
        return false;
}

vector<vector<string>> retrieveCache(fs::path pathName)
{ // assuming that this pathName has a valid cache copy
    fs::path cachePath = get_corresponding_cache_path(pathName);

    vector<vector<string>> fileContent;

    ifstream file(cachePath);

    if (!file)
    {
        std::cerr << "Failed to open file\n";
        return fileContent;
    }

    string lastModification;
    getline(file, lastModification);

    string numOfPages;
    getline(file, numOfPages);

    string line;
    vector<string> lines;

    while (getline(file, line))
    {
        if (line.size())
            lines.push_back(line);
        else
        {
            fileContent.push_back(lines);
            lines.clear();
        }
    }

    file.close();

    return fileContent;
}

void insertCache(fs::path pathName, vector<vector<string>> dataToBeCached)
{
    fs::path cachePath = get_corresponding_cache_path(pathName);

    fs::create_directories(cachePath.parent_path()); // creating the parent dir first
    ofstream cacheFile(cachePath);

    time_t lastModificationTime = convert_to_seconds_since_epoch(fs::last_write_time(pathName));

    cacheFile << lastModificationTime << nl;
    cacheFile << dataToBeCached.size() << nl; // writing #pages

    for (auto pageLines : dataToBeCached)
    {
        for (auto line : pageLines)
            cacheFile << line << nl;

        cacheFile << nl;
    }

    cacheFile.close();
}

void clearCache()
{
    fs::path cacheRoot = "cache";

    uintmax_t count = 0;

    if (fs::exists(cacheRoot))
        for (const auto &entry : fs::directory_iterator(cacheRoot, fs::directory_options::skip_permission_denied))
            count = fs::remove_all(entry.path());

    cout << "Deleted total '" << count << "' caches." << nl;
}

void updateCache()
{
    fs::path cacheRoot = "cache";

    uintmax_t count = 0;

    if (fs::exists(cacheRoot))
        for (fs::recursive_directory_iterator it(cacheRoot, fs::directory_options::skip_permission_denied), end; it != end; ++it)
        {
            auto entry = *it;

            if (entry.path().extension() != ".txt")
            {
                // checking if the original directory exists
                string entryPath_str = entry.path();
                entryPath_str.erase(0, 5);
                fs::path og_path = entryPath_str;

                if (!fs::exists(og_path))
                {
                    count += remove_all(entry.path());
                    it.disable_recursion_pending();
                }
            }
            else
            {
                // checking if the original file modified

                // getting corresponding original path
                string entryPath_str = entry.path();
                string temp_og_path = entryPath_str.substr(5, entryPath_str.size() - 8);
                temp_og_path += "pdf";
                fs::path og_path = temp_og_path;

                // getting last modification time stored in the cache
                ifstream cacheFile(entry.path());
                string last_modified_record_str;
                getline(cacheFile, last_modified_record_str);
                cacheFile.close();

                time_t last_modified_record = string_to_long_long(last_modified_record_str);

                if (fs::exists(og_path))
                {
                    time_t last_modified_og = convert_to_seconds_since_epoch(fs::last_write_time(og_path));

                    if (last_modified_og != last_modified_record)
                    {
                        remove(entry.path());
                        count++;
                    }
                }
                else
                {
                    remove(entry.path());
                    count++;
                }
            }
        }

    cout << "Deleted '" << count << "' caches." << nl;
}