#ifndef TREE_H
#define TREE_H

#include <vector>

template <class T > class TreeVisitor
{
public:
    virtual void visit(T* data) = 0;
};


template <class T> class TreeNode
{
    typedef std::vector< TreeNode<T>* > NodeList;
public:
    TreeNode(T* data) : data(data) {}
    ~TreeNode()
    {
        typename NodeList::iterator it = children.begin();
        while (it != children.end()) {
            TreeNode<T>* node = *it;
            delete node;
            it++;
        }
        delete data;
    }

    void addChild(TreeNode<T>* node) { children.push_back(node); }
    void visit(TreeVisitor<T>& visitor)
    {
        typename NodeList::iterator it = children.begin();
        while (it != children.end()) {
            TreeNode<T>* node = *it;
            node->visit(visitor);
            it++;
        }
        visitor.visit(data); 
    }

protected:
    NodeList children;
    T* data;
};

#endif // TREE_H
