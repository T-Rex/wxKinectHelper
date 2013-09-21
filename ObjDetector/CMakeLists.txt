
project(ObjDetector)
cmake_minimum_required(VERSION 2.6)

set(SRCS
	ObjectDefinition.cpp
	ObjectDetector.cpp
)
SET (HFILES
	ObjectDefinition.h
	ObjectDetector.h
)
set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES} 
   ${PROJECT_ROOT_DIR}/wxKinectHelper
   ${OpenCV_INCLUDE_DIRS}
)
set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};${OpenCV_DEFINITIONS};/DUSE_FLANN)
# Declare the target (an executable)
set(LIBRARY_NAME ObjDetector)
set (SRCS ${SRCS} ${HFILES})
add_library(${LIBRARY_NAME} STATIC ${SRCS})
add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})
