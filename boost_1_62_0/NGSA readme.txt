This boost distribution was compiled under Ubuntu 14.10 for x64Linux and armv7aLinux.
x64Win version was compiled under Windows with msvc 14.0
This distribution contains boost-process extention (see below)
****************************************************************

------------------------
x64Linux and armv7aLinux
------------------------

Compilation steps needed under Ubuntu:
1. Download boost_1_62_0.tar.bz2 from http://www.boost.org/users/history/version_1_62_0.html
Download zlib from http://zlib.net/zlib-1.2.8.tar.gz
Download bzip fro  http://www.bzip.org/1.0.6/bzip2-1.0.6.tar.gz 

2. Execute to extract:
tar --bzip2 -xf /boost_1_62_0.tar.bz2

3. Go to the directory boost_1_62_0/tools/build/
Run bootstrap.sh
Run b2 install --prefix=/home/username/Build.Boost
export PATH=$PATH:/home/username/Boost.Build/bin

3. Go to the directory boost_1_62_0

4. Compile Linux PC version
b2 toolset=gcc address-model=64 -sBZIP2_SOURCE=/home/adrian/workspace/boost/bzip2-1.0.6 -sZLIB_SOURCE=/home/adrian/workspace/boost/zlib-1.2.8 --build-type=minimal --build-dir=./Build/x64Linux  stage

5. Rename stage folder to x64Linux 
6. Remove Build folder

7. Copy user-config.jam to your home directory
Edit user-config.jam and add cross compilation toolchain
using gcc : arm : /home/username/workspace/Icarus/Toolchain/target/NUM02/Linux/gcc-linaro-4.9-2016.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++ ;

8. Compile arm version
b2 toolset=gcc-arm address-model=64 -sBZIP2_SOURCE=/home/adrian/workspace/boost/bzip2-1.0.6 -sZLIB_SOURCE=/home/adrian/workspace/boost/zlib-1.2.8 --build-type=minimal --build-dir=./Build/arm7aLinux stage

9. Rename stage folder to armv7aLinux
10. Clean content of Build folder
11. Move x64Linux and armv7aLinux to Build folder

------------------------
x64Win
------------------------

1. Go to the directory boost_1_62_0\tools\build\
Run bootstrap.bat
Run b2 install --prefix=C:\Boost.Build to be installed
Add C:\Boost.Build\bin to your PATH environment variable.

2. Go to the directory boost_1_62_0

3. Compile x64Win version
\boost_1_62_0>b2 --toolset=msvc-14.0 address-model=64 -sBZIP2_SOURCE=C:\Users\chadkos\Downloads\boost_1_62_0.tar\bzip2-1.0.6 -sZLIB_SOURCE=C:\Users\chadkos\Downloads\boost_1_62_0.tar\zlib-1.2.8 --build-dir="D:\workspace\x64Win" --build-type=minimal msvc stage

4. Rename stage folder to x64Win

****************************************************************

Boost Process extention to boost lib.

Boost.Process library is not part of the standard distro. To use, make sure first to do the following:
1. Get the boost-process lib from here: 
https://github.com/BorisSchaeling/boost-process
2. Copy contents of boost folder to boost_1_62_0/boost, copy contents of libs folder to boost_1_62_0/libs




