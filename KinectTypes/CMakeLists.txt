set(SRCS 
	KinectTypes.cpp 
	)
set(HFILES 
	KinectTypes.h
	)

set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES} 
)

set(LIBRARY_NAME KinectTypes)

if(WIN32)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_LIB;/D_UNICODE)
	set(LINK_DIRECTORIES ${LINK_DIRECTORIES})
	set(LIBS ${LIBS})
else()
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS})
endif(WIN32)

if(APPLE)
   set(LINK_DIRECTORIES ${LINK_DIRECTORIES};/usr/lib)
endif(APPLE)

if(LINUX)
   set(LINK_DIRECTORIES ${LINK_DIRECTORIES};/usr/local/lib)
endif(LINUX)

if(WIN32)
   set(OS_LIBS comctl32.lib rpcrt4.lib winmm.lib advapi32.lib wsock32.lib ws2_32.lib wldap32.lib gdiplus.lib)
endif(WIN32)

set(SRCS ${SRCS} ${HFILES})

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})
add_library(${LIBRARY_NAME} STATIC ${SRCS} ${HFILES})
