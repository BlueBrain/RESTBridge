/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#include <restconnector/types.h>
#include <restconnector/RestConnector.h>
#include <restconnector/RestZeqTranslator.h>

#include <zeq/event.h>
#include <zeq/subscriber.h>
#include <zeq/publisher.h>
#include <zeq/hbp/vocabulary.h>

#include <lunchbox/uri.h>

using namespace restconnector;

int main( int argc, char * argv[] )
{
    if ( argc != 3 ) {
        std::cerr << "Usage: " << argv[0] << " address port" << std::endl;
        return 1;
    }
    try
    {
        RestConnector mainServer( argv[1] , argv[2] );
        mainServer.run();
        while(true){}
    }
    catch ( std::exception &e )
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
