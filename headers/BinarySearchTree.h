#include <iostream>
using namespace std;

#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

template <typename TYPE>
class BinarySearchTree;

//*************************************************
// Node Declaration of Definition
//*************************************************
template <typename TYPE>
class Node
{
    TYPE key;
    Node<TYPE> *left;
    Node<TYPE> *right;
    Node<TYPE> *parent;
    Node() : left(nullptr), right(nullptr), parent(nullptr){};
    Node(TYPE key, Node<TYPE> *p) : key(key), left(nullptr), right(nullptr), parent(p){};
    Node(TYPE key, Node<TYPE> *l, Node<TYPE> *r, Node<TYPE> *p) : key(key), left(l), right(r), parent(p){};
    Node<TYPE> *operator=(Node<TYPE> *node)
    {
        key = node->key;
        left = node->left;
        right = node->right;
        parent = node->parent;
    };
    // friend ostream &operator<<(ostream &os, Node<TYPE> *node)
    // {
    //     os << node->key;
    //     return os;
    // }
    friend class BinarySearchTree<TYPE>;
};

//*************************************************
// Binary Search Tree Declaration and Definition
//*************************************************
template <typename TYPE>
class BinarySearchTree
{
public:
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Constructors, Destructors, and Operators
    /////////////////////////////////////////////////////////////////////////////////////////////
    BinarySearchTree() : root(nullptr) { cout << "Creating tree." << endl; };
    BinarySearchTree(TYPE key) { Insert(key); };
    BinarySearchTree(Node<TYPE> *tree)
    {
        CopyTree(tree->root, root, root);
        root = Search(tree->root->key);
        cursor = Search(tree->cursor->key);
    }
    ~BinarySearchTree()
    {
        cout << "Deleting tree..." << endl;
        ClearList();
        cout << "Tree deleted." << endl;
    };
    void operator=(const BinarySearchTree<TYPE> *tree)
    {
        CopyTree(tree->root, root, root);
        root = Search(tree->root->key);
        cursor = Search(tree->cursor->key);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Primary Functions
    /////////////////////////////////////////////////////////////////////////////////////////////
    bool Insert(TYPE key) { return Insert(key, root, root); };
    bool Remove(TYPE key) { return Remove(key, root); };
    void ClearList()
    {
        ClearList(root);
        root = nullptr;
        cursor = nullptr;
    }
    bool EmptyList() { return (root == nullptr); }
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Search Functions
    /////////////////////////////////////////////////////////////////////////////////////////////
    Node<TYPE> *Search(TYPE key) { return Search(key, root); };
    void FindParentOf(TYPE key) const // untested
    {
        Node<TYPE> *child = Search(key);
        if (child != nullptr)
        {
            if (child->parent == nullptr)
                cout << "Node with data {" << key << "} is the root node.";
            else
                cout << "Node with key {" << child->parent->key << "} is the parent node of {" << key << "}";
        }
        else
            cout << "ERROR: Node not found with data {" << key << "}.";
    };
    void GetRoot() const { return root->key; }
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Cursor Functions
    /////////////////////////////////////////////////////////////////////////////////////////////
    void GoToBeginning() { cursor = findMin(root); }
    void GoToEnd() { cursor = findMax(root); }
    void GoToNext()
    {
        bool parentExists = (cursor->parent != nullptr); // makes sure parent exists
        if (cursor == findMax(root))                     // if last in tree, circle to first
            cursor = findMin(root);
        else if (cursor->right != nullptr) // if right exists, find the successor
            cursor = findMin(cursor->right);
        else if (parentExists && cursor == cursor->parent->left) // if parent exists and cursor is the left child, set cursor to parent
            cursor = cursor->parent;
        else if (parentExists)        // if parent exists and not left child
            GoToNext(cursor->parent); // find the parent that is next in succession
    }
    void GoToPrev()
    {
        bool parentExists = (cursor->parent != nullptr);
        if (cursor == findMin(root)) // if first in tree, loop to the back
            cursor = findMax(root);
        else if (cursor->left != nullptr) // if left isn't null, find the predecessor on the left subtree
            cursor = findMax(cursor->left);
        else if (parentExists && cursor == cursor->parent->right) // if parent exists and cursor is the right child, new cursor is parent
            cursor = cursor->parent;
        else if (parentExists)        // if parent exists but cursor is left child
            GoToPrev(cursor->parent); // loop through parents to find next lowest node
    }
    int AtCursor() { return cursor->key; }
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Print Functions
    /////////////////////////////////////////////////////////////////////////////////////////////
    void PrintInOrder() const
    {
        if (root == nullptr)
            Empty();
        else
        {
            cout << "< ";
            PrintInOrder(root);
            cout << ">";
        }
    };
    void PrintPreOrder() const
    {
        if (root == nullptr)
            Empty();
        else
        {
            cout << "< ";
            PrintPreOrder(root);
            cout << ">";
        }
    };
    void PrintPostOrder() const
    {
        if (root == nullptr)
            Empty();
        else
        {
            cout << "< ";
            PrintPostOrder(root);
            cout << ">";
        }
    };

private:
    Node<TYPE> *root;
    Node<TYPE> *cursor;
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Primary (and Search) Helper Functions
    /////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @param key The data wanted to be added to tree.
     * @param prev The node keeping track of the parent of the node to be inserted
     * @param subroot The node moving through the tree until nullptr has been found.
     * @def Inserts a new node into a binary search tree organized by lesser-than or greater-than.
     */
    bool Insert(TYPE key, Node<TYPE> *prev, Node<TYPE> *&subroot)
    {
        bool success = true;
        if (subroot == nullptr)
        {
            subroot = new Node(key, prev);
            cursor = subroot;
        }
        else if (key > subroot->key)
            Insert(key, subroot, subroot->right);
        else if (key < subroot->key)
            Insert(key, subroot, subroot->left);
        else if (key == subroot->key)
        {
            success = false;
            cout << "ERROR: Duplicates not allowed." << endl;
        }
        return success;
    }
    /**
     * @param key The data of the tree to be found and removed.
     * @param subroot The root compared to the key to determine whether the key is present in the tree.
     * @def Searches through a BST for a specific value and removes it from the tree.
     */
    bool Remove(TYPE key, Node<TYPE> *&subroot)
    {
        bool succeed = true;
        if (subroot == nullptr) // if key doesnt exist in bst or bst doesn't exist
        {
            cout << "ERROR: Failed to remove." << endl;
            cursor = findMax(root);
            succeed = false;
        }
        else if (key > subroot->key) // loops to the right if not found but greater
            Remove(key, subroot->right);
        else if (key < subroot->key) // loops to the left if not found but greater
            Remove(key, subroot->left);
        else if (key == subroot->key) // if found
        {
            if (subroot->left == nullptr) // if left doesn't exist, bump right root up
            {
                cursor = subroot;
                GoToNext();
                subroot = subroot->right;
            }
            else if (subroot->right == nullptr) // if right doesn't exist, bump left root up
            {
                cursor = subroot;
                GoToNext();
                subroot = subroot->left;
            }
            else // if both roots exist
            {
                cursor = subroot;
                GoToNext();
                Node<TYPE> *temp = findMin(subroot->right); // find the successor
                subroot->key = temp->key;                   // set current root (to be removed) to successor
                Remove(temp->key, subroot->right);          // remove the original successor now that its been moved up
            }
        }
        return succeed;
    }
    Node<TYPE> *Search(TYPE key, Node<TYPE> *subroot)
    {
        Node<TYPE> *found = nullptr;
        if (subroot == nullptr)
            cursor = findMax(root);
        else if (key == subroot->key)
            found = subroot;
        else if (key > subroot->key)
            found = Search(key, subroot->right);
        else if (key < subroot->key)
            found = Search(key, subroot->left);
        return found;
    };
    void ClearList(Node<TYPE> *&subroot) // Recursive delete function
    {
        if (subroot != nullptr)
        {
            // cout << subroot->key << " "; // debug
            ClearList(subroot->left);
            ClearList(subroot->right);
            delete subroot;
        }
    }
    Node<TYPE> *findMin(Node<TYPE> *subroot)
    {
        Node<TYPE> *found = subroot;
        if (subroot->left != nullptr)
            found = findMin(subroot->left);
        return found;
    }
    Node<TYPE> *findMax(Node<TYPE> *subroot)
    {
        Node<TYPE> *found = subroot;
        if (subroot->right != nullptr)
            found = findMax(subroot->right);
        return found;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Copy Helper
    /////////////////////////////////////////////////////////////////////////////////////////////
    void CopyTree(const Node<TYPE> *ogSubroot, Node<TYPE> *copySubroot, Node<TYPE> *prev)
    {
        if (ogSubroot != nullptr)
        {
            copySubroot = new Node<TYPE>;
            copySubroot->key = ogSubroot->key;
            copySubroot->parent = prev;
            CopyTree(ogSubroot->left, copySubroot->left, copySubroot);
            CopyTree(ogSubroot->right, copySubroot->right, copySubroot);
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Cursor Function Helpers
    /////////////////////////////////////////////////////////////////////////////////////////////
    void GoToNext(Node<TYPE> *subroot)
    {
        if (subroot->key < cursor->key)
            GoToNext(subroot->parent);
        else
            cursor = subroot;
    }
    void GoToPrev(Node<TYPE> *subroot)
    {
        if (subroot->key > cursor->key)
            GoToPrev(subroot->parent);
        else
            cursor = subroot;
    }
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Print Helpers
    /////////////////////////////////////////////////////////////////////////////////////////////
    void PrintInOrder(Node<TYPE> *subroot) const
    {
        if (subroot != nullptr)
        {
            PrintInOrder(subroot->left);
            PrintKey(subroot);
            PrintInOrder(subroot->right);
        }
    };
    void PrintPreOrder(Node<TYPE> *subroot) const
    {
        if (subroot != nullptr)
        {
            PrintKey(subroot);
            PrintPreOrder(subroot->left);
            PrintPreOrder(subroot->right);
        }
    };
    void PrintPostOrder(Node<TYPE> *subroot) const
    {
        if (subroot != nullptr)
        {
            PrintPostOrder(subroot->left);
            PrintPostOrder(subroot->right);
            PrintKey(subroot);
        }
    };
    void PrintKey(const Node<TYPE> *subroot) const
    {
        if (subroot == cursor)
            cout << "[" << subroot->key << "] ";
        else
            cout << subroot->key << " ";
    }
    void Empty() const { cout << "Tree is empty."; }
};

#endif