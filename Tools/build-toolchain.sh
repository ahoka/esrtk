#!/bin/sh

set -e
set -x

GCC_VERSION=6.3.0
BINUTILS_VERSION=2.28
MPFR_VERSION=3.1.5
GMP_VERSION=6.1.2
MPC_VERSION=1.0.3
ICONV_VERSION=1.15
ISL_VERSION=0.18
CLOOG_VERSION=0.18.4
CDRTOOLS_VERSION=3.02a07

TARGET=i686-elf
PREFIX=$HOME/gcc/${TARGET}
ROOTDIR=${PWD}
DESTDIR=${ROOTDIR}/toolchain

CORES=$(nproc)
if [ -z "$CORES" ]
then
    CORES=1
fi

export PATH=${DESTDIR}/${PREFIX}/bin:${PATH}

download()
{
        wget -N https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.bz2
        wget -N https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.bz2
        wget -N http://www.mpfr.org/mpfr-current/mpfr-${MPFR_VERSION}.tar.bz2
        wget -N https://gmplib.org/download/gmp/gmp-${GMP_VERSION}.tar.bz2
        wget -N https://ftp.gnu.org/gnu/mpc/mpc-${MPC_VERSION}.tar.gz
        wget -N https://ftp.gnu.org/gnu/libiconv/libiconv-${ICONV_VERSION}.tar.gz
        wget -N http://isl.gforge.inria.fr/isl-${ISL_VERSION}.tar.bz2
        wget -N http://www.bastoul.net/cloog/pages/download/cloog-${CLOOG_VERSION}.tar.gz
        wget -N http://www.crufty.net/ftp/pub/sjg/bmake.tar.gz
        wget -N http://downloads.sourceforge.net/project/cdrtools/alpha/cdrtools-${CDRTOOLS_VERSION}.tar.bz2
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
        tar xjf gmp-${GMP_VERSION}.tar.bz2
    fi

    if [ ! -e mpc-${MPC_VERSION} ]
    then
        tar xzf mpc-${MPC_VERSION}.tar.gz
    fi

    if [ ! -e libiconv-${ICONV_VERSION} ]
    then
        tar xzf libiconv-${ICONV_VERSION}.tar.gz
    fi

    if [ ! -e isl-${ISL_VERSION} ]
    then
        tar xjf isl-${ISL_VERSION}.tar.bz2
    fi

    if [ ! -e cloog-${CLOOG_VERSION} ]
    then
        tar xzf cloog-${CLOOG_VERSION}.tar.gz
    fi

    if [ -e bmake.tar.gz ]
    then
        tar xvzf bmake.tar.gz
    fi

    if [ -e cdrtools-${CDRTOOLS_VERSION}.tar.bz2 ]
    then
        tar xvjf cdrtools-${CDRTOOLS_VERSION}.tar.bz2
    fi

    ln -fs ${ROOTDIR}/mpfr-${MPFR_VERSION} gcc-${GCC_VERSION}/mpfr
    ln -fs ${ROOTDIR}/gmp-${GMP_VERSION} gcc-${GCC_VERSION}/gmp
    ln -fs ${ROOTDIR}/mpc-${MPC_VERSION} gcc-${GCC_VERSION}/mpc
    ln -fs ${ROOTDIR}/libiconv-${ICONV_VERSION} gcc-${GCC_VERSION}/libiconv
    ln -fs ${ROOTDIR}/isl-${MPC_VERSION} gcc-${GCC_VERSION}/isl
    ln -fs ${ROOTDIR}/cloog-${CLOOG_VERSION} gcc-${GCC_VERSION}/cloog
}

configure_binutils()
{
    echo "====================================="
    echo "Configuring binutils..."
    echo "====================================="

    rm -rf ${ROOTDIR}/binutils

    mkdir ${ROOTDIR}/binutils
    cd ${ROOTDIR}/binutils
    ${ROOTDIR}/binutils-${BINUTILS_VERSION}/configure --prefix=${PREFIX} --target=${TARGET} --enable-lto --enable-plugins --with-sysroot --disable-nls --disable-werror
}

build_binutils()
{
    cd ${ROOTDIR}/binutils
    make -j "$CORES"
}

configure_gcc()
{
    echo "====================================="
    echo "Configuring gcc..."
    echo "====================================="

    rm -rf ${ROOTDIR}/gcc

    mkdir ${ROOTDIR}/gcc
    cd ${ROOTDIR}/gcc
    ${ROOTDIR}/gcc-${GCC_VERSION}/configure --prefix=${PREFIX} --target=${TARGET} --disable-nls --enable-languages=c,c++ --without-headers --disable-werror
}

build_gcc()
{
    cd ${ROOTDIR}/gcc
    make -j "$CORES" all-gcc
    make -j "$CORES" all-target-libgcc
}

install_binutils()
{
    cd ${ROOTDIR}/binutils
    make DESTDIR=${DESTDIR} install
}

install_gcc()
{
    cd ${ROOTDIR}/gcc
    make DESTDIR=${DESTDIR} install-gcc
    make DESTDIR=${DESTDIR} install-target-libgcc
}

configure_bmake()
{
    rm -rf ${ROOTDIR}/bmake-build
    mkdir ${ROOTDIR}/bmake-build

    cd ${ROOTDIR}/bmake-build
    ${ROOTDIR}/bmake/configure --prefix=${PREFIX}
}

build_bmake()
{
    cd ${ROOTDIR}/bmake-build
    make
}

install_bmake()
{
    cd ${ROOTDIR}/bmake-build
    make DESTDIR=${DESTDIR} install
}

build_mkisofs()
{
    cd ${ROOTDIR}/cdrtools-3.02
    make
}

install_mkisofs()
{
    cd ${ROOTDIR}/cdrtools-3.02/mkisofs
    install -m 0777 -v OBJ/*/mkisofs ${DESTDIR}${PREFIX}/bin/mkisofs
}

if [ $# -gt 0 ]
then
    case $1 in
        install )
            install_binutils         
            install_gcc
            install_bmake
            ;;
        build )
            build_binutils
            build_gcc
            build_bmake
            ;;
        download )
            download
            extract
            ;;
        configure )
            configure_binutils
            configure_gcc
            configure_bmake
            ;;
        bmake )
            download
            extract
            configure_bmake
            build_bmake
            install_bmake
            ;;
        mkisofs )
            build_mkisofs
            install_mkisofs
            ;;
    esac
else
    download
    extract
    
    configure_binutils
    build_binutils
    install_binutils

    configure_gcc
    build_gcc
    install_gcc

    configure_bmake
    build_bmake
    install_bmake

    build_mkisofs
    install_mkisofs
fi
