cmake_minimum_required(VERSION 3.10)
project(WavyPlane)

set(CMAKE_CXX_STANDARD 14)

get_filename_component( CINDER_PATH "~/cinder_master" ABSOLUTE )

include( "${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake" )

#file(COPY assets DESTINATION .)

ci_make_app(
	APP_NAME ${PROJECT_NAME}
        CINDER_PATH ${CINDER_PATH}
        SOURCES main.cpp
)
