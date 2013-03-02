#include "tree.h"
#include "dir_entry.h"
#include "calculate_size_visitor.h"

typedef TreeNode<DirectoryEntry> FileNode;

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
