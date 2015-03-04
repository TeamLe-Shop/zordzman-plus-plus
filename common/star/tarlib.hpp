#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

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
    char chksum[8];
    char linkflag;
    char linkname[100];
    char magic[8];
    char uname[TUNMLEN];
    char gname[TGNMLEN];
    char devmajor[8];
    char devminor[8];
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

static int octal_string_to_int(char* current_char, unsigned int size){
    unsigned int output = 0;
    while(size > 0){
        output = output * 8 + *current_char - '0';
        current_char++;
        size--;
    }
    return output;
}
