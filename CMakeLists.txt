cmake_minimum_required(VERSION 2.8)
project( loop )
find_package( OpenCV REQUIRED )
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( loop loop.cpp )
target_link_libraries( loop ${OpenCV_LIBS} )
target_link_libraries( loop ${WIRINGPI_LIBRARIES})