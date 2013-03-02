#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>
#include <string>
#include "file_utils.h"
#include "dir_entry.h"

class ScopedFile
{
public:
    ScopedFile() : file(0) {}

    ~ScopedFile()
    {
        if (file)
            fclose(file);
    }

    bool open(const std::string& filename)
    {
        file = fopen(filename.c_str(), "rb");
        if (!file)
            return false;
        return true;
    }

    FILE* fd() { return file; }

private:
    FILE* file;
};



size_t retrieveSize(const std::string& filename)
{
    ScopedFile file;
    file.open(filename);
    if (!file.open(filename)) {
        perror("Could not open file");
        return 0;
    }

    fseek(file.fd(), 0L, SEEK_END);
    long file_size = ftell(file.fd());
    if (file_size == 0L)
        return 0;
    return file_size;
}

FileNode* createFileTree(const std::string& path)
{
    FileNode* tree = new FileNode(new DirectoryEntry(path));
    std::vector<std::string> directories;
    struct stat st_buf;
    DIR* dir = 0;
    struct dirent* ent = 0;
    dir = opendir(path.c_str());
    if (dir) {
        while ((ent = readdir(dir)) != 0) {
            if (strcmp(ent->d_name, ".") == 0 ||
                strcmp(ent->d_name, "..") == 0)
                continue;

            std::string abs_path = path + "/" + ent->d_name;
            if (stat(abs_path.c_str(), &st_buf) == 0) {
                if (S_ISDIR(st_buf.st_mode)) {
                    tree->addChild(createFileTree(abs_path));
                }
                else
                {
                    tree->addChild(new FileNode(new DirectoryEntry(
                                    abs_path.c_str(),
                                    retrieveSize(abs_path))));
                }
            } 
        }
        closedir(dir);
    }
    return tree;
}


