#
# Stuff the average programmer needs to change
#
set(SRCS 
	../Source/internalJSONNode.cpp
	../Source/JSONAllocator.cpp
	../Source/JSONChildren.cpp
	../Source/JSONDebug.cpp
	../Source/JSONIterators.cpp
	../Source/JSONMemory.cpp
	../Source/JSONNode.cpp
	../Source/JSONNode_Mutex.cpp
	../Source/JSONPreparse.cpp
	../Source/JSONStream.cpp
	../Source/JSONValidator.cpp
	../Source/JSONWorker.cpp
	../Source/JSONWriter.cpp
	../Source/JSON_Base64.cpp
	../Source/libjson.cpp
)
set(HFILES 
	../Source/internalJSONNode.h
	../Source/JSONAllocator.h
	../Source/JSONChildren.h
	../Source/JSONDebug.h
	../Source/JSONDefs.h
	../Source/JSONGlobals.h
	../Source/JSONMemory.h
	../Source/JSONMemoryPool.h
	../Source/JSONNode.h
	../Source/JSONPreparse.h
	../Source/JSONSingleton.h
	../Source/JSONStream.h
	../Source/JSONValidator.h
	../Source/JSONWorker.h
	../Source/JSON_Base64.h
	../Source/NumberToString.h
)
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ../Source)
set(LIBRARY_NAME libjson)
set(SRCS ${SRCS} ${HFILES})
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DJSON_DEBUG")
add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})

add_library(${LIBRARY_NAME} STATIC ${SRCS})
