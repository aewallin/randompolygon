/*
 *  Copyright Anders E.E. Wallin (anders.e.e.wallin "at" gmail.com"
 *  December 2011.
 * 
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    * 
    */
    
// from: http://www.cgal.org/Manual/latest/doc_html/cgal_manual/Generator_ref/Function_random_polygon_2.html
//
// "2-opt" heuristic for generating a random polygon
//
// 
//

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
#include <CGAL/Random.h>
#include <CGAL/algorithm.h>
#include <CGAL/double.h>

#include <boost/python.hpp>
namespace bp = boost::python;

#include <fstream>
#include <list>

#include "version_string.hpp"

typedef double RT;
typedef CGAL::Simple_cartesian<RT>                        K;
typedef K::Point_2                                        Point_2;
typedef std::list<Point_2>                                Container;
typedef CGAL::Polygon_2<K, Container>                     Polygon_2;
typedef CGAL::Creator_uniform_2<RT, Point_2>             Creator;
typedef CGAL::Random_points_in_disc_2<Point_2, Creator> Point_generator;

typedef Polygon_2::Vertex_iterator VertexItr;

const double RADIUS = 1.0;

// generate polygon with size vertices
bp::list rpg2(int size, unsigned int seed=42) {   
    Polygon_2            polygon;
    std::list<Point_2>   point_set;
    CGAL::Random         rand;
    bool debug = false;
    
    // copy size points from the generator, eliminating duplicates
    // repeat until we have == size vertices
    if (debug) std::cout << "Waiting for " << size << " points..."<<std::flush;
    do {
        point_set.clear();
        CGAL::Random rnd(seed);
        CGAL::copy_n_unique( Point_generator(RADIUS, rnd ), size,
                         std::back_inserter(point_set));
    } while( point_set.size() != size );
    if (debug) std::cout << "Done.\n"<<std::flush;

    /*
    std::ostream_iterator< Point_2 >  out( std::cout, " " );
    std::cout << "From the following " << point_set.size() << " points "
             << std::endl;
    std::copy(point_set.begin(), point_set.end(), out);
    std::cout << std::endl;
    */
    
    CGAL::random_polygon_2(point_set.size(), 
                           std::back_inserter(polygon),
                           point_set.begin());
    //std::cout << "The following simple polygon was made: " << std::endl;
    //std::cout << " N = " << polygon.size() << "\n";
    VertexItr it, it_end;
    it= polygon.vertices_begin();
    it_end = polygon.vertices_end();
    bp::list output;
    for( ; it!=it_end; it++) {
        bp::list point;
        //std::cout << "( " << it->x() << " , " << it->y() << " )"  << "\n";
        point.append(it->x());
        point.append(it->y());
        output.append(point);
    }
    return output;
}

bp::list rpg1(int size) {
    return rpg2(size,42); // fixme: take seed from clock?
}

std::string version() {
    return VERSION_STRING;
}

BOOST_PYTHON_MODULE(rpg) {
    bp::def("rpg", rpg2); // two argument version
    bp::def("rpg", rpg1); // one argument version

    bp::def("version", version);
}
