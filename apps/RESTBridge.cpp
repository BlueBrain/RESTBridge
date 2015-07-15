/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#include <restbridge/RestBridge.h>
#include <restbridge/log.h>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#define PARAM_HOST   "host"
#define PARAM_PORT   "port"
#define PARAM_SCHEMA "schema"

int main( int argc, char * argv[] )
{
    // Declare the supported options.
    po::options_description description("Usage:");
    description.add_options()
        ( PARAM_HOST ",h", po::value<std::string>(), "Hostname or IP address")
        ( PARAM_PORT ",p", po::value<std::string>(), "Listening port")
        ( PARAM_SCHEMA ",s", po::value<std::string>(),
            "Schema prefix used by zeq publisher and subscriber");

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
    po::notify(vm);

    if( !vm.count( PARAM_HOST ) || !vm.count( PARAM_PORT ) || !vm.count( PARAM_SCHEMA ))
    {
        RBERROR << description << std::endl;
        return 1;
    }

    try
    {
        restbridge::RestBridge mainServer(
            vm[ PARAM_HOST ].as< std::string >() ,
            boost::lexical_cast<uint16_t>( vm[ PARAM_PORT ].as< std::string >()));
        mainServer.run( vm[ PARAM_SCHEMA ].as< std::string >());
        while(true){}
    }
    catch( const boost::bad_lexical_cast &e )
    {
        RBERROR << vm[ PARAM_PORT ].as< std::string >() << " is an invalid port number: " <<
            e.what() << std::endl;
        return 1;
    }
    catch ( std::exception &e )
    {
        RBERROR << e.what() << std::endl;
        return 1;
    }
    return 0;
}
