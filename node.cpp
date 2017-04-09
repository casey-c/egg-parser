#include "node.h"
#include <iostream>
#include <sstream>

#include <stack>
#include <vector>
#include <utility>
#include <algorithm>

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

// Helper comparator function
bool comparePairs( const std::pair< std::string, Node* >& first,
                   const std::pair< std::string, Node* >& second )
{
    return first.first.compare( second.first ) > 0;
    //return first.first < second.first;
}

// Sort the children into a standardized ordering
void Node::sort()
{
    // No need to sort unless at least 2 kids
    if ( children.size() > 1 )
    {
        // Perform the sort
        std::vector< std::pair< std::string, Node* > > letterVec;
        std::vector< std::pair< std::string, Node* > > cutVec;

        std::list<Node*>::iterator it = children.begin();
        for ( ; it != children.end(); ++it )
        {
            Node* curr = (*it);

            std::pair< std::string, Node* > currPair;
            currPair.first = curr->toString();
            currPair.second = curr;

            if ( curr->cut() )
                cutVec.push_back( currPair );
            else
                letterVec.push_back( currPair );
        }

        // Perform the sort by their string representation
        std::sort( letterVec.begin(), letterVec.end(), comparePairs );
        std::sort( cutVec.begin(), cutVec.end(), comparePairs );

        // Combine the two lists with letters at the front
        children.clear();

        std::vector< std::pair< std::string, Node* > >::iterator itp;
        for ( itp = letterVec.begin(); itp != letterVec.end(); ++itp )
            children.push_back( (*itp).second );
        for ( itp = cutVec.begin(); itp != cutVec.end(); ++itp )
            children.push_back( (*itp).second );
    }
}

// Standardize an entire graph, starting with root
void Node::standardize(Node* root)
{
    // Parent's depend on their children, so we must use DFS to produce a valid
    // ordering.
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

    // Now that we have an ordering where all children come before their
    // parents, we can call the sort routine.
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
