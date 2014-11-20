// Boost.Geometry (aka GGL, Generic Geometry Library)
// Unit Test

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// This file was modified by Oracle on 2014.
// Modifications copyright (c) 2014 Oracle and/or its affiliates.

// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <geometry_test_common.hpp>

#include <boost/concept_check.hpp>

#include <boost/geometry/extensions/gis/geographic/strategies/vincenty.hpp>

#include <boost/geometry/core/srs.hpp>
#include <boost/geometry/strategies/strategies.hpp>
#include <boost/geometry/algorithms/assign.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <test_common/test_point.hpp>

#ifdef HAVE_TTMATH
#  include <boost/geometry/extensions/contrib/ttmath_stub.hpp>
#endif


template <typename T>
void normalize_deg(T & deg)
{
    while ( deg > 180 )
        deg -= 360;
    while ( deg <= -180 )
        deg += 360;
}

double azimuth(double deg, double min, double sec)
{
    return deg + min/60.0 + sec/3600.0;
}

template <typename P1, typename P2, typename Spheroid>
void test_vincenty(double lon1, double lat1, double lon2, double lat2,
                   double expected_km,
                   double expected_azimuth_12,
                   double expected_azimuth_21,
                   Spheroid const& spheroid)
{
    // Set radius type, but for integer coordinates we want to have floating point radius type
    typedef typename bg::promote_floating_point
        <
            typename bg::coordinate_type<P1>::type
        >::type rtype;

    // formula
    {
        bg::detail::vincenty_inverse<double> vi(lon1 * bg::math::d2r,
                                                lat1 * bg::math::d2r,
                                                lon2 * bg::math::d2r,
                                                lat2 * bg::math::d2r,
                                                spheroid);
        double dist = vi.distance();
        double az12 = vi.azimuth12();
        double az21 = vi.azimuth21();

        double az12_deg = az12 * bg::math::r2d;
        double az21_deg = az21 * bg::math::r2d;
        
        // normalize angles
        normalize_deg(az12_deg);
        normalize_deg(az21_deg);
        normalize_deg(expected_azimuth_12);
        normalize_deg(expected_azimuth_21);
        
        BOOST_CHECK_CLOSE(dist, 1000.0 * expected_km, 0.001);
        BOOST_CHECK_CLOSE(az12_deg, expected_azimuth_12, 0.001);
        //BOOST_CHECK_CLOSE(az21_deg, expected_azimuth_21, 0.001);
    }

    // strategy
    {
        typedef bg::strategy::distance::vincenty<Spheroid> vincenty_type;

        BOOST_CONCEPT_ASSERT(
            (
                bg::concept::PointDistanceStrategy<vincenty_type, P1, P2>)
            );

        vincenty_type vincenty;
        typedef typename bg::strategy::distance::services::return_type<vincenty_type, P1, P2>::type return_type;

        P1 p1, p2;

        bg::assign_values(p1, lon1, lat1);
        bg::assign_values(p2, lon2, lat2);

        BOOST_CHECK_CLOSE(vincenty.apply(p1, p2), return_type(1000.0) * return_type(expected_km), 0.001);
    }
}

template <typename P1, typename P2>
void test_vincenty(double lon1, double lat1, double lon2, double lat2,
                   double expected_km,
                   double expected_azimuth_12,
                   double expected_azimuth_21)
{
    test_vincenty<P1, P2>(lon1, lat1, lon2, lat2,
                          expected_km, expected_azimuth_12, expected_azimuth_21,
                          bg::srs::spheroid<double>());
}

template <typename P1, typename P2>
void test_all()
{
    // See:
    //  - http://www.ga.gov.au/geodesy/datums/vincenty_inverse.jsp
    //  - http://www.ga.gov.au/geodesy/datums/vincenty_direct.jsp
    // Values in the comments below was calculated using the above pages
    // in some cases distances may be different, previously used values was left

    double gda_a = 6378137.0;
    double gda_f = 1.0 / 298.25722210;
    double gda_b = gda_a * ( 1.0 - gda_f );
    bg::srs::spheroid<double> gda_spheroid(gda_a, gda_b);

    // FlindersPeak -> Buninyong
    test_vincenty<P1, P2>(azimuth(144,25,29.52440), azimuth(-37,57,3.72030),
                          azimuth(143,55,35.38390), azimuth(-37,39,10.15610),
                          54.972271, azimuth(306,52,5.37), azimuth(127,10,25.07),
                          gda_spheroid);

    test_vincenty<P1, P2>(0, 0, 0, 50, 5540.847042, 0, 180); // N
    test_vincenty<P1, P2>(0, 0, 0, -50, 5540.847042, 180, 0); // S
    test_vincenty<P1, P2>(0, 0, 50, 0, 	5565.974540, 90, -90); // E
    test_vincenty<P1, P2>(0, 0, -50, 0, 5565.974540, -90, 90); // W
    
    test_vincenty<P1, P2>(0, 0, 50, 50, 7284.879297, azimuth(32,51,55.87), azimuth(237,24,50.12)); // NE

    // 1005.150875, 178deg 53' 23.85'', 359deg 53' 18.35''
    test_vincenty<P1, P2>(0, 89, 1, 80, 1005.1535769, azimuth(178,53,23.85), azimuth(359,53,18.35)); // sub-polar

    test_vincenty<P1, P2>(4, 52, 4, 52, 0.0, 0, 0); // no point difference

    // 1336.027219, 183deg 41' 29.08'', 2deg 58' 5.13''
    test_vincenty<P1, P2>(4, 52, 3, 40, 1336.039890, azimuth(183,41,29.08), azimuth(2,58,5.13)); // normal case
}

template <typename P>
void test_all()
{
    test_all<P, P>();
}

int test_main(int, char* [])
{

    //test_all<float[2]>();
    //test_all<double[2]>();
    test_all<bg::model::point<int, 2, bg::cs::geographic<bg::degree> > >();
    test_all<bg::model::point<float, 2, bg::cs::geographic<bg::degree> > >();
    test_all<bg::model::point<double, 2, bg::cs::geographic<bg::degree> > >();

#if defined(HAVE_TTMATH)
    test_all<bg::model::point<ttmath::Big<1,4>, 2, bg::cs::geographic<bg::degree> > >();
    test_all<bg::model::point<ttmath_big, 2, bg::cs::geographic<bg::degree> > >();
#endif


    return 0;
}
