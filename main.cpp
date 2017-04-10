#include <iostream>
#include <string>
#include "node.h"

int main()
{
    std::list <std::string> list;
    list.push_back( "221PQ21QP" );
    list.push_back( "221QP21PQ" );
    list.push_back( "3A1C3EBD" );
    list.push_back( "23CABA" );
    list.push_back( "31A1C1B" );
    list.push_back( "3ABC" );

    std::list<std::string>::iterator it = list.begin();
    for ( ; it != list.end(); ++it )
    {
        std::string curr = (*it);
        std::cout << "Current string: " << curr << std::endl;
        Node* node = Node::parseFromString( curr );

        if ( node == NULL )
        {
            std::cerr << "ERROR: invalid" << std::endl;
            continue;
        }

        node->print();

        std::cout << "Test to make sure toString retains input order:\n";
        std::cout << "OLD: " << curr << std::endl;
        std::cout << "NEW: " << node->toString() << std::endl << std::endl;

        std::cout << "Standardized:" << std::endl;
        Node::standardize( node );
        node->print();
        std::cout << "Final output: " << node->toString() << std::endl;
        std::cout << "----------" << std::endl;
    }

}
