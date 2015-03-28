#include "fileutil.hpp"

#ifdef _WIN32
#include <windows.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <strsafe.h>
#else
#include <dirent.h>
#endif

#include <iostream>

#include <format.h>

namespace common {
namespace util {
namespace file {

/// Return the file name in a path string.
std::string fileFromPath(std::string str) {
    unsigned found = str.find_last_of("/\\");
    return str.substr(found + 1);
}

std::vector<std::string> getDirectoryContents(std::string const & path) {
    std::vector<std::string> entries;
#ifdef _WIN32
    WIN32_FIND_DATA ffd;
    TCHAR szDir[MAX_PATH];
    size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    // Check that the input path plus 3 is not longer than MAX_PATH.
    // Three characters are for the "\*" plus NULL appended below.

    StringCchLength(path.c_str(), MAX_PATH, &length_of_arg);

    if (length_of_arg > (MAX_PATH - 3)) {
        throw std::runtime_error("Directory path is too long.");
    }

    // Prepare string for use with FindFile functions.  First, copy the
    // string to a buffer, then append '\*' to the directory name.

    StringCchCopy(szDir, MAX_PATH, path.c_str());
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

    // Find the first file in the directory.

    hFind = FindFirstFile(szDir, &ffd);

    if (INVALID_HANDLE_VALUE == hFind) {
        throw std::runtime_error("Invalid handle.");
    }

    // List all the files in the directory with some info about them.

    do {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // Do something for directories?
        } else {
            entries.push_back(ffd.cFileName);
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES) {
        throw std::runtime_error("Some kind of error, dunno");
    }

    FindClose(hFind);
#else
    DIR * dir;
    struct dirent * ent;

    if ((dir = opendir(path.c_str())) == nullptr) {
        throw std::runtime_error(
            fmt::format("Couldn't open directory \"{}\"", path));
    }

    while ((ent = readdir(dir)) != nullptr) {
        entries.push_back(ent->d_name);
    }
#endif
    return entries;
}

} // namespace file
} // namespace util
} // namespace common
