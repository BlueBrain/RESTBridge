/**
 * @file    main.cpp
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2015-01-13
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#include <src/types.h>
#include <src/RestConnector.h>

#include <zeq/event.h>
#include <zeq/subscriber.h>
#include <zeq/publisher.h>
#include <zeq/hbp/vocabulary.h>

using lunchbox::make_uint128;
class testRestZeqTranslator: public RestZeqTranslator
{
public:
    virtual zeq::Event translate( const std::string&, const std::vector< std::string >&,
                           const std::vector< std::string >&)
    {
        std::vector<float> matrix;
        matrix.push_back( 1 ); matrix.push_back( 0 ); matrix.push_back( 0 ); matrix.push_back( 0 );
        matrix.push_back( 0 ); matrix.push_back( 1 ); matrix.push_back( 0 ); matrix.push_back( 0 );
        matrix.push_back( 0 ); matrix.push_back( 0 ); matrix.push_back( 1 ); matrix.push_back( 0 );
        matrix.push_back( 0 ); matrix.push_back( 0 ); matrix.push_back( 0 ); matrix.push_back( 1 );

        zeq::Event event = zeq::hbp::serializeCamera( matrix );
        return event;
    }
};

int main( int argc, char * argv[] )
{
    if ( argc != 4 ) {
        std::cerr << "Usage: " << argv[0] << " address port timeout" << std::endl;
        return 1;
    }
    try
    {
        float timeOut = boost::lexical_cast<float>( argv[3] );
        RestZeqTranslatorPtr kakaToDo( new testRestZeqTranslator() );
        RestConnector mainServer( timeOut, argv[1] , argv[2] );
        mainServer.run( kakaToDo );
    }
    catch ( std::exception &e )
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
