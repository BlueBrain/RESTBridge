/* Copyright (c) 2014-2015, Human Brain Project
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 *
 * This file is part of RESTBridge <https://github.com/BlueBrain/RESTBridge>
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

#include <restbridge/RestBridge.h>
#include <restbridge/log.h>
#include <thread>

int main( int argc, char* argv[] )
{
    try
    {
        std::unique_ptr< restbridge::RestBridge> server(
            restbridge::RestBridge::create( argc, argv ));

        if( !server )
        {
            RBERROR << "Error creating REST bridge" << std::endl;
            return EXIT_FAILURE;
        }

        std::this_thread::sleep_for(
            std::chrono::hours( 0x8FFu /*23bit precision guaranteed*/ ));
    }
    catch ( std::exception &e )
    {
        RBERROR << "Error running REST bridge: " << e.what() << std::endl
                << "Usage: " << argv[0] << " "
                << restbridge::RestBridge::getHelp() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
