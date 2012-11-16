//
//  dir_tree.cpp
//  SizeSort
//
//  Created by Robert Norlander on 11/16/12.
//  Copyright (c) 2012 Robert Norlander. All rights reserved.
//

#include <dirent.h>
#include <sys/stat.h>

#include "dir_tree.h"

DirTree::DirTree(const string& path)
    : path(path)
    , size(0)
    , children_size(0)
{
    scanFiles();
}

DirTree::DirTree(const DirTree& dt)
{
    path = dt.path;
    children = dt.children;
    size = dt.size;
}

size_t DirTree::calculateSize()
{
    children_size = 0;
		vector<DirTree>::iterator it = children.begin();
    for ( ; it != children.end(); it++)
        children_size += (*it).calculateSize();
    return children_size + size;
}

void DirTree::print(int print_level)
{
    for (int i = 0; i < print_level; i++)
        printf(" ");
    calculateSize();
    printf("%s [size=%lu B]\n",
           path.c_str(),
           size+children_size);
    vector<DirTree>::iterator it = children.begin();
    for ( ; it != children.end(); it++)
        (*it).print();
}

void DirTree::scanFiles()
{
    struct stat st_buf;
    DIR *dir = 0;
    struct dirent *ent = 0;
    dir = opendir(path.c_str());
    if (dir) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 ||
                strcmp(ent->d_name, "..") == 0)
                continue;
            string abs_path = path + "/" + ent->d_name;
            if (stat(abs_path.c_str(), &st_buf) != 0) {
                perror("Could not stat file");
                continue;
            }
            if (S_ISDIR(st_buf.st_mode))
                children.push_back(DirTree(abs_path));
            else
                size = retrieveSize(abs_path);
        }
    } else
        perror("Could not open directory");
}

size_t DirTree::retrieveSize(const string& filename)
{
    FILE *file = fopen(filename.c_str(), "rb");
    if (!file) {
        string error = "Could not open " + filename;
        perror(error.c_str());
        return 0;
    }
    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    if (file_size == 0L) {
        perror("Could not tell size of file");
        return 0;
    }
    fclose(file);
    return file_size;
}
