//
//  main.cpp
//  SizeSort
//
//  Created by Robert Norlander on 11/14/12.
//  Copyright (c) 2012 Robert Norlander. All rights reserved.
//

#include <cstdio>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include "tree.h"
#include "dir_entry.h"

typedef TreeNode<DirectoryEntry> FileNode;

class CalculateSizeVisitor : public TreeVisitor<DirectoryEntry>
{
public:
    CalculateSizeVisitor() : total_size(0), number_of_files(0) {}

    virtual void visit(DirectoryEntry* data)
    {
        total_size += data->getSize();
        number_of_files++;
    }

    void printResults()
    {
        printf("Tree size: ");
        if (total_size > 10*1024)
            printf("%lu kB\n", total_size / 1024);
        else if (total_size > 100*1024)
            printf("%lu MB\n", total_size / (1024*1024));
        else
            printf("%lu B\n", total_size);
    }

    size_t getCalculatedSize() const { return total_size; }
    size_t getNumberOfFiles() const { return number_of_files; }

private:
    size_t total_size;
    size_t number_of_files;
};

void test_generic_tree()
{
    printf("[test_generic_tree]");

    FileNode* tree = new FileNode(new DirectoryEntry("/"));
    tree->addChild(new FileNode(new DirectoryEntry("passwd", 40)));
    CalculateSizeVisitor visitor;
    tree->visit(visitor);
    delete tree;

    size_t size = visitor.getCalculatedSize();
    if (size != 40) {
        printf(" size != 40, actual: %lu\n", size);
    } else {
        printf(" OK\n");
    }
}

void run_tests()
{
    test_generic_tree();
}

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

int main(int argc, const char * argv[])
{
    if (argc < 2) {
        printf("Usage: %s [options] [directory]\n\n", argv[0]);
        printf("Options:\n");
        printf(" -t: Run tests\n");
        printf(" -s: Calculate size for given directory\n");
        return 1;
    }

    if (strcmp(argv[1], "-t") == 0) {
        run_tests();
    } else if (strcmp(argv[1], "-s") == 0) {
        FileNode* tree = createFileTree(argv[1]);
        CalculateSizeVisitor visitor;
        tree->visit(visitor);
        visitor.printResults();
        delete tree;
    }

    return 0;
}
