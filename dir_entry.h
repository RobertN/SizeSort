#ifndef DIR_ENTRY_H
#define DIR_ENTRY_H

#include <string>

class DirectoryEntry
{
public:
    DirectoryEntry(const std::string& name)
        : path(name), size(0), directory(true) {}
    DirectoryEntry(const std::string& path, size_t size)
        : path(path), size(size), directory(false) {}
    DirectoryEntry(const DirectoryEntry& other)
    {
        path = other.path;
        size = other.size;
    }

    const std::string& getName() const { return path; }
    size_t getSize() { return size; }
    bool isDirectory() { return directory; }

private:
    std::string path;
    size_t size;
    bool directory;
};



#endif // DIR_ENTRY_H
