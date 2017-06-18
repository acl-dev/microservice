#!/bin/bash

#make build directory to build project
mkdir build
cd build
cmake .. -DACL_ROOT=/home/akzi/code/acl-dev/acl
make
