cmake_minimum_required( VERSION 3.10 )

project( Hue_Project )

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 

include(FindPkgConfig)
pkg_check_modules(JSONCPP jsoncpp )

# add the executable
add_executable( Hue_Project main.cpp )

include_directories(${CURL_INCLUDE_DIR})

target_link_libraries( Hue_Project ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES} )