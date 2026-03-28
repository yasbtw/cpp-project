#!/bin/bash 


mkdir -p build # create dir if doesn't exist
cd build
cmake ..
make -j$(nproc)

# alright run this
# how do i run shell files?
# yup did it ohhh alright 1sec alright did it
#worksss it said hello world
# bet bet
# git clone --depth 1 https://github.com/glfw/glfw thirdparty/glfw
# try building again ./build.sh
# ooooo interesting alright
# you just built glfw from source, lets use it
#yas@cachyos-x8664 ~/D/t/cpp-project> ls thirdparty
#drwxr-xr-x - yas 22 مار 19:17  glfw