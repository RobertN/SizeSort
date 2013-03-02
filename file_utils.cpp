#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>
#include <string>
#include "dir_entry.h"

size_t retrieveSize(const std::string& filename)
{
    FILE *file = fopen(filename.c_str(), "rb");
    if (!file) {
        std::string error = "Could not open " + filename;
        fclose(file);
        perror(error.c_str());
        return 0;
    }
    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    if (file_size == 0L) {
        fclose(file);
        return 0;
    }
    fclose(file);
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


