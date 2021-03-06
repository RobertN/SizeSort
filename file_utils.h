#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "dir_entry.h"


size_t retrieveSize(const std::string& filename);
FileNode* createFileTree(const std::string& path);

#endif // FILE_UTILS_H
