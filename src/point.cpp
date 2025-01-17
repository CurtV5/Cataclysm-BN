#include "point.h"

#include <algorithm>
#include <sstream>
#include <utility>

#include "cata_utility.h"

std::string point::to_string() const
{
    std::ostringstream os;
    os << *this;
    return os.str();
}

std::string tripoint::to_string() const
{
    std::ostringstream os;
    os << *this;
    return os.str();
}

std::ostream &operator<<( std::ostream &os, const point &pos )
{
    return os << "(" << pos.x << "," << pos.y << ")";
}

std::ostream &operator<<( std::ostream &os, const tripoint &pos )
{
    return os << "(" << pos.x << "," << pos.y << "," << pos.z << ")";
}

point clamp( const point &p, const half_open_rectangle &r )
{
    return point( clamp( p.x, r.p_min.x, r.p_max.x - 1 ), clamp( p.y, r.p_min.y, r.p_max.y - 1 ) );
}

point clamp( const point &p, const inclusive_rectangle &r )
{
    return point( clamp( p.x, r.p_min.x, r.p_max.x ), clamp( p.y, r.p_min.y, r.p_max.y ) );
}

std::vector<tripoint> closest_points_first( const tripoint &center, int max_dist )
{
    return closest_points_first( center, 0, max_dist );
}

std::vector<tripoint> closest_points_first( const tripoint &center, int min_dist, int max_dist )
{
    const std::vector<point> points = closest_points_first( center.xy(), min_dist, max_dist );

    std::vector<tripoint> result;
    result.reserve( points.size() );

    for( const point &p : points ) {
        result.emplace_back( p, center.z );
    }

    return result;
}

std::vector<point> closest_points_first( const point &center, int max_dist )
{
    return closest_points_first( center, 0, max_dist );
}

std::vector<point> closest_points_first( const point &center, int min_dist, int max_dist )
{
    min_dist = std::max( min_dist, 0 );
    max_dist = std::max( max_dist, 0 );

    if( min_dist > max_dist ) {
        return {};
    }

    const int min_edge = min_dist * 2 + 1;
    const int max_edge = max_dist * 2 + 1;

    const int n = max_edge * max_edge - ( min_edge - 2 ) * ( min_edge - 2 );
    const bool is_center_included = min_dist == 0;

    std::vector<point> result;
    result.reserve( n + ( is_center_included ? 1 : 0 ) );

    if( is_center_included ) {
        result.push_back( center );
    }

    int x = std::max( min_dist, 1 );
    int y = 1 - x;

    int dx = 1;
    int dy = 0;

    for( int i = 0; i < n; i++ ) {
        result.push_back( center + point{ x, y } );

        if( x == y || ( x < 0 && x == -y ) || ( x > 0 && x == 1 - y ) ) {
            std::swap( dx, dy );
            dx = -dx;
        }

        x += dx;
        y += dy;
    }

    return result;
}
