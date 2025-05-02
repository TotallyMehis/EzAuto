#include <gtest/gtest.h>

#include "offsetparser.h"

TEST( OffsetParserTest, BasicOffset )
{
    EXPECT_EQ( 0, COffsetParser().AddOffset( "name", "0x0" ) );
    EXPECT_EQ( 1, COffsetParser().AddOffset( "name", "0x1" ) );
    EXPECT_EQ( 2, COffsetParser().AddOffset( "name", "0x02" ) );
    EXPECT_EQ( 15, COffsetParser().AddOffset( "name", "0xf" ) );
    EXPECT_EQ( 15, COffsetParser().AddOffset( "name", "0xF" ) );
}

TEST( OffsetParserTest, Find )
{
    auto parser = COffsetParser();
    parser.AddOffset( "name1", "0x1" );
    parser.AddOffset( "name2", "0x2" );


    offset_t off1 = 0;
    offset_t off2 = 0;
    EXPECT_NE( -1, parser.FindOffset( "name1", &off1 ) );
    EXPECT_NE( -1, parser.FindOffset( "name2", &off2 ) );
    EXPECT_EQ( 1, off1 );
    EXPECT_EQ( 2, off2 );
}

TEST( OffsetParserTest, Add )
{
    auto parser = COffsetParser();
    parser.AddOffset( "name1", "0x1" );
    parser.AddOffset( "name2", "0x2" );
    EXPECT_EQ( 3, parser.AddOffset( "result", "name1+name2" ) );
    EXPECT_EQ( 4, parser.AddOffset( "result2", "result+name1" ) );
}
