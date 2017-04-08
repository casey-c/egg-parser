#include <iostream>
#include <string>
#include "node.h"

int main( int argc, char* argv[] )
{
    if ( argc != 2)
    {
        std::cout << "Usage: run \"./a.out 21AB\" where 21AB is the string you"
                  << " wish to standardize.\n";
        return -1;
    }

    std::string input( argv[1] );
    Node* node = Node::parseFromString( input );
    if (node == NULL)
    {
        std::cerr << "ERROR: not a valid string";
        return -1;
    }

    Node::standardize( node );
    std::cout << node->toString() << std::endl;
}
