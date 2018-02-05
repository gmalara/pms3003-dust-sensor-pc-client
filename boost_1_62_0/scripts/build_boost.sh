#!/bin/bash

WORKING_DIR=$1

export PATH=$PATH:$WORKING_DIR/Boost/Boost.Build/bin

echo "Changing chmod"
chmod 744 $WORKING_DIR/Boost/tools/build/bootstrap.sh $WORKING_DIR/Boost/tools/build/src/engine/build.sh $WORKING_DIR/Boost/tools/build/b2

echo "Entering $WORKING_DIR/Boost/tools/build/ folder"
cd $WORKING_DIR/Boost/tools/build/

echo "Executing bootstrap script"
./bootstrap.sh

echo "Installing Boost.Build with b2 tool"
./b2 install --prefix=$WORKING_DIR/Boost/Boost.Build

echo "Entering $WORKING_DIR/Boost/ folder"
cd $WORKING_DIR/Boost/

echo "Compiling x64Linux version"
./tools/build/b2 toolset=gcc --address-model=64 -sBZIP2_SOURCE=$WORKING_DIR/bzip2 -sZLIB_SOURCE=$WORKING_DIR/zlib --build-type=minimal --build-dir=./Build/x64Linux --stagedir=$WORKING_DIR/Boost/stage -j4 stage
echo "Renaming $WORKING_DIR/Boost/stage -> $WORKING_DIR/Boost/x64Linux"
mv $WORKING_DIR/Boost/stage $WORKING_DIR/Boost/x64Linux
echo "Removing $WORKING_DIR/Boost/Build"
rm -r $WORKING_DIR/Boost/Build

echo "Compiling arm7aLinux version"
cp $WORKING_DIR/Boost/scripts/user-config.jam $HOME/user-config.jam

if [ -d "$HOME/gcc-linaro-4.9-2016.02-x86_64_arm-linux-gnueabihf/bin/" ]; then
  ./tools/build/b2 toolset=gcc-arm --address-model=64 -sBZIP2_SOURCE=$WORKING_DIR/bzip2 -sZLIB_SOURCE=$WORKING_DIR/zlib --build-type=minimal --build-dir=./Build/arm7aLinux --stagedir=$WORKING_DIR/Boost/stage -j4 stage
else
  tar xvfJ $WORKING_DIR/Toolchain/gcc-linaro-4.9-2016.02-x86_64_arm-linux-gnueabihf.tar.xz --directory=$HOME
  ./tools/build/b2 toolset=gcc-arm --address-model=64 -sBZIP2_SOURCE=$WORKING_DIR/bzip2 -sZLIB_SOURCE=$WORKING_DIR/zlib --build-type=minimal --build-dir=./Build/arm7aLinux --stagedir=$WORKING_DIR/Boost/stage -j4 stage
fi

echo "Renaming $WORKING_DIR/Boost/stage -> $WORKING_DIR/Boost/armv7aLinux"
mv $WORKING_DIR/Boost/stage $WORKING_DIR/Boost/armv7aLinux

#echo "Compiling x64Win version"
#./tools/build/b2 toolset=gcc-mingw32 --host-os=linux --target-os=windows --variant=release --address-model=64 --architecture=x86 -sBZIP2_SOURCE=$WORKING_DIR/bzip2 -sZLIB_SOURCE=$WORKING_DIR/zlib --build-type=minimal --build-dir=./Build/win64 --stagedir=$WORKING_DIR/Boost/stage -j4 stage
#echo "Renaming $WORKING_DIR/Boost/stage -> $WORKING_DIR/Boost/x64Win"
#mv $WORKING_DIR/Boost/stage $WORKING_DIR/Boost/x64Win

rm -r $WORKING_DIR/Boost/Build/*
mv $WORKING_DIR/Boost/x64Linux $WORKING_DIR/Boost/Build/
mv $WORKING_DIR/Boost/armv7aLinux $WORKING_DIR/Boost/Build/
rm $HOME/user-config.jam
