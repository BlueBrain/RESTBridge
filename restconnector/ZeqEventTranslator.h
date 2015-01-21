/**
 * @file    RequestHandler.h
 * @brief
 * @author  Grigori Chevtchenko
 * @date    2014-26-11
 * @remarks Copyright (c) BBP/EPFL 2005-2014; All rights reserved. Do not distribute without further notice.
 */

#ifndef _ZeqEventTranslator_h_
#define _ZeqEventTranslator_h_

#include <restconnector/types.h>

namespace restconnector
{

class ZeqEventTranslator
{
public:

    ZeqEventTranslator();

    ~ZeqEventTranslator();

    void translate();
};

}
#endif // _ZeqEventTranslator_h_
