//
//  main.cpp
//  SizeSort
//
//  Created by Robert Norlander on 11/14/12.
//  Copyright (c) 2012 Robert Norlander. All rights reserved.
//

#include <cstdio>
#include "dir_tree.h"

int main(int argc, const char * argv[])
{
    if (argc < 2) {
        printf("usage: %s directory\n", argv[0]);
        return -1;
    }

    printf("Scanning directory: %s\n", argv[1]);
    DirTree dt(argv[1]);
    dt.print();

    return 0;
}
