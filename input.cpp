#include <iostream>
#include <string>

#include "node.h"

int main()
{
    while (1)
    {
        std::cout << "Input a graph string to standardize:\n";

        std::string input;
        std::cin >> input;

        Node* node = Node::parseFromString( input );
        if (node == NULL)
        {
            std::cout << input << " was invalid."  << std::endl;
        }
        else
        {
            Node::standardize( node );
            std::cout << node->toString() << std::endl;
        }

    }
}
