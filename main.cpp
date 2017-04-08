#include <stack>
#include <list>

#include <string>
#include <iostream>

#include "node.h"


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
Node* parseFromString( std::string s )
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


int main()
{
    // Let's examine two different inputs that should form the same tree
    std::string s = "5A01A2AB32AB4ABCD1C";
    std::string s2 = "52AB1A32BA4ABCD1CA0";

    // First string
    Node* root = parseFromString( s );
    if ( root == NULL )
    {
        std::cerr << "Not a valid tree\n";
        return -1;
    }

    Node::standardize( root );
    std::cout << "Input: " << s2 << std::endl;
    root->print();
    std::cout << "Output: " << root->toString() << std::endl;

    // Second string
    Node* root2 = parseFromString( s2 );
    if ( root2 == NULL )
    {
        std::cerr << "Not a valid tree\n";
        return -1;
    }
    
    std::cout << "\n----------\n";
    Node::standardize( root2 );
    std::cout << "\nInput 2: " << s2 << std::endl;
    root2->print();
    std::cout << "Output 2: " << root->toString() << std::endl;

    std::cout << "\n----------\n";
    std::cout << std::endl << "Verify that different inputs produce the same output:"
         << std::endl;

    std::cout << s  << ":" << root->toString()  << std::endl;
    std::cout << s2 << ":" << root2->toString() << std::endl;

    std::cout << (root->toString() == root2->toString()) << std::endl;



    // New tests
    std::cout << "\n----------\n";
    std::string s3 = "5A01A2AB31C2AB4ABCD";
    Node* root3 = parseFromString( s3 );
    std::cout << "Read in " << s3 << " and print: " << std::endl;
    root3->print();
    std::cout << "Result (notstandard): " << root3->toString() << std::endl;

    std::cout << std::endl << "Standardized:" << std::endl;
    Node::standardize( root3 );
    root3->print();
    std::cout << "Result (standard): " << root3->toString() << std::endl;

    // More
    std::cout << "\n----------\n";
    std::cout << "\nAll permutations test:" << std::endl;
    std::list< std::string > list;
    list.push_back( "22AB2CD" );
    list.push_back( "22AB2DC" );

    list.push_back( "22BA2CD" );
    list.push_back( "22BA2DC" );

    list.push_back( "22CD2AB" );
    list.push_back( "22CD2BA" );
    list.push_back( "22AB2CD" );
    list.push_back( "22BA2DC" );

    std::list< std::string >::iterator it = list.begin();

    for ( ; it != list.end(); ++it)
    {
        std::string curr = (*it);
        std::cout << "In:  " << curr << std::endl;
        Node* cr = parseFromString( curr );
        Node::standardize( cr );
        std::cout << "Out: " << cr->toString() << std::endl << std::endl;
    }

}
