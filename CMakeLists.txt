cmake_minimum_required(VERSION 2.8)
project( loop )
find_package( OpenCV REQUIRED )
# Locate libraries and headers

# Include headers
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( loop loop.cpp )


target_link_libraries( loop ${OpenCV_LIBS} )

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
find_path(WIRINGPI_INCLUDE_DIRS NAMES wiringPi.h)

target_link_libraries( loop ${WIRINGPI_LIBRARIES} )