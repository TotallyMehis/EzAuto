#pragma once


#include <vector>

#include "player_base.h"


class COffset;


class COffsetParser
{
public:
    ~COffsetParser();

    offset_t AddOffset( const char* name, const char* offset );
    offset_t AddOffset( const char* name, offset_t offset );

    int FindOffset( const char* name, offset_t* offset = nullptr );

private:
    std::vector<COffset*> m_Offsets;
};
