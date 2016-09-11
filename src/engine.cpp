#include "engine.h"


Engine_t Engine::EngineNameToType( const char* name )
{
    if ( _stricmp( name, "source" ) == 0 )
    {
        return ENGINE_SOURCE;
    }
    else if ( _stricmp( name, "goldsrc" ) == 0 )
    {
        return ENGINE_GOLDSRC;
    }

    return ENGINE_INVALID;
}