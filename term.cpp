#include <iostream>
#include <string>
#include "node.h"

int main( int argc, char* argv[] )
{
    if ( argc < 2)
    {
        std::cout << "Usage: run \"./a.out 21AB\" where 21AB is the string you"
                  << " wish to standardize.\nFlag -d will perform a diff on"
                  << " the output and input string to see if it is already"
                  << " standardized.\n";
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
    std::string output = node->toString();

    // Extra flags
    if (argc > 2)
    {
        std::string flag( argv[2] );

        // Check if the input and output string are already standardized
        if (flag == "-d")
        {
            if (input == output)
            {
                std::cout << "Already standard:" << std::endl;
                std::cout << "In:  " << input << std::endl;
                std::cout << "Out: " << output << std::endl;
                return 0;
            }
            else
            {
                std::cout << "Not already standardized." << std::endl;
                std::cout << "In:  " << input << std::endl;
                std::cout << "Out: " << output << std::endl;
                return -1;
            }

        }
    }

    std::cout << node->toString() << std::endl;
}
