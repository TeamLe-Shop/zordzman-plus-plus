#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <cstring>

#define ENTRYSIZE 512
#define NAMESIZE 100
#define TUNMLEN 32
#define TGNMLEN 32

struct TarEntry;
class  Tar;

struct TarEntry {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char typeflag[1];
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char pad[12];
    size_t file_size;
    std::string contents;
};

typedef struct TarEntry TarEntry;

class Tar {
public:
    /// Opens a tar file for reading.
    Tar(std::string path);

    /// Return all entries.
    std::vector<TarEntry*> getEntries() const;

private:

    std::vector<TarEntry*> m_entries;
    std::vector<char> m_data;
};
