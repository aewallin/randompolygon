cmake_minimum_required(VERSION 3.0)
 
Project(randompolygon)
set(CMAKE_BUILD_TYPE Release)

# find CGAL 
FIND_PACKAGE(CGAL COMPONENTS Core  REQUIRED)
include( ${CGAL_USE_FILE} )

add_definitions(-fPIC)

# this figures out the Python include directories and adds them to the
# header file search path
#execute_process(
#    COMMAND python-config --includes
#    COMMAND sed -r "s/-I//g; s/ +/;/g"
#    COMMAND tr -d '\n'
#    OUTPUT_VARIABLE Python_Includes
#)
#include_directories(${Python_Includes})

find_package(Python3 COMPONENTS Development Interpreter)
MESSAGE(STATUS "Python3_Includes: " ${Python3_INCLUDE_DIRS})
include_directories(${Python3_INCLUDE_DIRS})

#### create version_string.hpp, http://stackoverflow.com/questions/3780667
# include the output directory, where the version_string.hpp file is generated
include_directories(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE(version_string.cmake)
# we can now parse the git-commit-id:
STRING(REGEX REPLACE "([0-9][0-9]).*" "\\1" GIT_MAJOR_VERSION "${GIT_COMMIT_ID}" )
STRING(REGEX REPLACE "[0-9][0-9].([0-9][0-9])-.*" "\\1" GIT_MINOR_VERSION "${GIT_COMMIT_ID}" )
STRING(REGEX REPLACE "[0-9][0-9].[0-9][0-9]-(.*)-.*" "\\1" GIT_PATCH_VERSION "${GIT_COMMIT_ID}" )
SET(MY_VERSION "${GIT_MAJOR_VERSION}.${GIT_MINOR_VERSION}.${GIT_PATCH_VERSION}" CACHE STRING "version")


# a custom target that is always built
#add_custom_target(
#    version_string 
#    ALL
#    DEPENDS ver_string 
#)
# creates version_string.hpp using cmake script
#add_custom_command(
#    OUTPUT ver_string ${CMAKE_CURRENT_BINARY_DIR}/version_string.hpp
#    COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#                                            -P ${CMAKE_CURRENT_SOURCE_DIR}/version_string.cmake
#)
# version_string.hpp is a generated file
#set_source_files_properties(
#    ${CMAKE_CURRENT_BINARY_DIR}/version_string.hpp
#    PROPERTIES GENERATED TRUE
#    HEADER_FILE_ONLY TRUE
#)
#### end create version_string.hpp


find_package( Boost COMPONENTS python REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    MESSAGE(STATUS "Boost_LIB_VERSION: " ${Boost_LIB_VERSION})
    MESSAGE(STATUS "Boost_INCLUDE_DIRS are: " ${Boost_INCLUDE_DIRS})
    MESSAGE(STATUS "Boost_PYTHON_LIBRARY is: " ${Boost_PYTHON_LIBRARY})
    MESSAGE(STATUS "boost_LIBRARY_DIRS is: " ${Boost_LIBRARY_DIRS})
    MESSAGE(STATUS "Boost_LIBRARIES is: " ${Boost_LIBRARIES})    
endif()

set(RPG_INCLUDE_FILES rpg.hpp)
set(RPG_SRC_FILES rpg.cpp)

# the pure c++ shared library
add_library(
    librandompolygon
    SHARED
    ${RPG_SRC_FILES}
)
set_target_properties(librandompolygon PROPERTIES PREFIX "")
set_target_properties(librandompolygon PROPERTIES VERSION ${MY_VERSION}) 
#add_dependencies( 
#    librandompolygon
#    version_string 
#)
# set_target_properties(libopenvoronoi PROPERTIES VERSION ${MY_VERSION}) 

# static library, linked with the python module below
add_library(
    librandompolygon_static
    STATIC
    ${RPG_SRC_FILES}
)
#add_dependencies( 
#    librandompolygon_static
#    version_string 
#)

# this makes the Python module
add_library(
    randompolygon
    MODULE
    rpg_py.cpp
)
set_target_properties(randompolygon PROPERTIES PREFIX "") 
set_target_properties(randompolygon PROPERTIES VERSION ${MY_VERSION}) 
target_link_libraries(randompolygon librandompolygon_static ${Boost_LIBRARIES} ) 
#add_dependencies( 
#    randompolygon
#    version_string 
#)

# this figures out where to install the Python modules
#execute_process(
#    COMMAND python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()"
#    OUTPUT_VARIABLE Python_site_packages
#    OUTPUT_STRIP_TRAILING_WHITESPACE
#)
#MESSAGE(STATUS "Python libraries will be installed to: " ${Python_site_packages})

#
# this figures out where to install the Python modules
#
#execute_process(
#    COMMAND python -c "from distutils.sysconfig import get_python_lib; print get_python_lib(0,0,\"/usr/local\")"
#    OUTPUT_VARIABLE Python_site_packages
#    OUTPUT_STRIP_TRAILING_WHITESPACE
#) 
#MESSAGE(STATUS "Python module will be installed to: " ${Python_site_packages})    
# on Ubuntu 11.10 this outputs: /usr/local/lib/python2.7/dist-packages

# strip away /usr/local/  because that is what CMAKE_INSTALL_PREFIX is set to
# also, since there is no leading "/", it makes ${Python_site_packages} a relative path.
#STRING(REGEX REPLACE "/usr/local/(.*)$" "\\1" Python_site_packages "${Python_site_packages}" )
MESSAGE(STATUS "CMAKE_INSTALL_PREFIX is : " ${CMAKE_INSTALL_PREFIX})
MESSAGE(STATUS "Python libraries will be installed to: " ${Python3_SITELIB})


# this installs the python library
install(
    TARGETS randompolygon
    LIBRARY DESTINATION ${Python3_SITELIB}
)


# this installs the c++ library
install(
    TARGETS librandompolygon
    LIBRARY 
    DESTINATION lib/randompolygon
    ARCHIVE DESTINATION lib/randompolygon
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

# this installs the c++ include headers
install(
    FILES ${RPG_INCLUDE_FILES}
    DESTINATION include/randompolygon
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
)
    
########################################################################
# "make package" stuff
execute_process(
    COMMAND dpkg --print-architecture
    OUTPUT_VARIABLE DEB_ARCHITECTURE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
MESSAGE(STATUS "DEB_ARCHITECTURE is: " ${DEB_ARCHITECTURE})
include(deb/package_details.cmake)

# "make spackage" will build a source package
add_custom_target(spackage 
              ${CMAKE_COMMAND} 
              -D SRC_DIR:STRING=${CMAKE_SOURCE_DIR} 
              -D MY_VERSION:STRING=${MY_VERSION} 
              -C ${CMAKE_SOURCE_DIR}/deb/package_details.cmake
              -P ${CMAKE_CURRENT_SOURCE_DIR}/deb/DebSourcePPA.cmake 
            ) 

MESSAGE(STATUS " 'make' builds the c++ library and the python module" )
MESSAGE(STATUS " 'make package' builds a binary DEB package" )
MESSAGE(STATUS " 'make spackage' builds a source DEB package" )
MESSAGE(STATUS " 'make install' installs" )
   
# "make package" builds a binary debian package
include(CPack) # this should be last
