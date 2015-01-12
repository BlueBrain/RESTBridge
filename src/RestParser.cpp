/**
 * @file    WebEventInfo.h
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2014-29-07
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#include <src/RestParser.h>

#include <iostream>
#include <boost/algorithm/string.hpp>

RestParser::RestParser()
{}

bool RestParser::parse( const std::string& data )
{
    if( !findRestRequest_( data ) )
        return false;
    if( !parseRestRequest_() )
        return false;

    return true;
}

bool RestParser::findRestRequest_( const std::string& data )
{
    if( !data.empty() )
    {
        std::vector<std::string> dataParts;
        boost::split( dataParts, data, boost::is_any_of("/") );
        restRequest_ = dataParts.back();
        std::cout<<"REST: "<<restRequest_<<std::endl;
        return true;
    }
    else
    {
        std::cout<<"Error: The request string is empty."<<std::endl;
        return false;
    }
}

bool RestParser::parseRestRequest_()
{
    std::vector<std::string> commandAndParam;
    boost::split( commandAndParam, restRequest_, boost::is_any_of("?") );

    if( commandAndParam[0] == "" )
    {
        std::cout<<"Error: No command found"<<std::endl;
        return false;
    }
    else if( commandAndParam[1] == "" )
    {
        command_ = commandAndParam[0];
        return true;
    }

    command_ = commandAndParam[0];

    std::vector<std::string> params;
    boost::split( params, commandAndParam[1], boost::is_any_of("&") );

    if( params.empty() )
        return true;

    for( uint32_t i=0; i < params.size(); i++)
    {
        if( params[i] != "" )
        {
            std::vector<std::string> paramsAndValues;
            boost::split( paramsAndValues, params[i], boost::is_any_of("=") );

            if( ( paramsAndValues[0] == "" ) || ( paramsAndValues[1] == "") || (paramsAndValues.size() != 2 ) )
            {
                std::cout<<"Error: Parameter number "<< i + 1u <<" is unconstistant."<<std::endl;
                return false;
            }

            params_.push_back( paramsAndValues[0]);
            values_.push_back( paramsAndValues[1]);
        }
    }
    std::cout<<"Parsing ok. Number of parameters: "<<params_.size()<<std::endl;
    return true;
}
