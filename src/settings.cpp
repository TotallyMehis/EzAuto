#include <cstdio>
#include <cstring>


#include "sys.h"
#include "settings.h"


CSettings* g_pGameData = nullptr;


CSettings::~CSettings()
{
    for ( size_t i = 0; i < data.size(); i++ )
    {
        delete data[i];
    }
}

void CSettings::CreateSection( const char* name )
{
    // Check if our previous section has a name.
    // If not, make new section part of it and just update the name.
    if ( name && *name )
    {
        int prev = data.size() - 1;

        if ( prev >= 0 && !data[prev]->HasValidName() )
        {
            data[prev]->SetSectionName( name );

            return;
        }
    }

    CSettings_Section* section = new CSettings_Section( name );

    data.push_back( section );
}

void CSettings::ParseOption( CSettings* data, const char* src )
{
    if ( !data ) return;

    if ( !src || !*src ) return;


    char* c;

    // Parse new section.
    if ( *src == '[' )
    {
        c = (char*)strchr( src, ']' );

        if ( c )
        {
            *c = 0;

            data->CreateSection( src + 1 );
        }
        

        return;
    }
    
    // See if it's an option instead.
    c = (char*)strchr( src, '=' );
    if ( !c ) return;


    *c = 0;

    int target = data->data.size() - 1;

    // We must have a section to put this into!
    // Create an empty section.
    if ( target < 0 )
    {
        data->CreateSection( nullptr );

        target = data->data.size() - 1;
    }

    
    if ( target >= 0 )
    {
        data->data[target]->InsertOption( src, c + 1 );
    }
}

CSettings* CSettings::OpenFile( const char* filepath )
{
    if ( !filepath || !*filepath ) return nullptr;


    FILE* file;
    fopen_s( &file, filepath, "r" );


    if ( !file ) return nullptr;


    CSystem::PrintDev( "Opening settings file '%s'!\n", filepath );


    CSettings* data = new CSettings();


#define BUFFER_SIZE     512

    char* buffer = new char[BUFFER_SIZE];

    size_t len;


    while ( (len = fread( (void*)buffer, 1, BUFFER_SIZE - 1, file )) )
    {
        buffer[len] = 0;

        char* c = buffer;

        while ( c )
        {
            char* n = strchr( c, '\n' );

            // We don't want to read any further than this.
            if ( n )
            {
                *n = 0;

                // Remove CR for windows, kinda hacky but whatever.
                if ( *(n - 1) == '\r' )
                {
                    *(n - 1) = 0;
                }
            }

            // Delete any comments.
            char* comment = strchr( c, ';' );

            if ( comment )
            {
                *comment = 0;
            }
                

            CSettings::ParseOption( data, c );


            if ( n )
            {
                // Jump to the next line.
                c = n + 1;
            }
            else
            {
                // We've just hit the end of our buffer.
                // Go back to the start of a new line, otherwise this line wouldn't be read correctly.


                // End of file, just break.
                if ( feof( file ) ) break;


                size_t linelen = strlen( c );

                // There were no new lines in this buffer. Just break.
                if ( linelen >= BUFFER_SIZE ) break;


                // Go back to the start of our line to read it fully.
                fpos_t pos;
                if ( fgetpos( file, &pos ) == 0 )
                {
                    pos -= linelen;

                    fsetpos( file, &pos );
                }

                break;
            }
        }


    }

    fclose( file );

    delete[] buffer;


    return data;
}

const char* CSettings::FindOption( const char* name )
{
    for ( size_t i = 0; i < data.size(); i++ )
    {
        const char* value = data[i]->GetOptionValue( name );

        if ( value ) return value;
    }

    return nullptr;
}

bool CSettings::FindOption( const char* name, char* value, size_t len )
{
    for ( size_t i = 0; i < data.size(); i++ )
    {
        const char* myvalue = data[i]->GetOptionValue( name );

        if ( myvalue )
        {
            if ( value && len )
            {
                strcpy_s( value, len, myvalue );
            }

            return true;
        }
    }

    return false;
}

void CSettings::RemoveSection( size_t index )
{
    data.erase( data.begin() + index );
}
