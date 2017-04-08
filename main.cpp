#include <stack>
#include <list>

#include <string>
#include <iostream>

#include "node.h"




int main()
{
    // Let's examine two different inputs that should form the same tree
    std::string s = "5A01A2AB32AB4ABCD1C";
    std::string s2 = "52AB1A32BA4ABCD1CA0";

    // First string
    Node* root = Node::parseFromString( s );
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
    Node* root2 = Node::parseFromString( s2 );
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
    Node* root3 = Node::parseFromString( s3 );
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
        Node* cr = Node::parseFromString( curr );
        Node::standardize( cr );
        std::cout << "Out: " << cr->toString() << std::endl << std::endl;
    }

}
