#!/bin/bash
echo OS Type: $OSTYPE

export WX_CONFIG="/usr/bin/wx-config"

# ----------------------------------
# build Debug configuration makefile
# ----------------------------------
echo building Debug configuration makefile
echo directory "LinuxDebug"
rm -drf "LinuxDebug"
mkdir "LinuxDebug"
cd "LinuxDebug"
cmake -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=Debug -DwxWidgets_CONFIG_EXECUTABLE=${WX_CONFIG} ../ 
	cmake -DCMAKE_BUILD_TYPE:STRING=Debug ../ 

#cmake -DwxWidgets_CONFIG_EXECUTABLE=/usr/local/bin/wxgtk2u-2.8-config \
#	-DCMAKE_C_COMPILER:STRING=cc \
#	-DCMAKE_CXX_COMPILER:STRING=c++ \
#	-DCMAKE_C_FLAGS:STRING=-O2 \
#	-pipe  -fno-strict-aliasing \
#	-DCMAKE_CXX_FLAGS:STRING=-O2 \
#	-pipe -fno-strict-aliasing  \
#	-DCMAKE_INSTALL_PREFIX:PATH=/usr/local \
#	-DCMAKE_BUILD_TYPE:STRING=Release \
#	-DTHREADS_HAVE_PTHREAD_ARG:BOOL=YES \
#	-DCMAKE_THREAD_LIBS:STRING=-pthread \
#	-DCMAKE_USE_PTHREADS:BOOL=ON \
#	-DCMAKE_EXE_LINKER_FLAGS:STRING=-pthread \
#	-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON ..

cd ..

# ----------------------------------
# build Release configuration makefile
# ----------------------------------
echo building Release configuration makefile
echo directory "LinuxRelease"
rm -drf "LinuxRelease"
mkdir "LinuxRelease"
cd "LinuxRelease"
cmake -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=Release ../ 
#	cmake -DCMAKE_BUILD_TYPE:STRING=Release ../ 
cd ..

