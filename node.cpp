#include "node.h"
#include <iostream>
#include <sstream>

#include <stack>

// Workaround for appending an unsigned int to a string
std::string operator+(std::string const&a, unsigned int b)
{
    std::ostringstream oss;
    oss << a << b;
    return oss.str();
}

// Add a child to this node
Node* Node::addChild()
{
    Node* c = new Node();

    c->parent = this;
    c->depth = depth + 1;

    children.push_back(c);
    return c;
}

// Print out the tree
void Node::print()
{
    // Spacing 
    for (int i = 0; i < depth; ++i)
        std::cout << ".";

    if (isRoot)
        std::cout << "ROOT" << std::endl;
    else if (isCut)
        std::cout << "CUT" << std::endl;
    else if (isLetter)
        std::cout << letter << std::endl;

    std::list<Node*>::iterator it = children.begin();
    for ( ; it != children.end(); ++it )
        (*it)->print();
}

// Produce an output string
std::string Node::toString()
{
    std::string result = "";

    if ( isCut || isRoot )
    {
        result = result + children.size();

        std::list<Node*>::reverse_iterator it = children.rbegin();
        for ( ; it != children.rend(); ++it )
            result += (*it)->toString();
    }
    else if ( isLetter )
        result += letter;

    return result;
}

// Sort the children into a standardized ordering
void Node::sort()
{
    // Test if this actually needs to be sorted
    if ( children.size() > 1 )
    {
        // Perform the sort
        std::list<std::string> letterList;
        std::list<std::string> cutList;

        std::list<Node*>::iterator it = children.begin();
        for ( ; it != children.end(); ++it )
        {
            if ( (*it)->cut() )
                cutList.push_back( (*it)->toString() );
            else
                letterList.push_back( (*it)->toString() );
        }

        std::list<std::string>::iterator its;

        letterList.sort();
        cutList.sort();

        // Combine the two lists by putting letters at the front
        std::list<std::string> combined = letterList;
        for ( its = cutList.begin(); its != cutList.end(); ++its )
            combined.push_back( *its );

        // Make our children list reflect this new ordering
        std::list<Node*> newOrdering;
        for ( its = combined.begin(); its != combined.end(); ++its )
        {
            std::string target = (*its);

            std::list<Node*>::iterator itn = children.begin();
            for ( ; itn != children.end(); ++itn )
            {
                std::string potential = (*itn)->toString();
                if ( target == potential )
                    newOrdering.push_back( (*itn) );
            }
        }

        children = newOrdering;
    }
}

// Reorders the tree so that the entire graph is standardized
void Node::standardize(Node* root)
{
    // Find a proper ordering (using DFS)
    std::stack<Node*> stack;
    std::list<Node*> ordering;

    stack.push(root);
    while ( !stack.empty() )
    {
        Node* curr = stack.top();
        stack.pop();

        ordering.push_front( curr );
        
        // Push the next nodes to the stack
        std::list<Node*> c = curr->getChildren();
        std::list<Node*>::iterator it = c.begin();

        for ( ; it != c.end(); ++it )
            stack.push( *it );
    }

    // Sort based on that ordering
    std::list<Node*>::iterator it = ordering.begin();
    for( ; it != ordering.end(); ++it )
        (*it)->sort();
}

/*
 * Attempts to construct a tree from the given graph string.
 *
 * Params
 *      s: the string to parse ( e.g. 121AB )
 *
 * Returns
 *      Node*: a pointer to the root node of the tree if successful
 *      NULL: if input string is invalid
*/
Node* Node::parseFromString( std::string s )
{
    // Initialize the stack
    std::stack<Node*> stack;
    Node* root = new Node();
    stack.push(root);

    // Loop through the string char by char
    for ( unsigned int pos = 0; pos < s.length(); ++pos )
    {
        // Bad string
        if ( stack.empty() )
        {
            std::cerr << "ERROR: too many nodes\n";
            return NULL;
        }

        // Determine which node needs to be set
        Node* curr = stack.top();
        stack.pop();

        // Check the current char at pos
        if ( isdigit( s[pos] ) ) // Integers indicate cuts
        {
            curr->setCut();

            // Push kids
            for (int i = 0; i < s[pos] - '0'; ++i)
                stack.push(curr->addChild());
        }
        else // Letters indicate statements
            curr->setLetter(s[pos]);
    }

    // Bad string
    if ( !stack.empty() )
    {
        std::cerr << "ERROR: not enough nodes\n";
        return NULL;
    }


    // Make sure the root is actually a root node
    root->setRoot();

    return root;
}
