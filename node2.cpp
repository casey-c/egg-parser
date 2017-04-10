#include "node2.h"
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

////////////////////////////
/// Set this node's type ///
////////////////////////////

// Letter
void Node::setAsLetter( char c )
{
    letter = c;
    isLetter = true;
    updateStdString();
}

// Cut
void Node::setAsCut()
{
    isCut = true;
    updateStdString();
}

// Root
void Node::setAsRoot()
{
    isRoot = true;
    updateStdString();
}


////////////////
/// Printing ///
////////////////

// Print out the tree
void Node::print()
{
    // Spacing 
    for (int i = 0; i < depth; ++i)
        std::cout << ".";

    // Elements
    if (isRoot)
        std::cout << "ROOT" << std::endl;
    else if (isCut)
        std::cout << "CUT" << std::endl;
    else if (isLetter)
        std::cout << letter << std::endl;

    // Children
    std::list<Node*>::iterator it = children.begin();
    for ( ; it != children.end(); ++it )
        (*it)->print();
}

// Produce an output string
std::string Node::toString()
{
    updateStdString();
    return stdString;
}

///////////////////////
/// Standardization ///
///////////////////////


// Helper comparator function
bool comparePairs( const std::pair< std::string, Node* >& first,
                   const std::pair< std::string, Node* >& second )
{
    //std::cout << "Comparing " << first.first << " with " << second.first
        //<< std::endl;

    std::string str1( first.first );
    std::string str2( second.first );

    std::reverse( str1.begin(), str1.end() );
    std::reverse( str2.begin(), str2.end() );

    // Sort by length too
    int l1 = str1.length();
    int l2 = str2.length();

    if (l1 == l2)
        return str1.compare( str2 ) < 0;
    else
        return l1 < l2;
}

// Updates the standard string
void Node::updateStdString()
{
    std::string result = "";

    if ( isLetter )
        result.append(1, letter);
    else
    {
        result = result + children.size();
        //std::list<Node*>::iterator it = children.begin();

        //for ( ; it != children.end(); ++it)
            //result += (*it)->stdString;
        std::list<Node*>::reverse_iterator it = children.rbegin();

        for ( ; it != children.rend(); ++it)
            result += (*it)->stdString;
    }

    stdString = result;

    // Recurse up the tree
    if ( parent != NULL )
        parent->updateStdString();
}

void Node::sortChildren()
{
    std::vector< std::pair< std::string, Node* > > letterVec;
    std::vector< std::pair< std::string, Node* > > cutVec;

    std::list<Node*>::iterator itn = children.begin();
    for ( ; itn!= children.end(); ++itn )
    {
        Node* child = (*itn);

        // This is only okay because of our ordering earlier
        std::pair< std::string, Node* > pair;
        pair.first = child->stdString;
        pair.second = child;

        // Fill the proper vector with the pair for this child
        if (child->isLetter)
            letterVec.push_back( pair );
        else
            cutVec.push_back( pair );
    }

    // Perform the sort here
    std::sort( letterVec.begin(), letterVec.end(), comparePairs );
    std::sort( cutVec.begin(), cutVec.end(), comparePairs );
    //std::reverse( cutVec.begin(), cutVec.end() );

    //std::cout << "We sorted the letters and cuts. They look like:\n";



    // Update the children list to reflect the new order
    children.clear();

    std::vector< std::pair< std::string, Node*> >::iterator itp;
    for ( itp = letterVec.begin(); itp != letterVec.end(); ++itp )
    {
        //std::cout << (*itp).first << std::endl;
        children.push_back( (*itp).second );
    }
    for ( itp = cutVec.begin(); itp != cutVec.end(); ++itp )
    {
        //std::cout << (*itp).first << std::endl;
        children.push_back( (*itp).second );
    }
}

// (Static)
// Standardize an entire graph, starting with root
void Node::standardize(Node* root)
{
    // Parents depend on their children, so we must use DFS to produce a valid
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
        std::list<Node*>::iterator it = curr->children.begin();

        for ( ; it != curr->children.end(); ++it )
            stack.push( *it );
    }

    // Now that we have an ordering where all children come before their
    // parents, we can call the sort routine.
    std::list<Node*>::iterator it = ordering.begin();
    for( ; it != ordering.end(); ++it )
    {
        Node* curr = (*it);

        if ( curr->isCut || curr->isRoot ) // Cuts & root
        {
            if ( curr->children.size() > 0 ) // At least one child
            {
                curr->sortChildren();
                curr->updateStdString();
            }

        }

        // DEBUG: Verify that we have made a proper string
        //std::cout << "Made string: " << curr->stdString << std::endl;

    }

    // DEBUG: notify finish
    //std::cout << "Finished standardizing the entire graph\n";
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
            curr->setAsCut();

            // Push kids
            for (int i = 0; i < s[pos] - '0'; ++i)
                stack.push(curr->addChild());
        }
        else // Letters indicate statements
            curr->setAsLetter(s[pos]);
    }

    // Bad string
    if ( !stack.empty() )
    {
        std::cerr << "ERROR: not enough nodes\n";
        return NULL;
    }


    // Make sure the root is actually a root node
    root->setAsRoot();

    return root;
}
