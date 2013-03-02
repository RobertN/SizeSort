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
#include "file_utils.h"
#include "calculate_size_visitor.h"
#include "tests.h"

void usage(const char* name)
{
    printf("Usage: %s [options] [directory]\n\n", name);
    printf("Options:\n");
    printf(" -t: Run tests\n");
    printf(" -s: Calculate size for given directory\n");
}

int main(int argc, const char * argv[])
{
    if (argc == 2 && strcmp(argv[1], "-t") == 0) {
        run_tests();
    } else if (argc == 3 && strcmp(argv[1], "-s") == 0) {
        FileNode* tree = createFileTree(argv[2]);
        CalculateSizeVisitor visitor;
        tree->visit(visitor);
        printf("%s: ", argv[2]);
        visitor.printResults();
        delete tree;
    } else {
        usage(argv[0]);
        return 1;
    }

    return 0;
}
