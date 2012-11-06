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
    
#include <boost/foreach.hpp>
#include <boost/python.hpp>
namespace bp = boost::python;

#include <fstream>
#include <list>

#include "rpg.hpp"

// generate polygon with size vertices, and given seed for random-generator
bp::list rpg2(int size, unsigned int seed=42) {   
    std::vector< std::pair<double,double> > point_list = rpg(size,seed);
    bp::list output;
    typedef std::pair<double,double> pt_type;
    BOOST_FOREACH( pt_type pt, point_list ) {
        bp::list point;
        point.append(pt.first);
        point.append(pt.second);
        output.append(point);
    }
    return output;
}

bp::list rpg1(int size) {
    return rpg2(size,42); // fixme: take seed from clock?
}

BOOST_PYTHON_MODULE(randompolygon) {
    bp::def("rpg", rpg2); // two argument call
    bp::def("rpg", rpg1); // one argument call
    bp::def("version", version); // version string
}
