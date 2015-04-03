#include "tarlib.hpp"

#include "common/util/fileutil.hpp"

Tar::Tar(std::string path) {
    std::ifstream stream(path, std::ios::in | std::ios::binary);

    if (!stream.is_open()) {
        throw std::runtime_error("(Tar) Error opening file " + path);
    }

    // Copypasted from stream.cpp

    auto originalPos = stream.tellg();
    stream.seekg(0, std::ios::end);
    auto end = stream.tellg();
    auto size = end - originalPos;
    m_data.resize(size);
    stream.seekg(originalPos);
    stream.read(m_data.data(), size);

    std::streampos position;

    char blank[ENTRYSIZE];
    memset(blank, 0, ENTRYSIZE);

    while (position < size) {
        if (std::memcmp(m_data.data() + position, blank, ENTRYSIZE) == 0) {
            break;
        }

        // Fill in the tar entry.
        TarEntry* entry = new TarEntry[sizeof(TarEntry)];
        std::memcpy(entry, m_data.data() + position, ENTRYSIZE);

        // Set certain variables.
        // TODO: Implement reading of file size if base-256 encoding.
        size_t i;
        for (i = 0; i < 11; i++) {
            if (i != 0) {
                break;
            }
        }
        entry->file_size = std::stoi(entry->size, &i, 8);

        entry->contents.resize(entry->file_size);
        std::memcpy(const_cast<char*>(entry->contents.data()),
                    m_data.data() + (long) position + ENTRYSIZE,
                    entry->file_size + 1);
        if (entry->file_size % 512 == 0) {
            position += ENTRYSIZE + entry->file_size;
        } else {
            position += ENTRYSIZE + (((long) entry->file_size / ENTRYSIZE) + 1)
                        * ENTRYSIZE;
        }
        m_entries.push_back(entry);
    }
}

std::vector<TarEntry*> Tar::getEntries() const {
    return m_entries;
}
