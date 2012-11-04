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

#pragma once

#include <fstream>
#include <list>
#include <vector>



// generate polygon with size vertices
std::vector< std::pair<double,double> > rpg(int size, unsigned int seed=42);

std::string version();
