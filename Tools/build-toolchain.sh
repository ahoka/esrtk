#!/bin/sh

set -e

GCC_VERSION=5.1.0
BINUTILS_VERSION=2.25
MPFR_VERSION=3.1.2
GMP_VERSION=6.0.0
MPC_VERSION=1.0.3
ICONV_VERSION=1.14

TARGET=i686-elf
PREFIX=/opt/${TARGET}

ROOTDIR=${PWD}

CORES=$(nproc)
if [ -z "$CORES" ]
then
    CORES=1
fi

export PATH=${PREFIX}/bin:${PATH}

download()
{
    if [ ! -e binutils-${BINUTILS_VERSION}.tar.bz2 ]
    then
        wget https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.bz2
    fi

    if [ ! -e gcc-${GCC_VERSION}.tar.bz2 ]
    then
#	wget ftp://gcc.gnu.org/pub/gcc/snapshots/LATEST-5/gcc-${GCC_VERSION}.tar.bz2
        wget https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.bz2
    fi

    if [ ! -e mpfr-${MPFR_VERSION}.tar.bz2 ]
    then
        wget http://www.mpfr.org/mpfr-current/mpfr-${MPFR_VERSION}.tar.bz2
    fi

    if [ ! -e gmp-${GMP_VERSION}a.tar.bz2 ]
    then
        wget https://gmplib.org/download/gmp/gmp-${GMP_VERSION}a.tar.bz2
    fi

    if [ ! -e mpc-${MPC_VERSION}.tar.gz ]
    then
        wget https://ftp.gnu.org/gnu/mpc/mpc-${MPC_VERSION}.tar.gz
    fi

    if [ ! -e libiconv-${ICONV_VERSION}.tar.gz ]
    then
        wget https://ftp.gnu.org/gnu/libiconv/libiconv-${ICONV_VERSION}.tar.gz
    fi
}

extract()
{
    if [ ! -e binutils-${BINUTILS_VERSION} ]
    then
        tar xjf binutils-${BINUTILS_VERSION}.tar.bz2
    fi

    if [ ! -e gcc-${GCC_VERSION} ]
    then
        tar xjf gcc-${GCC_VERSION}.tar.bz2
    fi

    if [ ! -e mpfr-${MPFR_VERSION} ]
    then
        tar xjf mpfr-${MPFR_VERSION}.tar.bz2
    fi

    if [ ! -e gmp-${GMP_VERSION} ]
    then
        tar xjf gmp-${GMP_VERSION}a.tar.bz2
    fi

    if [ ! -e mpc-${MPC_VERSION} ]
    then
        tar xzf mpc-${MPC_VERSION}.tar.gz
    fi

    if [ ! -e libiconv-${ICONV_VERSION} ]
    then
        tar xzf libiconv-${ICONV_VERSION}.tar.gz
    fi

    ln -fs ${ROOTDIR}/mpfr-${MPFR_VERSION} gcc-${GCC_VERSION}/mpfr
    ln -fs ${ROOTDIR}/gmp-${GMP_VERSION} gcc-${GCC_VERSION}/gmp
    ln -fs ${ROOTDIR}/mpc-${MPC_VERSION} gcc-${GCC_VERSION}/mpc
    ln -fs ${ROOTDIR}/libiconv-${ICONV_VERSION} gcc-${GCC_VERSION}/libiconv
}

build_binutils()
{
    rm -rf ${ROOTDIR}/binutils

    mkdir ${ROOTDIR}/binutils
    cd ${ROOTDIR}/binutils
    ${ROOTDIR}/binutils-${BINUTILS_VERSION}/configure --prefix=${PREFIX} --target=${TARGET} --with-sysroot --disable-nls --disable-werror
    make -j "$CORES"
}

build_gcc()
{
    rm -rf ${ROOTDIR}/gcc

    mkdir ${ROOTDIR}/gcc
    cd ${ROOTDIR}/gcc
    ${ROOTDIR}/gcc-${GCC_VERSION}/configure --prefix=${PREFIX} --target=${TARGET} --disable-nls --enable-languages=c,c++ --without-headers --disable-werror
    make -j "$CORES" all-gcc
    make -j "$CORES" all-target-libgcc
}

install_binutils()
{
    cd ${ROOTDIR}/binutils
    sudo make install
}

install_gcc()
{
    cd ${ROOTDIR}/gcc
    sudo make install-gcc
    sudo make install-target-libgcc
}

download
extract

build_binutils
install_binutils

build_gcc
install_gcc
