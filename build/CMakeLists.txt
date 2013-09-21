cmake_minimum_required(VERSION 2.6.0)
include(Bundle.cmake)
include(PCHSupport.cmake)
 
# Only debug & release builds
# This must be before the project command
if(WIN32 OR APPLE)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
	set(LIB_SUFFIX "") 
endif(WIN32 OR APPLE)

if(UNIX AND NOT APPLE)
	set(LINUX ON)
	set(LIB_SUFFIX /${CMAKE_BUILD_TYPE}) 
endif(UNIX AND NOT APPLE)


# Knock off ZERO_TEST at least BUILD_ALL remains
set(CMAKE_SUPPRESS_REGENERATION 1)

set(PROJECT_NAME wxKinectHelper)
project(${PROJECT_NAME})

# Location where cmake first looks for cmake modules.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

if(WIN32)
   set(OS_BASE_NAME Win)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
   set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
   set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
   set(wxWidgets_USE_LIBS core net base)
endif(WIN32)
if(APPLE)
    set(OS_BASE_NAME Mac)
    set(PREPROCESSOR_DEFINITIONS /DWXUSINGDLL;/D__WXMAC__;/D__WXOSX__;/D__WXOSX_COCOA__;/DwxUSE_BASE=1;/D_FILE_OFFSET_BITS=64;/D_LARGE_FILES;/DWX_PRECOMP=1;/DMACOS_CLASSIC)
    set(CMAKE_OSX_SYSROOT "macosx10.5")
    set(wxWidgets_USE_LIBS core net base)
endif(APPLE)
if(LINUX)
    set(OS_BASE_NAME Linux)
    set(wxWidgets_USE_LIBS core net base)
    set(PREPROCESSOR_DEFINITIONS /D__WXGTK__;/D_FILE_OFFSET_BITS=64)
endif(LINUX)

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)
set(BASE_INCLUDE_DIRECTORIES ${PROJECT_ROOT_DIR}/include)

set(BUILD_SHARED_LIBS 0)

find_package(wxWidgets REQUIRED)	
include(FindKinectSDK.cmake)
if(USE_OPENCV)
	find_package(OpenCV REQUIRED)
endif(USE_OPENCV)
if(USE_PCL)
	find_package(PCL 1.3 REQUIRED)
endif(USE_PCL)	

message("wxWidgets Definitons" ${wxWidgets_DEFINITIONS})

if(WIN32)
	set(wxWidgets_DEFINITIONS /DUNICODE;/D_UNICODE)
	set(wxWidgets_DEFINITIONS ${wxWidgets_DEFINITIONS};/D_CRT_SECURE_NO_DEPRECATE;/D_CRT_NONSTDC_NO_DEPRECATE;/D_UNICODE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /wd4996")
endif(WIN32)

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)
set(BASE_INCLUDE_DIRECTORIES ${PROJECT_ROOT_DIR}/include)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__WXDEBUG__=1" )

add_subdirectory (../wxKinectHelper ../../wxKinectHelper/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory (../KinectTest ../../KinectTest/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory (../KinectServer ../../KinectServer/${OS_BASE_NAME}${LIB_SUFFIX})
#add_subdirectory (../ServerTest ../../ServerTest/${OS_BASE_NAME}${LIB_SUFFIX})
if(USE_OPENCV)
	add_subdirectory (../ObjDetector ../../ObjDetector/${OS_BASE_NAME}${LIB_SUFFIX})
endif(USE_OPENCV)
if(USE_PCL)
	add_subdirectory (../Segmentation ../../Segmentation/${OS_BASE_NAME}${LIB_SUFFIX})
endif(USE_PCL)
add_subdirectory (../KinectTypes ../../KinectTypes/${OS_BASE_NAME}${LIB_SUFFIX})
add_subdirectory (../3dparty/libjson/build ../../3dparty/libjson/build/${OS_BASE_NAME}${LIB_SUFFIX})
if(WIN32)
	add_subdirectory (../libevent ../../libevent/${OS_BASE_NAME}${LIB_SUFFIX})
endif(WIN32)

