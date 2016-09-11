#pragma once


#include "settings_int.h"


enum Engine_t
{
    ENGINE_INVALID = -1,

    ENGINE_SOURCE,
    ENGINE_GOLDSRC,

    ENGINE_MAX
};

namespace Engine
{
    Engine_t EngineNameToType( const char* name );
};