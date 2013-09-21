set(SRCS 
	ServerTest.cpp
	../KinectTest/KinectDatasender.cpp
	)
set(HFILES 
	../KinectTest/KinectDatasender.h)

set(INCLUDE_DIRECTORIES ${BASE_INCLUDE_DIRECTORIES} 
   ${PROJECT_ROOT_DIR}/wxKinectHelper
   ${PROJECT_ROOT_DIR}/KinectTest
   ${wxWidgets_INCLUDE_DIRS}
   ${PROJECT_ROOT_DIR}/KinectTypes
   )
set(LINK_DIRECTORIES ${LINK_DIRECTORIES};
   ${PROJECT_ROOT_DIR}/wxKinectHelper/${OS_BASE_NAME}${LIB_SUFFIX}
   ${PROJECT_ROOT_DIR}/KinectTypes/${OS_BASE_NAME}${LIB_SUFFIX}
)
set(EXECUTABLE_NAME ServerTest)

set(SERVER_TEST_DEPENDENCIES wxKinectHelper KinectTypes)

if(WIN32)
	set(SERVER_TEST_DEPENDENCIES ${SERVER_TEST_DEPENDENCIES} libevent)
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};${wxWidgets_DEFINITIONS};/DwxUSE_NO_MANIFEST=1)
	set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} 
	   ${PROJECT_ROOT_DIR}/libevent/include
	   ${PROJECT_ROOT_DIR}/libevent/WIN32-Code
	)
	set(LINK_DIRECTORIES ${LINK_DIRECTORIES} 
		${KinectSDK_LIB_DIR}
		${PROJECT_ROOT_DIR}/libevent/${OS_BASE_NAME}${LIB_SUFFIX})
	set(LIBS ${LIBS} ${KinectSDK_LIBRARIES})
else()
	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS})
endif(WIN32)

if(APPLE)
   set(LINK_DIRECTORIES ${LINK_DIRECTORIES};/usr/lib)
endif(APPLE)

if(LINUX)
   set(LINK_DIRECTORIES ${LINK_DIRECTORIES};/usr/local/lib)
   set(THIRDPARTY_LIBS freenect)
endif(LINUX)

if(WIN32)
   set(OS_LIBS comctl32.lib rpcrt4.lib winmm.lib advapi32.lib wsock32.lib ws2_32.lib wldap32.lib gdiplus.lib)
   set(THIRDPARTY_LIBS libevent.lib)
endif(WIN32)


add_definitions(${PREPROCESSOR_DEFINITIONS})

include_directories(${INCLUDE_DIRECTORIES})

set(SRCS ${SRCS} ${HFILES})

link_directories(${LINK_DIRECTORIES} ${wxWidgets_LIB_DIR})

add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_TYPE} ${SRCS})

set(LIBS ${LIBS} ${wxWidgets_LIBRARIES} ${THIRDPARTY_LIBS} ${OS_LIBS})
set(LIBS wxKinectHelper KinectTypes ${LIBS})
target_link_libraries(${EXECUTABLE_NAME} ${LIBS})

set(EXE_DIR bin)
set(TARGET_LOCATION ${PROJECT_SOURCE_DIR}/${EXE_DIR}${LIB_SUFFIX})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TARGET_LOCATION})

add_dependencies(${EXECUTABLE_NAME} ${SERVER_TEST_DEPENDENCIES})

if(APPLE)
   set_target_properties(${EXECUTABLE_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
   set(BUNDLE_LOC ${TARGET_LOCATION}/$(CONFIGURATION)/${EXECUTABLE_NAME})
endif(APPLE)

