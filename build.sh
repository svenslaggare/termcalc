#!/bin/bash

if [ "$#" -lt 1 ]; then
	BUILD_TYPE=Release
else
	BUILD_TYPE=$1
fi

mkdir -p Build/$BUILD_TYPE
cd Build/$BUILD_TYPE
cmake ../../ -DCMAKE_BUILD_TYPE=$BUILD_TYPE
make