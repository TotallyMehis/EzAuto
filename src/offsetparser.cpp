#include "offsetparser.h"
#include "offsetparser_int.h"


COffsetParser::~COffsetParser()
{
    for ( size_t i = 0; i < m_Offsets.size(); i++ )
    {
        delete m_Offsets[i];
    }
}

offset_t COffsetParser::AddOffset( const char* name, const char* offset )
{
    if ( !offset || !*offset ) return NULL;


    COffset* off = nullptr;
    int index = FindOffset( name );


    if ( index == -1 )
    {
        off = new COffset( this, name, offset );

        m_Offsets.push_back( off );
    }
    else
    {
        off = m_Offsets[index];
    }


    return off->GetOffset();
}

offset_t COffsetParser::AddOffset( const char* name, offset_t offset )
{
    COffset* off = nullptr;
    int index = FindOffset( name );


    if ( index == -1 )
    {
        off = new COffset( this, name, offset );

        m_Offsets.push_back( off );
    }
    else
    {
        off = m_Offsets[index];
    }


    return off->GetOffset();
}

int COffsetParser::FindOffset( const char* name, offset_t* offset )
{
    for ( size_t i = 0; i < m_Offsets.size(); i++ )
    {
        if ( _stricmp( name, m_Offsets[i]->GetName() ) == 0 )
        {
            if ( offset )
            {
                *offset = m_Offsets[i]->GetOffset();
            }

            return i;
        }
    }

    if ( offset )
    {
        *offset = NULL;
    }

    return -1;
}