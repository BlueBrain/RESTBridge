/**
 * @file    main.cpp
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2015-01-13
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#include <restconnector/types.h>
#include <restconnector/RestConnector.h>
#include <restconnector/RestZeqTranslator.h>

#include <zeq/event.h>
#include <zeq/subscriber.h>
#include <zeq/publisher.h>
#include <zeq/hbp/vocabulary.h>

#include <lunchbox/uri.h>

#include <restconnector/Lib/zeq/events.h>

using namespace restconnector;

void handleEvent( bool* ok)
{
    std::cout<<"Got Camera Event"<<std::endl;
    sleep(5);
    *ok = true;
    std::cout<<"Done with Camera Event"<<std::endl;
}

//class MyThread most probably will be removed...
class MyThread
{
public:

   static void run()
    {
        boost::shared_ptr< ::zeq::Subscriber > subscriber_;
        boost::shared_ptr< ::zeq::Publisher > publisher_;

        publisher_.reset( new ::zeq::Publisher( lunchbox::URI( "restResponse://" ) ) );
        subscriber_.reset( new ::zeq::Subscriber( lunchbox::URI( "restCommand://" ) ) );

        bool ok(false);
        subscriber_->registerHandler( ::zeq::hbp::EVENT_CAMERA, boost::bind( &handleEvent, &ok ));

        while(true)
        {
            sleep( 1 );
            while( subscriber_->receive( 0 ) ) {}
            if( ok )
            {
                restconnector::zeq::GenericResponseInfo info;
                info.id = 1;
                info.status = 2;
                info.type = 3;
                for( uint8_t i=0; i<50; ++i )
                    info.data.push_back(i);


                std::cout << "Sending Zeq response" << std::endl;

                const ::zeq::Event& event = restconnector::zeq::serializeGenericResponse( info );
                publisher_->publish( event );
                ok = false;
            }
            std::cout << "Waiting on Zeq messages" << std::endl;
        }
    }

};

using lunchbox::make_uint128;
class testRestZeqTranslator: public RestZeqTranslator
{
public:
    virtual ::zeq::Event translate( const std::string&, const std::vector< std::string >&,
                           const std::vector< std::string >&)
    {
        std::vector<float> matrix;
        matrix.push_back( 1 ); matrix.push_back( 0 ); matrix.push_back( 0 ); matrix.push_back( 0 );
        matrix.push_back( 0 ); matrix.push_back( 1 ); matrix.push_back( 0 ); matrix.push_back( 0 );
        matrix.push_back( 0 ); matrix.push_back( 0 ); matrix.push_back( 1 ); matrix.push_back( 0 );
        matrix.push_back( 0 ); matrix.push_back( 0 ); matrix.push_back( 0 ); matrix.push_back( 1 );

        ::zeq::Event event = ::zeq::hbp::serializeCamera( matrix );
        return event;
    }
};

int main( int argc, char * argv[] )
{
    if ( argc != 3 ) {
        std::cerr << "Usage: " << argv[0] << " address port" << std::endl;
        return 1;
    }
    try
    {
        RestZeqTranslatorPtr kakaToDo( new testRestZeqTranslator() );
        RestConnector mainServer( argv[1] , argv[2] );

        boost::thread t(&MyThread::run );

        mainServer.run( kakaToDo );

        t.join();
    }
    catch ( std::exception &e )
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
