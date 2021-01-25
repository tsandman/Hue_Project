cmake_minimum_required( VERSION 3.10 )
set(CMAKE_BUILD_TYPE Debug)

project( Hue_Project )

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 

#find_package(jsoncpp REQUIRED) 
#get_target_property (JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES) 


#include(FindPkgConfig)
#pkg_check_modules(JSONCPP jsoncpp )

add_subdirectory( lib/jsoncpp )

# add the executable
add_executable( Hue_Project main.cpp HueManager.cpp Utility.cpp )

include_directories( ${CURL_INCLUDE_DIR} )

#target_link_libraries( Hue_Project ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES} )
target_link_libraries( Hue_Project ${CURL_LIBRARIES} "$ENV{HUE_PROJECT_BUILD_DIR}/../lib/jsoncpp/build/debug/lib/libjsoncpp.a" )