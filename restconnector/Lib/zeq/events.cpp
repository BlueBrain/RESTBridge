/* Copyright (c) 2014, EPFL/Blue Brain Project
 *                     Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 *                     Cyrille Favreau <cyrille.favreau@epfl.ch>
 *
 * This file is part of Livre <https://github.com/BlueBrain/Livre>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "events.h"

#include "restconnector/response_generated.h"
#include "restconnector/makeCurrent_generated.h"

#include "zeq/event.h"
#include "zeq/vocabulary.h"

#include <lunchbox/debug.h>

namespace restconnector
{
namespace zeq
{

template< class T >
void deserializeVector( std::vector< T > &out, const flatbuffers::Vector< T > *in )
{
    if( in )
    {
        for (uint32_t i = 0; i < in->size(); ++i)
        {
            out.push_back(in->Get(i));
        }
    }
}

::zeq::Event serializeMakeCurrent( const uint32_t session )
{
    ::zeq::Event event( EVENT_MAKE_CURRENT );
    flatbuffers::FlatBufferBuilder& fbb = event.getFBB();

    MakeCurrentBuilder builder( fbb );

    builder.add_session( session );

    return event;
}

uint32_t deserializeMakeCurrent( const ::zeq::Event& event )
{
    auto data = GetMakeCurrent( event.getData( ));
    return data->session();
}

::zeq::Event serializeGenericResponse( const GenericResponseInfo& genericResponseInfo )
{
    ::zeq::Event event( EVENT_GENERIC_RESPONSE );
    flatbuffers::FlatBufferBuilder& fbb = event.getFBB();

    GenericResponseBuilder builder( fbb );

    auto data = fbb.CreateVector( &genericResponseInfo.data[0], genericResponseInfo.data.size() );

    builder.add_id( genericResponseInfo.id );
    builder.add_status( genericResponseInfo.status );
    builder.add_type( genericResponseInfo.type );
    builder.add_data( data );

    fbb.Finish( builder.Finish( ));
    return event;
}

GenericResponseInfo deserializeGenericResponse( const ::zeq::Event& event )
{
    if( event.getType() != EVENT_GENERIC_RESPONSE )
        return GenericResponseInfo();

    auto data = GetGenericResponse( event.getData( ));
    GenericResponseInfo info;
    info.id = data->id();
    info.status = data->status();
    info.type = data->type();
    deserializeVector( info.data, data->data( ) );
    return info;
}

#undef BUILD_VECTOR_ONLY_BUFFER

}
}
