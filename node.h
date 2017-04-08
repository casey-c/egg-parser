#ifndef NODE_H
#define NODE_H

#include <list>
#include <string>

// A basic storage class for an existential graph tree
class Node
{
public:
    // Constructor
    Node() : 
        parent(NULL),
        depth(0),
        isLetter(false),
        isCut(false),
        isRoot(false) {}

    // Needed for standardization
    const std::list<Node*> getChildren() const { return children; }
    bool cut() { return isCut; }

    // Add new child node
    Node* addChild();

    // Set type
    void setLetter(char c) { letter = c; isLetter = true; }
    void setCut() { isCut = true; }
    void setRoot() { isRoot = true; }
    

    // Print
    void print();

    // Prints out the graph string using DFS
    std::string toString();

    // Sort children to follow a standard format
    static void standardize( Node* root );
    void sort();

private:
    Node* parent;
    std::list<Node*> children;

    // ID's
    char letter;
    int depth;

    // Bool
    bool isLetter;
    bool isCut;
    bool isRoot;
};

#endif // NODE_H
