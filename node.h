#ifndef NODE_H
#define NODE_H

#include <list>
#include <string>

// A basic storage class for an existential graph tree
class Node
{
public:
    /* Constructor */
    Node() : 
        parent(NULL),
        depth(0),
        isLetter(false),
        isCut(false),
        isRoot(false) {}

    /* Add new nodes */
    Node* addChild();

    /* Set type */
    void setAsLetter( char c );
    void setAsCut();
    void setAsRoot();
    
    /* Printing functions */
    void print();
    std::string toString();

    /* Input from string */
    static Node* parseFromString( std::string in );

    /* Standardization */
    static void standardize( Node* root );
    void updateStdString();
    void sortChildren();
    //void sort();
    //void updateStdString();

private:
    Node* parent;
    std::list<Node*> children;

    /* Extra details */
    char letter;
    int depth;

    /* Bool */
    bool isLetter;
    bool isCut;
    bool isRoot;

    /* Standardization */
    std::string stdString;
};

#endif // NODE_H
