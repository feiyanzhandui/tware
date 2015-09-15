#!/bin/sh

#
# Starting installation
#
printf "\n\n********** STARTING TUPLEWARE INSTALLATION **********\n\n"
cd ~
sudo apt-get update

#
# Dependencies
#
printf "\n\n********** INSTALLING BASE DEPENDENCIES **********\n\n"
cd ~
sudo apt-get install -y autoconf
sudo apt-get install -y automake
sudo apt-get install -y build-essential
sudo apt-get install -y cmake
sudo apt-get install -y curl
sudo apt-get install -y fabric
sudo apt-get install -y libboost-dev
sudo apt-get install -y libboost-filesystem-dev
sudo apt-get install -y libboost-iostreams-dev
sudo apt-get install -y libboost-program-options-dev
sudo apt-get install -y libboost-system-dev
sudo apt-get install -y libtool
sudo apt-get install -y make
sudo apt-get install -y maven
sudo apt-get install -y pkg-config
sudo apt-get install -y python-boto
sudo apt-get install -y subversion

#
# infiniband
#
#sudo apt-get install -y libaio-dev
#sudo apt-get install -y libibverbs-dev
#sudo apt-get install -y libnuma-dev
#sudo apt-get install -y librdmacm-dev
#sudo apt-get install -y numactl

#sudo apt-get install -y ibverbs-utils
#sudo apt-get install -y rdmacm-utils
#sudo apt-get install -y infiniband-diags
#sudo apt-get install -y perftest

#
# g++
#
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo update-alternatives --remove-all gcc
sudo update-alternatives --remove-all g++
sudo apt-get update
sudo apt-get install -y g++-4.9
sudo apt-get install -y libgtest-dev

#
# Java
#
printf "\n\n********** INSTALLING JAVA **********\n\n"
cd ~
sudo apt-get install -y openjdk-7-jdk
JAVA_HOME="/usr/lib/jvm/java-1.7.0-openjdk-amd64"
echo "export JAVA_HOME=$JAVA_HOME" >> ~/.bashrc

#
# LLVM
#
printf "\n\n********** INSTALLING LLVM **********\n\n"
sudo add-apt-repository -s 'deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.6 main'
sudo apt-get update
wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key|sudo apt-key add -
sudo apt-get install clang-3.6 clang-3.6-doc libclang-common-3.6-dev libclang-3.6-dev libclang1-3.6 libclang1-3.6-dbg libllvm-3.6-ocaml-dev libllvm3.6 libllvm3.6-dbg lldb-3.6 llvm-3.6 llvm-3.6-dev llvm-3.6-doc llvm-3.6-examples llvm-3.6-runtime clang-modernize-3.6 clang-format-3.6 python-clang-3.6 lldb-3.6-dev

#
# numba
#
#cd ~
#git clone https://github.com/numba/llvmlite
#cd llvmlite
#python3 setup.py install
#cd ~
#sudo pip3 install numba
#pip install -r requirements.txt
#python setup.py build_ext --inplace
#python setup.py install

#
# libsodium
#
git clone git://github.com/jedisct1/libsodium.git
wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.3.tar.gz
cd libsodium
./autogen.sh
./configure
make check
sudo make install
sudo ldconfig
cd ~

#
# ZeroMQ
#
printf "\n\n********** INSTALLING ZEROMQ **********\n\n"
cd ~
wget http://download.zeromq.org/zeromq-4.1.2.tar.gz
tar -xf zeromq-4.1.2.tar.gz
cd zeromq-4.1.2
./configure
make check
sudo make install
cd ~
sudo rm -rf zeromq-4.1.2.tar.gz zeromq-4.1.2

#
# Accelio
#
#printf "\n\n********** INSTALLING ACCELIO **********\n\n"
#cd ~
#git clone http://github.com/accelio/accelio.git
#cd accelio
#./autogen.sh
#./configure
#make

#
# Cereal
#
printf "\n\n********** INSTALLING CEREAL **********\n\n"
cd ~
git clone http://github.com/USCiLab/cereal.git
sudo mv cereal/include/cereal /usr/local/include
sudo rm -rf cereal

#
# Hadoop/HDFS
#
printf "\n\n********** INSTALLING HADOOP/HDFS **********\n\n"
cd ~
wget https://s3.amazonaws.com/agg_hadoop/hadoop-2.4.0.tar.gz
tar -xf hadoop-2.4.0.tar.gz
sudo mv hadoop-2.4.0 /usr/local/hadoop
HADOOP_INSTALL="/usr/local/hadoop"
echo "export HADOOP_INSTALL=$HADOOP_INSTALL" >> ~/.bashrc
echo "export PATH=$PATH:$HADOOP_INSTALL/bin:$HADOOP_INSTALL/sbin" >> ~/.bashrc
echo "export HADOOP_MAPRED_HOME=$HADOOP_INSTALL" >> ~/.bashrc
echo "export HADOOP_COMMON_HOME=$HADOOP_INSTALL" >> ~/.bashrc
echo "export HADOOP_HDFS_HOME=$HADOOP_INSTALL" >> ~/.bashrc
echo "export YARN_HOME=$HADOOP_INSTALL" >> ~/.bashrc
source ~/.bashrc
echo "export JAVA_HOME=$JAVA_HOME" >> "$HADOOP_INSTALL/etc/hadoop/hadoop-env.sh"
echo "Host *" >> ~/.ssh/config
echo "    StrictHostKeyChecking no" >> ~/.ssh/config
sudo rm -rf hadoop-2.4.0.tar.gz hadoop-2.4.0-src.tar.gz

#
# Google Test
#
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp *.a /usr/lib

#
# Configuring environment
#
printf "\n\n********** CONFIGURING ENVIRONMENT **********\n\n"
cd ~
sudo ldconfig
TUPLEWARE_HOME="$HOME/tupleware"
if [ -d $TUPLEWARE_HOME ]; then
    echo "export TUPLEWARE_HOME=$TUPLEWARE_HOME" >> ~/.bashrc
    source ~/.bashrc
    cd $TUPLEWARE_HOME
    mkdir build
else
    echo "*** WARNING: Tupleware was not installed in $TUPLEWARE_HOME ***"
    echo "Please do the following manually:"
    echo "  1) set TUPLEWARE_HOME to the correct directory"
    echo "  2) create the directory $TUPLEWARE_HOME/build"
fi

#
# Installation complete
#
printf "\n\n********** TUPLEWARE INSTALLATION COMPLETE **********\n\n"
