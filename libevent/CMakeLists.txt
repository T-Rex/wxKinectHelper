#
# Stuff the average programmer needs to change
#
set(SRCS 
	event.c
	buffer.c
	bufferevent.c
	bufferevent_sock.c
	bufferevent_pair.c
	listener.c
	evmap.c
	log.c
	evutil.c
	strlcpy.c
	signal.c
	bufferevent_filter.c
	evthread.c
	bufferevent_ratelim.c
	evutil_rand.c
	win32select.c
	evthread_win32.c
	buffer_iocp.c
	event_iocp.c
	bufferevent_async.c
	event_tagging.c
	http.c
	evdns.c
	evrpc.c)
set(HFILES 
	evdns.h
	event.h
	evhttp.h
	evrpc.h
	evutil.h
	event-internal.h
	http-internal.h
	log-internal.h
	evrpc-internal.h
	evsignal-internal.h
	minheap-internal.h
	strlcpy-internal.h
	#WIN32-Code/config.h
	WIN32-Code/event2/event-config.h
	WIN32-Code/tree.h)
set(INCLUDE_DIRECTORIES 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/WIN32-Code 
	${CMAKE_CURRENT_SOURCE_DIR}/WIN32-Code/event2
	${CMAKE_CURRENT_SOURCE_DIR}/compat)
set(PREPROCESSOR_DEFINITIONS /DHAVE_CONFIG_H;/D_CRT_SECURE_NO_DEPRECATE)
set(LIBRARY_NAME libevent)
if(WIN32)
  	set(PREPROCESSOR_DEFINITIONS ${PREPROCESSOR_DEFINITIONS};/D_LIB)
endif(WIN32)

add_definitions(${PREPROCESSOR_DEFINITIONS})
include_directories(${INCLUDE_DIRECTORIES})

add_library(${LIBRARY_NAME} STATIC ${SRCS} ${HFILES})

set_target_properties(${LIBRARY_NAME} PROPERTIES 
	COMPILE_FLAGS "")
