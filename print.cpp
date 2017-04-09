#include <iostream>
#include <string>
#include "node.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./print 21A1B where \"21A1B\" is the graph string"
                  << " you wish to be printed. This app will print both the"
                  << " original tree as it is parsed, the standardized tree,"
                  << " and the final output string." << std::endl;
        return -1;
    }

    std::string input( argv[1] );
    Node* node = Node::parseFromString( input );

    if (node == NULL)
    {
        std::cerr << "ERROR: not a valid input string" << std::endl;
        return -1;
    }

    std::cout << "Original input: " << input << std::endl;
    node->print();

    Node::standardize( node );
    std::string output( node->toString() );

    if (input == output)
    {
        std::cout << "\nString was already standardized" << std::endl;
        return 0;
    }

    std::cout << "\nFinal output: " << output << std::endl;
    node->print();

    return 0;
}
