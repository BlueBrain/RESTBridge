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

#ifndef _RestConnector_Events_h_
#define _RestConnector_Events_h_

#include <lunchbox/uint128_t.h>
#include <zeq/types.h>

namespace restconnector
{
namespace zeq
{

struct GenericResponseInfo
{
  uint64_t id;
  uint8_t status;
  uint32_t type;
  std::vector< uint8_t > data;
};

static const lunchbox::uint128_t EVENT_GENERIC_RESPONSE(
    lunchbox::make_uint128( "hbp::GenericResponseEvent" ));

static const lunchbox::uint128_t EVENT_MAKE_CURRENT(
    lunchbox::make_uint128( "hbp::MakeCurrentEvent" ));

static const lunchbox::uint128_t EVENT_GET_IMAGE(
    lunchbox::make_uint128( "hbp::GetImageEvent" ));

::zeq::Event serializeMakeCurrent( const uint32_t session );

uint32_t deserializeMakeCurrent( const ::zeq::Event& event );

/**
 * TO DO
 */
::zeq::Event serializeGenericResponse( const GenericResponseInfo& genericResponseInfo );

/**
 * TO DO
 */
GenericResponseInfo deserializeGenericResponse( const ::zeq::Event& event );

}
}

#endif // _RestConnector_Events_h_

