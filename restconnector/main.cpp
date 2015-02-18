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
#include <lunchbox/log.h>

#include <boost/program_options.hpp>

using namespace restconnector;
namespace po = boost::program_options;

#define PARAM_HOST   "host"
#define PARAM_PORT   "port"
#define PARAM_SCHEMA "schema"

int main( int argc, char * argv[] )
{
    lunchbox::Log::instance().level = lunchbox::LOG_ALL;

    // Declare the supported options.
    po::options_description description("Usage:");
    description.add_options()
        ( PARAM_HOST ",h", po::value<std::string>(), "Hostname or IP address")
        ( PARAM_PORT ",p", po::value<std::string>(), "Listening port")
        ( PARAM_SCHEMA ",s", po::value<std::string>(),
            "Schema prefix used by zeq publisher and subscriber")
    ;

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
    po::notify(vm);

    if( !vm.count( PARAM_HOST ) || !vm.count( PARAM_PORT ) || !vm.count( PARAM_SCHEMA ))
    {
        LBERROR << description << std::endl;
        return 1;
    }

    try
    {
        RestConnector mainServer(
            vm[ PARAM_HOST ].as< std::string >() ,
            boost::lexical_cast<uint16_t>( vm[ PARAM_PORT ].as< std::string >()));
        mainServer.run( vm[ PARAM_SCHEMA ].as< std::string >());
        while(true){}
    }
    catch( const boost::bad_lexical_cast &e )
    {
        LBERROR << vm[ PARAM_PORT ].as< std::string >() << " is an invalid port number: " <<
            e.what() << std::endl;
        return 1;
    }
    catch ( std::exception &e )
    {
        LBERROR << e.what() << std::endl;
        return 1;
    }
    return 0;
}
