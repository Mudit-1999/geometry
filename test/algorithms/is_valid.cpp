// Boost.Geometry (aka GGL, Generic Geometry Library)
// Unit Test

// Copyright (c) 2014-2015, Oracle and/or its affiliates.

// Contributed and/or modified by Menelaos Karavelas, on behalf of Oracle

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html

#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE test_is_valid
#endif

#include <iostream>

#include <boost/test/included/unit_test.hpp>

#include <from_wkt.hpp>
#include "test_is_valid.hpp"
#include <boost/geometry/algorithms/reverse.hpp>


BOOST_AUTO_TEST_CASE( test_is_valid_point )
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << " is_valid: POINT " << std::endl;
    std::cout << "************************************" << std::endl;
#endif

    typedef point_type G;
    typedef default_validity_tester tester;
    typedef test_valid<tester, G> test;

    test::apply(from_wkt<G>("POINT(0 0)"), true, "p01");
}

BOOST_AUTO_TEST_CASE( test_is_valid_multipoint )
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << " is_valid: MULTIPOINT " << std::endl;
    std::cout << "************************************" << std::endl;
#endif

    typedef multi_point_type G;
    typedef default_validity_tester tester;
    typedef test_valid<tester, G> test;

    test::apply(from_wkt<G>("MULTIPOINT()"), true, "mp01");
    test::apply(from_wkt<G>("MULTIPOINT(0 0,0 0)"), true, "mp02");
    test::apply(from_wkt<G>("MULTIPOINT(0 0,1 0,1 1,0 1)"), true, "mp03");
    test::apply(from_wkt<G>("MULTIPOINT(0 0,1 0,1 1,1 0,0 1)"), true, "mp04");
}

BOOST_AUTO_TEST_CASE( test_is_valid_segment )
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << " is_valid: SEGMENT " << std::endl;
    std::cout << "************************************" << std::endl;
#endif

    typedef segment_type G;
    typedef default_validity_tester tester;
    typedef test_valid<tester, G> test;

    test::apply(from_wkt<G>("SEGMENT(0 0,0 0)"), false, "s01");
    test::apply(from_wkt<G>("SEGMENT(0 0,1 0)"), true, "s02");
}

BOOST_AUTO_TEST_CASE( test_is_valid_box )
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << " is_valid: BOX " << std::endl;
    std::cout << "************************************" << std::endl;
#endif

    typedef box_type G;
    typedef default_validity_tester tester;
    typedef test_valid<tester, G> test;

    // boxes where the max corner and below and/or to the left of min corner
    test::apply(from_wkt<G>("BOX(0 0,-1 0)"), false, "b01");
    test::apply(from_wkt<G>("BOX(0 0,0 -1)"), false, "b02");
    test::apply(from_wkt<G>("BOX(0 0,-1 -1)"), false, "b03");

    // boxes of zero area; they are not 2-dimensional, so invalid
    test::apply(from_wkt<G>("BOX(0 0,0 0)"), false, "b04");
    test::apply(from_wkt<G>("BOX(0 0,1 0)"), false, "b05");
    test::apply(from_wkt<G>("BOX(0 0,0 1)"), false, "b06");

    test::apply(from_wkt<G>("BOX(0 0,1 1)"), true, "b07");
}

template <typename G, bool AllowSpikes>
void test_linestrings()
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << "SPIKES ALLOWED? "
              << std::boolalpha
              << AllowSpikes
              << std::noboolalpha
              << std::endl;
#endif

    typedef validity_tester_linear<AllowSpikes> tester;
    typedef test_valid<tester, G> test;

    // empty linestring
    test::apply(from_wkt<G>("LINESTRING()"), false, "l01");

    // 1-point linestrings
    test::apply(from_wkt<G>("LINESTRING(0 0)"), false, "l02");
    test::apply(from_wkt<G>("LINESTRING(0 0,0 0)"), false, "l03");
    test::apply(from_wkt<G>("LINESTRING(0 0,0 0,0 0)"), false, "l04");

    // 2-point linestrings
    test::apply(from_wkt<G>("LINESTRING(0 0,1 2)"), true, "l05");
    test::apply(from_wkt<G>("LINESTRING(0 0,1 2,1 2)"), true, "l06");
    test::apply(from_wkt<G>("LINESTRING(0 0,0 0,1 2,1 2)"), true, "l07");
    test::apply(from_wkt<G>("LINESTRING(0 0,0 0,0 0,1 2,1 2)"), true, "l08");

    // 3-point linestrings
    test::apply(from_wkt<G>("LINESTRING(0 0,1 0,2 10)"), true, "l09");
    test::apply(from_wkt<G>("LINESTRING(0 0,1 0,2 10,0 0)"), true, "l10");
    test::apply(from_wkt<G>("LINESTRING(0 0,10 0,10 10,5 0)"), true, "l11");

    // linestrings with spikes
    test::apply(from_wkt<G>("LINESTRING(0 0,1 2,0 0)"), AllowSpikes, "l12");
    test::apply(from_wkt<G>("LINESTRING(0 0,1 2,1 2,0 0)"), AllowSpikes, "l13");
    test::apply(from_wkt<G>("LINESTRING(0 0,0 0,1 2,1 2,0 0)"),
                AllowSpikes,
                "l14");
    test::apply(from_wkt<G>("LINESTRING(0 0,0 0,0 0,1 2,1 2,0 0,0 0)"),
                AllowSpikes,
                "l15");
    test::apply(from_wkt<G>("LINESTRING(0 0,10 0,5 0)"), AllowSpikes, "l16");
    test::apply(from_wkt<G>("LINESTRING(0 0,10 0,10 10,5 0,0 0)"),
                AllowSpikes,
                "l17");
    test::apply(from_wkt<G>("LINESTRING(0 0,10 0,10 10,5 0,4 0,6 0)"),
                AllowSpikes,
                "l18");
    test::apply(from_wkt<G>("LINESTRING(0 0,1 0,1 1,5 5,4 4)"),
                AllowSpikes,
                "l19");
    test::apply(from_wkt<G>("LINESTRING(0 0,1 0,1 1,5 5,4 4,6 6)"),
                AllowSpikes,
                "l20");
    test::apply(from_wkt<G>("LINESTRING(0 0,1 0,1 1,5 5,4 4,4 0)"),
                AllowSpikes,
                "l21");
    test::apply(from_wkt<G>("LINESTRING(0 0,0 0,1 0,1 0,1 0,0 0,0 0,2 0)"),
                AllowSpikes,
                "l22");
    test::apply(from_wkt<G>("LINESTRING(0 0,1 0,0 0,2 0,0 0,3 0,0 0,4 0)"),
                AllowSpikes,
                "l23");
    test::apply(from_wkt<G>("LINESTRING(0 0,1 0,0 0,2 0,0 0,3 0,0 0,4 0,0 0)"),
                AllowSpikes,
                "l24");

    // other examples
    test::apply(from_wkt<G>("LINESTRING(0 0,10 0,10 10,5 0,4 0)"), true, "l25");
    test::apply(from_wkt<G>("LINESTRING(0 0,10 0,10 10,5 0,4 0,3 0)"),
                true,
                "l26");
    test::apply(from_wkt<G>("LINESTRING(0 0,10 0,10 10,5 0,4 0,-1 0)"),
                true,
                "l27");
    test::apply(from_wkt<G>("LINESTRING(0 0,1 0,1 1,-1 1,-1 0,0 0)"),
                true,
                "l28");
    test::apply(from_wkt<G>("LINESTRING(0 0,1 0,1 1,-1 1,-1 0,0.5 0)"),
                true,
                "l29");
}

BOOST_AUTO_TEST_CASE( test_is_valid_linestring )
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << " is_valid: LINESTRING " << std::endl;
    std::cout << "************************************" << std::endl;
#endif

    bool const allow_spikes = true;
    bool const do_not_allow_spikes = !allow_spikes;

    test_linestrings<linestring_type, allow_spikes>();
    test_linestrings<linestring_type, do_not_allow_spikes>();
}

template <typename G, bool AllowSpikes>
void test_multilinestrings()
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << "SPIKES ALLOWED? "
              << std::boolalpha
              << AllowSpikes
              << std::noboolalpha
              << std::endl;
#endif

    typedef validity_tester_linear<AllowSpikes> tester;
    typedef test_valid<tester, G> test;

    // empty multilinestring
    test::apply(from_wkt<G>("MULTILINESTRING()"), true, "mls01");

    // multilinestring with empty linestring(s)
    test::apply(from_wkt<G>("MULTILINESTRING(())"), false, "mls02");
    test::apply(from_wkt<G>("MULTILINESTRING((),(),())"), false, "mls03");
    test::apply(from_wkt<G>("MULTILINESTRING((),(0 1,1 0))"), false, "mls04");

    // multilinestring with invalid linestrings
    test::apply(from_wkt<G>("MULTILINESTRING((0 0),(0 1,1 0))"),
                false,
                "mls05");
    test::apply(from_wkt<G>("MULTILINESTRING((0 0,0 0),(0 1,1 0))"),
                false,
                "mls06");
    test::apply(from_wkt<G>("MULTILINESTRING((0 0),(1 0))"), false, "mls07");
    test::apply(from_wkt<G>("MULTILINESTRING((0 0,0 0),(1 0,1 0))"),
                false,
                "mls08");
    test::apply(from_wkt<G>("MULTILINESTRING((0 0),(0 0))"), false, "mls09");
    test::apply(from_wkt<G>("MULTILINESTRING((0 0,1 0,0 0),(5 0))"),
                false,
                "mls09");

    // multilinstring that has linestrings with spikes
    test::apply(from_wkt<G>("MULTILINESTRING((0 0,1 0,0 0),(5 0,1 0,4 1))"),
                AllowSpikes,
                "mls10");
    test::apply(from_wkt<G>("MULTILINESTRING((0 0,1 0,0 0),(1 0,2 0))"),
                AllowSpikes,
                "mls11");

    // valid multilinestrings
    test::apply(from_wkt<G>("MULTILINESTRING((0 0,1 0,2 0),(5 0,1 0,4 1))"),
                true,
                "mls12");
    test::apply(from_wkt<G>("MULTILINESTRING((0 0,1 0,2 0),(1 0,2 0))"),
                true,
                "mls13");
    test::apply(from_wkt<G>("MULTILINESTRING((0 0,1 1),(0 1,1 0))"),
                true,
                "mls14");
    test::apply(from_wkt<G>("MULTILINESTRING((0 0,1 1,2 2),(0 1,1 0,2 2))"),   
                true,
                "mls15");
}

BOOST_AUTO_TEST_CASE( test_is_valid_multilinestring )
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << " is_valid: MULTILINESTRING " << std::endl;
    std::cout << "************************************" << std::endl;
#endif

    bool const allow_spikes = true;
    bool const do_not_allow_spikes = !allow_spikes;

    test_multilinestrings<multi_linestring_type, allow_spikes>();
    test_multilinestrings<multi_linestring_type, do_not_allow_spikes>();
}


template <typename Point, bool AllowDuplicates>
void test_open_rings()
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << " is_valid: RING (open) " << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << "DUPLICATES ALLOWED? "
              << std::boolalpha
              << AllowDuplicates
              << std::noboolalpha
              << std::endl;
#endif

    typedef bg::model::ring<Point, false, false> OG; // ccw, open ring
    typedef bg::model::ring<Point, false, true> CG; // ccw, closed ring
    typedef bg::model::ring<Point, true, false> CW_OG; // cw, open ring
    typedef bg::model::ring<Point, true, true> CW_CG; // cw, closed ring
 
    typedef validity_tester_areal<AllowDuplicates> tester;
    typedef test_valid<tester, OG, CG, CW_OG, CW_CG> test;

    // not enough points
    test::apply(from_wkt<OG>("POLYGON(())"), false, "r01");
    test::apply(from_wkt<OG>("POLYGON((0 0))"), false, "r02");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0))"), false, "r03");

    // duplicate points
    test::apply(from_wkt<OG>("POLYGON((0 0,0 0,0 0))"), false, "r04");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,1 0))"), false, "r05");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,0 0))"), false, "r06");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,1 1,0 0,0 0))"),
                AllowDuplicates,
                "r07");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,1 0,1 1))"),
                AllowDuplicates,
                "r08");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,1 0,1 1,0 0))"),
                AllowDuplicates,
                "r09");

    // with spikes
    test::apply(from_wkt<OG>("POLYGON((0 0,2 0,2 2,0 2,1 2))"), false, "r10");
    test::apply(from_wkt<OG>("POLYGON((0 0,2 0,1 0,2 2))"), false, "r11");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,2 0,1 0,4 0,4 4))"),
                false,
                "r12");
    test::apply(from_wkt<OG>("POLYGON((0 0,2 0,2 2,1 0))"), false, "r13");
    test::apply(from_wkt<OG>("POLYGON((0 0,2 0,1 0))"), false, "r14");
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,4 4,5 5,0 5))"),
                false,
                "r15");
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,4 4,3 3,5 5,0 5))"),
                false,
                "r16");

    // with spikes and duplicate points
    test::apply(from_wkt<OG>("POLYGON((0 0,0 0,2 0,2 0,1 0,1 0))"),
                false,
                "r17");

    // with self-crossings
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,3 -1,0 5))"), false, "r18");

    // with self-crossings and duplicate points
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,5 5,3 -1,0 5,0 5))"),
                false,
                "r19");

    // with self-intersections
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,3 5,3 0,2 0,2 5,0 5))"),
                false,
                "r20");

    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,3 5,3 0,2 5,0 5))"),
                false,
                "r21");

    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 1,1 1,1 2,2 2,3 1,4 2,5 2,5 5,0 5))"),
                false,
                "r22");

    // with self-intersections and duplicate points
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,3 5,3 5,3 0,3 0,2 0,2 0,2 5,2 5,0 5))"),
                false,
                "r23");

    // next two suggested by Adam Wulkiewicz
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,0 5,4 4,2 2,0 5))"),
                false,
                "r24");
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,1 4,4 4,4 1,0 5))"),
                false,
                "r25");

    // and a few more
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,4 4,1 4,1 1,4 1,4 4,0 5))"),
                false,
                "r26");
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,4 4,4 1,1 1,1 4,4 4,0 5))"),
                false,
                "r27");

    // valid rings
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,1 1))"), true, "r28");
    test::apply(from_wkt<OG>("POLYGON((1 0,1 1,0 0))"), true, "r29");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,1 1,0 1))"), true, "r30");
    test::apply(from_wkt<OG>("POLYGON((1 0,1 1,0 1,0 0))"), true, "r31");

    // test cases coming from buffer
    test::apply
        (from_wkt<OG>("POLYGON((1.1713032141645456 -0.9370425713316364,\
                      5.1713032141645456 4.0629574286683638,\
                      4.7808688094430307 4.3753049524455756,\
                      4.7808688094430307 4.3753049524455756,\
                      0.7808688094430304 -0.6246950475544243,\
                      0.7808688094430304 -0.6246950475544243))"),
         AllowDuplicates,
         "r32");
}


template <typename Point, bool AllowDuplicates>
void test_closed_rings()
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << " is_valid: RING (closed) " << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << "DUPLICATES ALLOWED? "
              << std::boolalpha
              << AllowDuplicates
              << std::noboolalpha
              << std::endl;
#endif

    typedef bg::model::ring<Point, false, true> CG; // ccw, closed ring
    typedef bg::model::ring<Point, true, true> CW_CG; // cw, closed ring

    typedef validity_tester_areal<AllowDuplicates> tester;
    typedef test_valid<tester, CG, CG, CW_CG> test;

    // not enough points
    test::apply(from_wkt<CG>("POLYGON(())"), false, "r01c");
    test::apply(from_wkt<CG>("POLYGON((0 0))"), false, "r02c");
    test::apply(from_wkt<CG>("POLYGON((0 0,0 0))"), false, "r03c");
    test::apply(from_wkt<CG>("POLYGON((0 0,1 0))"), false, "r04c");
    test::apply(from_wkt<CG>("POLYGON((0 0,1 0,1 0))"), false, "r05c");
    test::apply(from_wkt<CG>("POLYGON((0 0,1 0,2 0))"), false, "r06c");
    test::apply(from_wkt<CG>("POLYGON((0 0,1 0,1 0,2 0))"), false, "r07c");
    test::apply(from_wkt<CG>("POLYGON((0 0,1 0,2 0,2 0))"), false, "r08c");

    // boundary not closed
    test::apply(from_wkt<CG>("POLYGON((0 0,1 0,1 1,1 2))"), false, "r09c");
    test::apply(from_wkt<CG>("POLYGON((0 0,1 0,1 0,1 1,1 1,1 2))"),
                false,
                "r10c");
}

BOOST_AUTO_TEST_CASE( test_is_valid_ring )
{
    bool const allow_duplicates = true;
    bool const do_not_allow_duplicates = !allow_duplicates;

    test_open_rings<point_type, allow_duplicates>();
    test_open_rings<point_type, do_not_allow_duplicates>();

    test_closed_rings<point_type, allow_duplicates>();
    test_closed_rings<point_type, do_not_allow_duplicates>();
}

template <typename Point, bool AllowDuplicates>
void test_open_polygons()
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << " is_valid: POLYGON (open) " << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << "DUPLICATES ALLOWED? "
              << std::boolalpha
              << AllowDuplicates
              << std::noboolalpha
              << std::endl;
#endif

    typedef bg::model::polygon<Point, false, false> OG; // ccw, open
    typedef bg::model::polygon<Point, false, true> CG; // ccw, closed
    typedef bg::model::polygon<Point, true, false> CW_OG; // cw, open
    typedef bg::model::polygon<Point, true, true> CW_CG; // cw, closed

    typedef validity_tester_areal<AllowDuplicates> tester;
    typedef test_valid<tester, OG, CG, CW_OG, CW_CG> test;

    // not enough points in exterior ring
    test::apply(from_wkt<OG>("POLYGON(())"), false, "pg001");
    test::apply(from_wkt<OG>("POLYGON((0 0))"), false, "pg002");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0))"), false, "pg003");

    // not enough points in interior ring
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),())"),
                false,
                "pg004");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1))"),
                false,
                "pg005");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,2 2))"),
                false,
                "pg006");

    // duplicate points in exterior ring
    test::apply(from_wkt<OG>("POLYGON((0 0,0 0,0 0))"), false, "pg007");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,1 0))"), false, "pg008");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,0 0))"), false, "pg009");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,1 1,0 0,0 0))"),
                AllowDuplicates,
                "pg010");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,1 0,1 1))"),
                AllowDuplicates,
                "pg011");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,1 0,1 1,0 0))"),
                AllowDuplicates,
                "pg011a");

    // duplicate points in interior ring
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,1 1,1 1))"),
                false,
                "pg012");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,2 1,2 1))"),
                false,
                "pg013");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,2 1,1 1))"),
                false,
                "pg014");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,2 2,2 1,1 1,1 1))"),
                AllowDuplicates,
                "pg015");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,2 2,2 2,2 1))"),
                AllowDuplicates,
                "pg016");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,2 2,2 1,2 1,1 1))"),
                AllowDuplicates,
                "pg017");

    // with spikes in exterior ring
    test::apply(from_wkt<OG>("POLYGON((0 0,2 0,2 2,0 2,1 2))"), false, "pg018");
    test::apply(from_wkt<OG>("POLYGON((0 0,2 0,1 0,2 2))"), false, "pg019");
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,2 0,1 0,4 0,4 4))"),
                false,
                "pg020");
    test::apply(from_wkt<OG>("POLYGON((0 0,2 0,2 2,1 0))"), false, "pg021");
    test::apply(from_wkt<OG>("POLYGON((0 0,2 0,1 0))"), false, "pg022");
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,4 4,5 5,0 5))"),
                false,
                "pg023");
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,4 4,3 3,5 5,0 5))"),
                false,
                "pg024");

    // with spikes in interior ring
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,3 1,3 3,1 3,2 3))"),
                false,
                "pg025");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,3 1,2 1,3 3))"),
                false,
                "pg026");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,2 1,3 1,2 1,4 1,4 4))"),
                false,
                "pg027");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,3 1,3 3,2 1))"),
                false,
                "pg028");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,3 1,2 1))"),
                false,
                "pg029");

    // with self-crossings in exterior ring
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,3 -1,0 5))"),
                false,
                "pg030");

    // example from Norvald Ryeng
    test::apply(from_wkt<OG>("POLYGON((100 1300,140 1300,140 170,100 1700))"),
                false,
                "pg031");
    // and with point order reversed
    test::apply(from_wkt<OG>("POLYGON((100 1300,100 1700,140 170,140 1300))"),
                false,
                "pg032");

    // with self-crossings in interior ring
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(3 3,3 7,4 6,2 6))"),
                false,
                "pg033");

    // with self-crossings between rings
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,0 5),(1 1,2 1,1 -1))"),
                false,
                "pg034");

    // with self-intersections in exterior ring
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,3 5,3 0,2 0,2 5,0 5))"),
                false,
                "pg035");

    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,3 5,3 0,2 5,0 5))"),
                false,
                "pg036");

    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 1,1 1,1 2,2 2,3 1,4 2,5 2,5 5,0 5))"),
                false,
                "pg037");

    // next two suggested by Adam Wulkiewicz
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,0 5,4 4,2 2,0 5))"),
                false,
                "pg038");
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,1 4,4 4,4 1,0 5))"),
                false,
                "pg039");
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,4 4,1 4,1 1,4 1,4 4,0 5))"),
                false,
                "pg040");
    test::apply(from_wkt<OG>("POLYGON((0 0,5 0,5 5,4 4,4 1,1 1,1 4,4 4,0 5))"),
                false,
                "pg041");

    // with self-intersections in interior ring
    test::apply(from_wkt<OG>("POLYGON((-10 -10,10 -10,10 10,-10 10),(0 0,5 0,5 5,3 5,3 0,2 0,2 5,0 5))"),
                false,
                "pg042");
    test::apply(from_wkt<OG>("POLYGON((-10 -10,10 -10,10 10,-10 10),(0 0,5 0,5 5,3 5,3 0,2 5,0 5))"),
                false,
                "pg043");

    test::apply(from_wkt<OG>("POLYGON((-10 -10,10 -10,10 10,-10 10),(0 0,5 0,5 1,1 1,1 2,2 2,3 1,4 2,5 2,5 5,0 5))"),
                false,
                "pg044");

    // with self-intersections between rings
    // hole has common segment with exterior ring
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,1 10,2 10,2 1))"),
                false,
                "pg045");
    test::apply(from_wkt<OG>("POLYGON((0 0,0 0,10 0,10 10,0 10,0 10),(1 1,1 10,1 10,2 10,2 10,2 1))"),
                false,
                "pg046");
    // hole touches exterior ring at one point
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,1 10,2 1))"),
                true,
                "pg047");
    // "hole" is outside the exterior ring, but touches it
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(5 10,4 11,6 11))"),
                false,
                "pg048");
    // hole touches exterior ring at vertex
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(0 0,1 4,4 1))"),
                true,
                "pg049");
    // "hole" is completely outside the exterior ring
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(20 20,20 21,21 21,21 20))"),
                false,
                "pg050");
    // two "holes" completely outside the exterior ring, that touch
    // each other
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(20 0,25 10,21 0),(30 0,25 10,31 0))"),
                false,
                "pg051");

    // example from Norvald Ryeng
    test::apply(from_wkt<OG>("POLYGON((58 31,56.57 30,62 33),(35 9,28 14,31 16),(23 11,29 5,26 4))"),
                false,
                "pg052");
    // and with points reversed
    test::apply(from_wkt<OG>("POLYGON((58 31,62 33,56.57 30),(35 9,31 16,28 14),(23 11,26 4,29 5))"),
                false,
                "pg053");

    // "hole" is completely inside another "hole"
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,1 9,9 9,9 1),(2 2,2 8,8 8,8 2))"),
                false,
                "pg054");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,1 9,9 9,9 1),(2 2,8 2,8 8,2 8))"),
                false,
                "pg055");

    // "hole" is inside another "hole" (touching)
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,1 9,9 9,9 1),(2 2,2 8,8 8,9 6,8 2))"),
                false,
                "pg056");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,1 9,9 9,9 1),(2 2,8 2,9 6,8 8,2 8))"),
                false,
                "pg057");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,9 1,9 9,1 9),(2 2,2 8,8 8,9 6,8 2))"),
                false,
                "pg058");
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,9 1,9 9,1 9),(2 2,8 2,9 6,8 8,2 8))"),
                false,
                "pg059");
    // hole touches exterior ring at two points
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(5 0,0 5,5 5))"),
                false,
                "pg060");

    // cases with more holes
    // two holes, touching the exterior at the same point
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(0 0,1 9,2 9),(0 0,9 2,9 1))"),
                true,
                "pg061");
    test::apply(from_wkt<OG>("POLYGON((0 0,0 0,10 0,10 10,0 10,0 0),(0 0,0 0,1 9,2 9),(0 0,0 0,9 2,9 1))"),
                AllowDuplicates,
                "pg062");
    test::apply(from_wkt<OG>("POLYGON((0 10,0 0,0 0,0 0,10 0,10 10),(2 9,0 0,0 0,1 9),(9 1,0 0,0 0,9 2))"),
                AllowDuplicates,
                "pg063");
    // two holes, one inside the other
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(0 0,1 9,9 1),(0 0,4 5,5 4))"),
                false,
                "pg064");
    // 1st hole touches has common segment with 2nd hole
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,1 5,5 5,5 1),(5 4,5 8,8 8,8 4))"),
                false,
                "pg065");
    // 1st hole touches 2nd hole at two points
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,1 9,9 9,9 8,2 8,2 1),(2 5,5 8,5 5))"),
                false,
                "pg066");
    // polygon with many holes, where the last two touch at two points
    test::apply(from_wkt<OG>("POLYGON((0 0,20 0,20 20,0 20),(1 18,1 19,2 19,2 18),(3 18,3 19,4 19,4 18),(5 18,5 19,6 19,6 18),(7 18,7 19,8 19,8 18),(9 18,9 19,10 19,10 18),(11 18,11 19,12 19,12 18),(13 18,13 19,14 19,14 18),(15 18,15 19,16 19,16 18),(17 18,17 19,18 19,18 18),(1 1,1 9,9 9,9 8,2 8,2 1),(2 5,5 8,5 5))"),
                false,
                "pg067");
    // two holes completely inside exterior ring but touching each
    // other at a point
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(1 1,1 9,2 9),(1 1,9 2,9 1))"),
                true,
                "pg068");
    // four holes, each two touching at different points
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(0 10,2 1,1 1),(0 10,4 1,3 1),(10 10,9 1,8 1),(10 10,7 1,6 1))"),
                true,
                "pg069");
    // five holes, with two pairs touching each at some point, and
    // fifth hole creating a disconnected component for the interior
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(0 10,2 1,1 1),(0 10,4 1,3 1),(10 10,9 1,8 1),(10 10,7 1,6 1),(4 1,4 4,6 4,6 1))"),
                false,
                "pg070");
    // five holes, with two pairs touching each at some point, and
    // fifth hole creating three disconnected components for the interior
    test::apply(from_wkt<OG>("POLYGON((0 0,10 0,10 10,0 10),(0 10,2 1,1 1),(0 10,4 1,3 1),(10 10,9 1,8 1),(10 10,7 1,6 1),(4 1,4 4,6 4,6 1,5 0))"),
                false,
                "pg071");

    // both examples: a polygon with one hole, where the hole contains
    // the exterior ring
    test::apply(from_wkt<OG>("POLYGON((0 0,1 0,1 1,0 1),(-10 -10,-10 10,10 10,10 -10))"),
                false,
                "pg072");
    test::apply(from_wkt<OG>("POLYGON((-10 -10,1 0,1 1,0 1),(-10 -10,-10 10,10 10,10 -10))"),
                false,
                "pg073");

    test::apply
        (from_wkt<OG>("POLYGON((-6 -10,-6.6923076923076925 -6.711538461538462,\
                      -9 -7,-8.824742268041238 -6.123711340206185,\
                      -10 -6,-8.583333333333332 -4.916666666666667,\
                      -8.094117647058823 -2.4705882352941173,-10 -3,\
                      -8.526315789473683 -0.05263157894736803,-10 1,\
                      -10 10,-7.764705882352941 8.509803921568627,\
                      -7.65090909090909 7.789090909090909,-10 10,\
                      -7.574468085106383 7.304964539007091,-7.4375 6.4375,\
                      -6.5 5.5,-6.4 6,-7.574468085106383 7.304964539007091,\
                      -7.65090909090909 7.789090909090909,\
                      -6.297029702970297 6.514851485148515,\
                      0 0,-6.297029702970297 6.514851485148515,\
                      -4.848484848484849 5.151515151515151,-4 6,\
                      -6.117647058823529 7.411764705882352,\
                      0 0,-6.11764705882353 7.411764705882353,\
                      -7.764705882352941 8.509803921568627,-8 10,\
                      -2.9473684210526314 7.052631578947368,-2 8,\
                      -0.17821782178217824 6.633663366336634,1 10,\
                      1.8095238095238098 5.142857142857142,\
                      3.2038834951456314 4.097087378640777,7 7,\
                      3.7142857142857144 3.7142857142857144,\
                      4.4 3.1999999999999997,8 2,\
                      6.540540540540541 1.5945945945945947,10 -1,\
                      7.454545454545455 -4.393939393939394,8 -5,\
                      7.320754716981132 -4.716981132075472,7 -6,\
                      6.062068965517241 -5.117241379310345,\
                      4.9504132231404965 -5.256198347107438,\
                      6.1506849315068495 -7.123287671232877,9 -8,\
                      6.548387096774194 -7.741935483870968,8 -10,\
                      5.906976744186046 -7.674418604651163,\
                      3.9107142857142856 -7.464285714285714,4 -8,\
                      2.8043478260869565 -7.3478260869565215,\
                      1.7829457364341086 -7.24031007751938,2 -8,\
                      1.0728476821192054 -7.1655629139072845,\
                      -4.3583617747440275 -6.593856655290103,-5 -9,\
                      -5.2020725388601035 -7.720207253886011,-6 -10),\
                      (5.127659574468085 -6.808510638297872,\
                      3.72972972972973 -6.378378378378379,\
                      3.571428571428571 -5.428571428571429,\
                      3.8539325842696632 -5.393258426966292,\
                      5.127659574468085 -6.808510638297872),\
                      (-5.5 4.5,-6.5 5.5,-6.4 6,\
                      -5.263157894736842 4.736842105263158,-5.5 4.5))"),
         false,
         "pg074");

    // test cases coming from buffer
    test::apply
        (from_wkt<OG>("POLYGON((1.1713032141645456 -0.9370425713316364,\
                      5.1713032141645456 4.0629574286683638,\
                      4.7808688094430307 4.3753049524455756,\
                      4.7808688094430307 4.3753049524455756,\
                      0.7808688094430304 -0.6246950475544243,\
                      0.7808688094430304 -0.6246950475544243))"),
         AllowDuplicates,
         "pg075");
}

template <typename Point>
inline void test_doc_example_polygon()
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << " is_valid: doc example polygon " << std::endl;
    std::cout << "************************************" << std::endl;
#endif

    typedef bg::model::polygon<Point> CCW_CG;

    CCW_CG poly;

    typedef validity_tester_areal<true> tester;
    typedef test_valid<tester, CCW_CG> test;

    test::apply(from_wkt<CCW_CG>("POLYGON((0 0,0 10,10 10,10 0,0 0),(0 0,9 1,9 2,0 0),(0 0,2 9,1 9,0 0),(2 9,9 2,9 9,2 9))"),
                false,
                "pg-doc");
}

BOOST_AUTO_TEST_CASE( test_is_valid_polygon )
{
    bool const allow_duplicates = true;
    bool const do_not_allow_duplicates = !allow_duplicates;

    test_open_polygons<point_type, allow_duplicates>();
    test_open_polygons<point_type, do_not_allow_duplicates>();
    test_doc_example_polygon<point_type>();
}

template <typename Point, bool AllowDuplicates>
void test_open_multipolygons()
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << " is_valid: MULTIPOLYGON (open) " << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << "DUPLICATES ALLOWED? "
              << std::boolalpha
              << AllowDuplicates
              << std::noboolalpha
              << std::endl;
#endif

    // cw, ccw, open and closed polygons
    typedef bg::model::polygon<point_type,false,false> ccw_open_polygon_type;
    typedef bg::model::polygon<point_type,false,true>  ccw_closed_polygon_type;
    typedef bg::model::polygon<point_type,true,false>  cw_open_polygon_type;
    typedef bg::model::polygon<point_type,true,true>   cw_closed_polygon_type;

    typedef bg::model::multi_polygon<ccw_open_polygon_type> OG;
    typedef bg::model::multi_polygon<ccw_closed_polygon_type> CG;
    typedef bg::model::multi_polygon<cw_open_polygon_type> CW_OG;
    typedef bg::model::multi_polygon<cw_closed_polygon_type> CW_CG;

    typedef validity_tester_areal<AllowDuplicates> tester;
    typedef test_valid<tester, OG, CG, CW_OG, CW_CG> test;

    // not enough points
    test::apply(from_wkt<OG>("MULTIPOLYGON()"), true, "mpg01");
    test::apply(from_wkt<OG>("MULTIPOLYGON((()))"), false, "mpg02");
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0)),(()))"), false, "mpg03");
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0,1 0)))"), false, "mpg04");

    // two disjoint polygons
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0,1 0,1 1,0 1)),((2 2,3 2,3 3,2 3)))"),
                true,
                "mpg05");

    // two disjoint polygons with multiple points
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0,1 0,1 0,1 1,0 1)),((2 2,3 2,3 3,3 3,2 3)))"),
                AllowDuplicates,
                "mpg06");

    // two polygons touch at a point
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0,1 0,1 1,0 1)),((1 1,2 1,2 2,1 2)))"),
                true,
                "mpg07");

    // two polygons share a segment at a point
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0,1.5 0,1.5 1,0 1)),((1 1,2 1,2 2,1 2)))"),
                false,
                "mpg08");

    // one polygon inside another and boundaries touching
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0,10 0,10 10,0 10)),((0 0,9 1,9 2)))"),
                false,
                "mpg09");

    // one polygon inside another and boundaries not touching
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0,10 0,10 10,0 10)),((1 1,9 1,9 2)))"),
                false,
                "mpg10");

    // free space is disconnected
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0,1 0,1 1,0 1)),((1 1,2 1,2 2,1 2)),((0 1,0 2,-1 2,-1 -1)),((1 2,1 3,0 3,0 2)))"),
                true,
                "mpg11");

    // multi-polygon with a polygon inside the hole of another polygon
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0,100 0,100 100,0 100),(1 1,1 99,99 99,99 1)),((2 2,98 2,98 98,2 98)))"),
                true,
                "mpg12");
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0,100 0,100 100,0 100),(1 1,1 99,99 99,99 1)),((1 1,98 2,98 98,2 98)))"),
                true,
                "mpg13");

    // test case suggested by Barend Gehrels: take two valid polygons P1 and
    // P2 with holes H1 and H2, respectively, and consider P2 to be
    // fully inside H1; now invalidate the multi-polygon by
    // considering H2 as a hole of P1 and H1 as a hole of P2; this
    // should be invalid
    //
    // first the valid case:
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0,100 0,100 100,0 100),(1 1,1 99,99 99,99 1)),((2 2,98 2,98 98,2 98),(3 3,3 97,97 97,97 3)))"),
                true,
                "mpg14");
    // and the invalid case:
    test::apply(from_wkt<OG>("MULTIPOLYGON(((0 0,100 0,100 100,0 100),(3 3,3 97,97 97,97 3)),((2 2,98 2,98 98,2 98),(1 1,1 99,99 99,99 1)))"),
                false,
                "mpg15");

    test::apply
        (from_wkt<OG>("MULTIPOLYGON(((-1 4,8 -10,-10 10,7 -6,8 -2,\
                      -10 10,-10 1,-3 -4,4 1,-1 2,4 3,-8 10,-5 -9,-1 6,-5 0)),\
                      ((-10 -3,-8 1,2 -8,-2 6,-4 0,8 -5,-1 5,8 2)),\
                      ((-6 -10,1 10,4 -8,-7 -2,2 0,-4 3,-10 9)),\
                      ((10 -1,-2 8,-7 3,-6 8,-9 -7,7 -5)),\
                      ((7 7,-4 -4,9 -8,-10 -6)))"),
         false,
         "mpg16");

    test::apply
        (from_wkt<OG>("MULTIPOLYGON(((-1 4,8 -10,-10 10,7 -6,8 -2,\
                      -10 10,-10 1,-3 -4,4 1,-1 2,4 3,-8 10,-5 -9,-1 6,-5 0)),\
                      ((-10 -3,-8 1,2 -8,-2 6,-4 0,8 -5,-1 5,8 2)),\
                      ((-6 -10,-10 9,-4 3,2 0,-7 -2,4 -8,1 10)),\
                      ((10 -1,-2 8,-7 3,-6 8,-9 -7,7 -5)),\
                      ((7 7,-10 -6,9 -8,-4 -4)))"),
         false,
         "mpg17");


    // test cases coming from buffer
    {
        std::string wkt = "MULTIPOLYGON(((1.1713032141645456 -0.9370425713316364,5.1713032141645456 4.0629574286683638,4.7808688094430307 4.3753049524455756,4.7808688094430307 4.3753049524455756,0.7808688094430304 -0.6246950475544243,0.7808688094430304 -0.6246950475544243,1.1713032141645456 -0.9370425713316364)))";

        OG open_mpgn = from_wkt<OG>(wkt);
        bg::reverse(open_mpgn);

        test::apply(open_mpgn, false, "mpg18");
    }
    {
        std::string wkt = "MULTIPOLYGON(((5.2811206375710933 9.9800205994776228,5.2446420208654896 10.0415020265598844,5.1807360092909640 10.1691699739962242,5.1261005500004773 10.3010716408018013,5.0810140527710059 10.4365348863171388,5.0457062680576819 10.5748694208940446,5.0203571162381344 10.7153703234534277,5.0050957707794934 10.8573216336015328,5.0000000000000000 10.9999999999999964,5.0050957707794925 11.1426783663984619,5.0203571162381344 11.2846296765465670,5.0457062680576801 11.4251305791059501,5.0810140527710042 11.5634651136828559,5.1261005500004755 11.6989283591981934,5.1807360092909622 11.8308300260037704,5.2446420208654869 11.9584979734401102,5.3174929343376363 12.0812816349111927,5.3989175181512774 12.1985553330226910,5.4885008512914810 12.3097214678905669,5.5857864376269024 12.4142135623730923,5.6902785321094269 12.5114991487085145,5.8014446669773028 12.6010824818487190,5.9187183650888020 12.6825070656623602,6.0415020265598844 12.7553579791345104,6.1691699739962260 12.8192639907090360,6.3010716408018030 12.8738994499995236,6.4365348863171405 12.9189859472289950,6.5748694208940472 12.9542937319423199,6.7153703234534312 12.9796428837618656,6.8573216336015381 12.9949042292205075,7.0000000000000036 13.0000000000000000,7.1426783663984690 12.9949042292205075,7.2846296765465750 12.9796428837618656,7.4251305791059590 12.9542937319423181,7.5634651136828657 12.9189859472289932,7.6989283591982032 12.8738994499995201,7.8308300260037802 12.8192639907090324,7.9584979734401209 12.7553579791345069,8.0812816349112033 12.6825070656623566,8.1985553330227017 12.6010824818487137,8.3097214678905793 12.5114991487085092,8.4142135623731029 12.4142135623730869,8.5114991487085252 12.3097214678905598,8.6010824818487297 12.1985553330226821,8.6825070656623708 12.0812816349111838,8.7553579791345193 11.9584979734400996,8.8192639907090431 11.8308300260037580,8.8738994499995290 11.6989283591981810,8.9189859472290003 11.5634651136828417,8.9542937319423235 11.4251305791059359,8.9796428837618691 11.2846296765465510,8.9949042292205093 11.1426783663984441,9.0000000000000000 11.0000000000000000,8.9949042292205075 10.8573216336015346,8.9796428837618656 10.7153703234534294,8.9542937319423181 10.5748694208940464,8.9189859472289950 10.4365348863171405,8.8738994499995236 10.3010716408018030,8.8192639907090360 10.1691699739962278,8.7553579791345122 10.0415020265598862,8.7188787869375428 9.9800200826281831,8.8573216336015381 9.9949042292205075,9.0000000000000036 10.0000000000000000,9.1426783663984690 9.9949042292205075,9.2846296765465759 9.9796428837618656,9.4251305791059590 9.9542937319423181,9.5634651136828648 9.9189859472289932,9.6989283591982041 9.8738994499995201,9.8308300260037793 9.8192639907090324,9.9584979734401209 9.7553579791345069,10.0812816349112033 9.6825070656623566,10.1985553330227017 9.6010824818487137,10.3097214678905793 9.5114991487085092,10.4142135623731029 9.4142135623730869,10.5114991487085252 9.3097214678905598,10.6010824818487297 9.1985553330226821,10.6825070656623708 9.0812816349111838,10.7553579791345193 8.9584979734400996,10.8192639907090431 8.8308300260037580,10.8738994499995290 8.6989283591981810,10.9189859472290003 8.5634651136828417,10.9542937319423235 8.4251305791059359,10.9796428837618691 8.2846296765465510,10.9949042292205093 8.1426783663984441,11.0000000000000000 8.0000000000000000,10.9949042292205075 7.8573216336015355,10.9796428837618656 7.7153703234534294,10.9542937319423181 7.5748694208940464,10.9189859472289950 7.4365348863171405,10.8738994499995236 7.3010716408018030,10.8192639907090360 7.1691699739962269,10.7553579791345122 7.0415020265598862,10.6825070656623620 6.9187183650888047,10.6010824818487208 6.8014446669773063,10.5114991487085163 6.6902785321094296,10.4142135623730958 6.5857864376269051,10.3097214678905704 6.4885008512914837,10.1985553330226946 6.3989175181512792,10.0812816349111962 6.3174929343376380,9.9584979734401138 6.2446420208654887,9.8308300260037740 6.1807360092909640,9.6989283591981970 6.1261005500004764,9.5634651136828595 6.0810140527710050,9.4251305791059536 6.0457062680576810,9.2846296765465706 6.0203571162381344,9.1426783663984654 6.0050957707794925,9.0000000000000018 6.0000000000000000,8.8573216336015363 6.0050957707794925,8.7153703234534312 6.0203571162381344,8.5748694208940481 6.0457062680576810,8.4365348863171423 6.0810140527710050,8.3010716408018048 6.1261005500004764,8.1691699739962278 6.1807360092909622,8.0415020265598880 6.2446420208654878,7.9187183650888064 6.3174929343376363,7.8014446669773072 6.3989175181512783,7.6902785321094314 6.4885008512914819,7.5857864376269060 6.5857864376269033,7.4885008512914846 6.6902785321094278,7.3989175181512810 6.8014446669773045,7.3174929343376389 6.9187183650888029,7.2446420208654896 7.0415020265598844,7.1807360092909640 7.1691699739962251,7.1261005500004773 7.3010716408018013,7.0810140527710059 7.4365348863171379,7.0457062680576819 7.5748694208940437,7.0203571162381344 7.7153703234534268,7.0050957707794934 7.8573216336015328,7.0000000000000000 7.9999999999999973,7.0050957707794925 8.1426783663984619,7.0203571162381344 8.2846296765465670,7.0457062680576801 8.4251305791059501,7.0810140527710042 8.5634651136828559,7.1261005500004755 8.6989283591981934,7.1807360092909622 8.8308300260037704,7.2446420208654869 8.9584979734401102,7.2811219724467575 9.0199799990140797,7.1426783663984654 9.0050957707794925,7.0000000000000009 9.0000000000000000,6.8573216336015363 9.0050957707794925,6.7188786030357956 9.0199806804111571,6.7553579791345184 8.9584979734400996,6.8192639907090431 8.8308300260037580,6.8738994499995290 8.6989283591981810,6.9189859472290003 8.5634651136828417,6.9542937319423235 8.4251305791059359,6.9796428837618683 8.2846296765465510,6.9949042292205084 8.1426783663984441,7.0000000000000000 8.0000000000000000,6.9949042292205075 7.8573216336015355,6.9796428837618656 7.7153703234534294,6.9542937319423190 7.5748694208940464,6.9189859472289950 7.4365348863171405,6.8738994499995236 7.3010716408018030,6.8192639907090369 7.1691699739962269,6.7553579791345113 7.0415020265598862,6.6825070656623620 6.9187183650888047,6.6010824818487208 6.8014446669773063,6.5114991487085163 6.6902785321094296,6.4142135623730949 6.5857864376269051,6.3097214678905704 6.4885008512914837,6.1985553330226946 6.3989175181512792,6.0812816349111953 6.3174929343376380,5.9584979734401138 6.2446420208654887,5.8308300260037731 6.1807360092909640,5.6989283591981970 6.1261005500004764,5.5634651136828603 6.0810140527710050,5.4251305791059536 6.0457062680576810,5.2846296765465715 6.0203571162381344,5.1426783663984654 6.0050957707794925,5.0000000000000009 6.0000000000000000,4.8573216336015363 6.0050957707794925,4.7153703234534312 6.0203571162381344,4.5748694208940481 6.0457062680576810,4.4365348863171423 6.0810140527710050,4.3010716408018048 6.1261005500004764,4.1691699739962287 6.1807360092909622,4.0415020265598880 6.2446420208654878,3.9187183650888064 6.3174929343376363,3.8014446669773077 6.3989175181512783,3.6902785321094314 6.4885008512914819,3.5857864376269064 6.5857864376269033,3.4885008512914846 6.6902785321094278,3.3989175181512805 6.8014446669773045,3.3174929343376389 6.9187183650888029,3.2446420208654896 7.0415020265598844,3.1807360092909640 7.1691699739962251,3.1261005500004773 7.3010716408018013,3.0810140527710059 7.4365348863171379,3.0457062680576819 7.5748694208940437,3.0203571162381349 7.7153703234534268,3.0050957707794934 7.8573216336015328,3.0000000000000000 7.9999999999999973,3.0050957707794925 8.1426783663984619,3.0203571162381344 8.2846296765465670,3.0457062680576801 8.4251305791059501,3.0810140527710042 8.5634651136828559,3.1261005500004755 8.6989283591981934,3.1807360092909618 8.8308300260037704,3.2446420208654869 8.9584979734401102,3.3174929343376358 9.0812816349111927,3.3989175181512770 9.1985553330226910,3.4885008512914810 9.3097214678905669,3.5857864376269024 9.4142135623730923,3.6902785321094269 9.5114991487085145,3.8014446669773028 9.6010824818487190,3.9187183650888020 9.6825070656623602,4.0415020265598844 9.7553579791345104,4.1691699739962260 9.8192639907090360,4.3010716408018030 9.8738994499995236,4.4365348863171405 9.9189859472289950,4.5748694208940472 9.9542937319423199,4.7153703234534312 9.9796428837618656,4.8573216336015381 9.9949042292205075,5.0000000000000036 10.0000000000000000,5.1426783663984690 9.9949042292205075)))";

        OG open_mpgn = from_wkt<OG>(wkt);
        bg::reverse(open_mpgn);

        // polygon has a self-touching point
        test::apply(open_mpgn, false, "mpg19");
    }
    {
        std::string wkt = "MULTIPOLYGON(((-1.1713032141645421 0.9370425713316406,-1.2278293047051545 0.8616467945203863,-1.2795097139219473 0.7828504914601357,-1.3261404828502752 0.7009646351604617,-1.3675375811487496 0.6163123916860891,-1.4035376333829217 0.5292278447680804,-1.4339985637934827 0.4400546773279756,-1.4588001570043776 0.3491448151183161,-1.4778445324579732 0.2568570378324778,-1.4910565307049013 0.1635555631651331,-1.4983840100240693 0.0696086094114048,-1.4997980522022116 -0.0246130577225216,-1.4952930766608652 -0.1187375883622537,-1.4848868624803642 -0.2123935159867641,-1.4686204782339323 -0.3052112234370423,-1.4465581199087858 -0.3968244016261590,-1.4187868575539013 -0.4868714951938814,-1.3854162916543107 -0.5749971294005020,-1.3465781205880585 -0.6608535126285795,-1.3024256208728704 -0.7441018089575634,-1.2531330422537639 -0.8244134753943718,-1.1988949200189114 -0.9014715584824893,-1.1399253072577331 -0.9749719451724563,-1.0764569300911435 -1.0446245630171400,-1.0087402692078766 -1.1101545249551616,-0.9370425713316382 -1.1713032141645441,-0.8616467945203836 -1.2278293047051563,-0.7828504914601331 -1.2795097139219491,-0.7009646351604588 -1.3261404828502767,-0.6163123916860862 -1.3675375811487509,-0.5292278447680773 -1.4035376333829228,-0.4400546773279725 -1.4339985637934838,-0.3491448151183129 -1.4588001570043785,-0.2568570378324746 -1.4778445324579736,-0.1635555631651299 -1.4910565307049017,-0.0696086094114016 -1.4983840100240695,0.0246130577225248 -1.4997980522022114,0.1187375883622569 -1.4952930766608650,0.2123935159867673 -1.4848868624803639,0.3052112234370455 -1.4686204782339316,0.3968244016261621 -1.4465581199087849,0.4868714951938845 -1.4187868575539002,0.5749971294005050 -1.3854162916543096,0.6608535126285824 -1.3465781205880569,0.7441018089575662 -1.3024256208728686,0.8244134753943745 -1.2531330422537621,0.9014715584824917 -1.1988949200189096,0.9749719451724583 -1.1399253072577313,1.0446245630171418 -1.0764569300911420,1.1101545249551634 -1.0087402692078746,1.1713032141645456 -0.9370425713316364,5.1713032141645456 4.0629574286683638,5.1713032141645439 4.0629574286683621,5.2278293047051561 4.1383532054796159,5.2795097139219491 4.2171495085398671,5.3261404828502767 4.2990353648395407,5.3675375811487509 4.3836876083139131,5.4035376333829230 4.4707721552319217,5.4339985637934838 4.5599453226720268,5.4588001570043785 4.6508551848816859,5.4778445324579739 4.7431429621675241,5.4910565307049017 4.8364444368348689,5.4983840100240693 4.9303913905885972,5.4997980522022116 5.0246130577225232,5.4952930766608645 5.1187375883622552,5.4848868624803639 5.2123935159867658,5.4686204782339320 5.3052112234370439,5.4465581199087856 5.3968244016261604,5.4187868575539007 5.4868714951938822,5.3854162916543107 5.5749971294005025,5.3465781205880578 5.6608535126285799,5.3024256208728699 5.7441018089575637,5.2531330422537632 5.8244134753943726,5.1988949200189110 5.9014715584824895,5.1399253072577329 5.9749719451724559,5.0764569300911440 6.0446245630171394,5.0087402692078768 6.1101545249551616,4.9370425713316379 6.1713032141645439,4.8616467945203841 6.2278293047051561,4.7828504914601337 6.2795097139219482,4.7009646351604593 6.3261404828502759,4.6163123916860869 6.3675375811487509,4.5292278447680783 6.4035376333829230,4.4400546773279732 6.4339985637934838,4.3491448151183141 6.4588001570043785,4.2568570378324750 6.4778445324579739,4.1635555631651311 6.4910565307049017,4.0696086094114028 6.4983840100240693,3.9753869422774759 6.4997980522022116,3.8812624116377439 6.4952930766608645,3.7876064840132333 6.4848868624803639,3.6947887765629552 6.4686204782339320,3.6031755983738387 6.4465581199087847,3.5131285048061165 6.4187868575539007,3.4250028705994957 6.3854162916543098,3.3391464873714183 6.3465781205880578,3.2558981910424345 6.3024256208728691,3.1755865246056261 6.2531330422537623,3.0985284415175087 6.1988949200189101,3.0250280548275423 6.1399253072577320,2.9553754369828584 6.0764569300911422,2.8898454750448366 6.0087402692078751,2.8286967858354544 5.9370425713316362,-1.1713032141645456 0.9370425713316364,-1.1713032141645421 0.9370425713316406)))";

        OG open_mpgn = from_wkt<OG>(wkt);
        bg::reverse(open_mpgn);

        // polygon contains a spike
        test::apply(open_mpgn, false, "mpg20");
    }
}

BOOST_AUTO_TEST_CASE( test_is_valid_multipolygon )
{
    bool const allow_duplicates = true;
    bool const do_not_allow_duplicates = !allow_duplicates;

    test_open_multipolygons<point_type, allow_duplicates>();
    test_open_multipolygons<point_type, do_not_allow_duplicates>();
}

BOOST_AUTO_TEST_CASE( test_is_valid_variant )
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << " is_valid: variant support" << std::endl;
    std::cout << "************************************" << std::endl;
#endif

    typedef bg::model::polygon<point_type> polygon_type; // cw, closed

    typedef boost::variant
        <
            linestring_type, multi_linestring_type, polygon_type
        > variant_geometry;
    typedef test_valid_variant<variant_geometry> test;

    variant_geometry vg;

    linestring_type valid_linestring =
        from_wkt<linestring_type>("LINESTRING(0 0,1 0)");
    multi_linestring_type invalid_multi_linestring =
        from_wkt<multi_linestring_type>("MULTILINESTRING((0 0,1 0),(0 0))");
    polygon_type valid_polygon =
        from_wkt<polygon_type>("POLYGON((0 0,1 1,1 0,0 0))");
    polygon_type invalid_polygon =
        from_wkt<polygon_type>("POLYGON((0 0,1 1,1 0))");

    vg = valid_linestring;
    test::apply(vg, true, "v01");
    vg = invalid_multi_linestring;
    test::apply(vg, false, "v02");
    vg = valid_polygon;
    test::apply(vg, true, "v03");
    vg = invalid_polygon;
    test::apply(vg, false, "v04");
}
