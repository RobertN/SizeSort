//
//  dir_tree.h
//  SizeSort
//
//  Created by Robert Norlander on 11/16/12.
//  Copyright (c) 2012 Robert Norlander. All rights reserved.
//

#ifndef __SizeSort__dir_tree__
#define __SizeSort__dir_tree__

#include <string>
#include <vector>

using std::string;
using std::vector;

class DirTree
{
public:
    DirTree(const string& path);
    DirTree(const DirTree& dt);
    virtual ~DirTree() {}

    void print(int print_level = 0);
    string getPath() const { return path; }
    size_t calculateSize();

private:
    void scanFiles();
    size_t retrieveSize(const string& filename);

    string path;
    vector<DirTree> children;
    size_t size, children_size;
};

#endif /* defined(__SizeSort__dir_tree__) */
