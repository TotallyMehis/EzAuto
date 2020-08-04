#pragma once


#include <vector>


#include "engine.h"

#include "settings_int.h"


class CSettings
{
public:
    ~CSettings();



    static CSettings* OpenFile( const char* filepath );

    static void ParseOption( CSettings* data, const char* src );



    void CreateSection( const char* name );

    const char* FindOption( const char* name );
    bool FindOption( const char* name, char* value, size_t len );

    
    void RemoveSection( size_t index );


    std::vector<CSettings_Section*> data;
};


extern CSettings* g_pGameData;
