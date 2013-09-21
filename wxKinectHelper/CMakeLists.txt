set(SRCS 
   KinectGrabberBase.cpp
   KinectHelper.cpp
   SkeletonPainter.cpp
   KinectHelperImpl.cpp
)

set(HFILES 
   KinectGrabberBase.h
   KinectHelper.h
   SkeletonPainter.h
   SkeletonPainterImpl.h
   KinectHelperImpl.h
)

set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES} ${wxWidgets_INCLUDE_DIRS})
#Add Kinect Data type header
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${PROJECT_ROOT_DIR}/KinectTypes)
set(LIBRARY_NAME wxKinectHelper)

if(WIN32)
   set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${KinectSDK_INCLUDE_DIR})
   set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_LIB;/D_UNICODE)
   set(SRCS ${SRCS} KinectGrabberMSW.cpp SkeletonPainterImplMSW.cpp KinectHelperImplMSW.cpp)
   set(HFILES ${HFILES} KinectGrabberMSW.h SkeletonPainterImplMSW.h KinectHelperImplMSW.h)
endif(WIN32)

if(LINUX)
   set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} /usr/local/include/libfreenect)
   set(SRCS ${SRCS} KinectGrabberFreenect.cpp KinectHelperImplFreenect.cpp)
   set(HFILES ${HFILES} KinectGrabberFreenect.h KinectHelperImplFreenect.h)
endif(LINUX)

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})
add_library(${LIBRARY_NAME} STATIC ${SRCS} ${HFILES})

