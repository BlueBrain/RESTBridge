/**
 * @file    main.cpp
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2015-01-13
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#include <src/types.h>
#include <src/RestConnector.h>

int main( int argc, char * argv[] )
{
    if ( argc != 4 ) {
        std::cerr << "Usage: " << argv[0] << " address port timeout" << std::endl;
        return 1;
    }
    try
    {
        float timeOut = boost::lexical_cast<float>( argv[3] );
        RestConnector mainServer( timeOut, argv[1] , argv[2] );
        mainServer.run();
    }
    catch ( std::exception &e )
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
