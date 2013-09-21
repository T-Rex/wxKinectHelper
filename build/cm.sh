#!/bin/bash
echo OS Type: $OSTYPE
extra=""
if [ "$OSTYPE" == "msys" ]
then
	thedir="Win"
	# maketype="NMake Makefiles"
	maketype="Visual Studio 9 2008"
	extra="CMAKE_RC_COMPILER="
else
	if [ "$OSTYPE" == "darwin9.0" ]
	then
		thedir="Mac"
		maketype="Xcode"
	else
		thedir="Linux"
#		maketype="Unix Makefiles"
		maketype="CodeBlocks - Unix Makefiles"
	fi
fi

echo "Creating $maketype in $thedir. $extra"

rm -rf $thedir
mkdir $thedir
pushd $thedir
cmake ../ -G "$maketype"
popd

