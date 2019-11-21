#!/bin/bash
rm -rf build
DIR=`pwd`
mkdir build
cd build
cmake ..
make -j4
