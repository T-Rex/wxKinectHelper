set(SRCS 
	PclSegmentation.cpp
	PclObjectDetector.cpp
	)
set(HFILES 
	PclSegmentation.h
	PclObjectDetector.h
	)
set(SRCS ${SRCS} ${HFILES})
set(LIBRARY_NAME Segmentation)
set(SEGMENTATION_DEPENDENCIES KinectTypes)
set(INCLUDE_DIRECTORIES
	${INCLUDE_DIRECTORIES}    
	${PROJECT_ROOT_DIR}/KinectTypes
	${PCL_INCLUDE_DIRS}
	)
set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};${PCL_DEFINITIONS})

include_directories(${INCLUDE_DIRECTORIES})
add_definitions(${PREPROCESSOR_DEFINITIONS})
add_library(${LIBRARY_NAME} ${SRCS})
add_dependencies(${LIBRARY_NAME} ${SEGMENTATION_DEPENDENCIES})

