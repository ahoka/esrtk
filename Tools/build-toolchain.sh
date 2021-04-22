#!/bin/sh

set -e
set -x

GCC_VERSION=9.2.0
BINUTILS_VERSION=2.32
MPFR_VERSION=4.0.2
GMP_VERSION=6.1.2
MPC_VERSION=1.1.0
ICONV_VERSION=1.15
ISL_VERSION=0.18
CLOOG_VERSION=0.18.4
CDRTOOLS_VERSION=3.02a07

TARGET=i686-elf
PREFIX="$HOME/gcc/${TARGET}"
ROOTDIR="${PWD}"
BUILDROOT="${ROOTDIR}/buildroot"
DESTDIR="${ROOTDIR}/toolchain"

CORES=$(nproc)
if [ -z "$CORES" ]
then
    CORES=1
fi

TAR="tar -C ${BUILDROOT}"

export PATH="${DESTDIR}/${PREFIX}/bin:${PATH}"

download()
{
        wget -N https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.bz2
        wget -N https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.gz
        wget -N https://www.mpfr.org/mpfr-${MPFR_VERSION}/mpfr-${MPFR_VERSION}.tar.bz2
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
    mkdir -p ${BUILDROOT}
    
    if [ ! -e binutils-${BINUTILS_VERSION} ]
    then
        ${TAR} -xjf binutils-${BINUTILS_VERSION}.tar.bz2
    fi

    if [ ! -e gcc-${GCC_VERSION} ]
    then
        ${TAR} -xzf gcc-${GCC_VERSION}.tar.gz
    fi

    if [ ! -e mpfr-${MPFR_VERSION} ]
    then
        ${TAR} -xjf mpfr-${MPFR_VERSION}.tar.bz2
    fi

    if [ ! -e gmp-${GMP_VERSION} ]
    then
        ${TAR} -xjf gmp-${GMP_VERSION}.tar.bz2
    fi

    if [ ! -e mpc-${MPC_VERSION} ]
    then
        ${TAR} -xzf mpc-${MPC_VERSION}.tar.gz
    fi

    if [ ! -e libiconv-${ICONV_VERSION} ]
    then
        ${TAR} -xzf libiconv-${ICONV_VERSION}.tar.gz
    fi

    if [ ! -e isl-${ISL_VERSION} ]
    then
        ${TAR} -xjf isl-${ISL_VERSION}.tar.bz2
    fi

    if [ ! -e cloog-${CLOOG_VERSION} ]
    then
        ${TAR} -xzf cloog-${CLOOG_VERSION}.tar.gz
    fi

    if [ ! -e ${BUILDROOT}/bmake.tar.gz ]
    then
        ${TAR} -xzf bmake.tar.gz
    fi

    if [ ! -e ${BUILDROOT}/cdrtools-${CDRTOOLS_VERSION}.tar.bz2 ]
    then
        ${TAR} -xjf cdrtools-${CDRTOOLS_VERSION}.tar.bz2
    fi

    ln -fs ${BUILDROOT}/mpfr-${MPFR_VERSION} ${BUILDROOT}/gcc-${GCC_VERSION}/mpfr
    ln -fs ${BUILDROOT}/gmp-${GMP_VERSION} ${BUILDROOT}/gcc-${GCC_VERSION}/gmp
    ln -fs ${BUILDROOT}/mpc-${MPC_VERSION} ${BUILDROOT}/gcc-${GCC_VERSION}/mpc
    ln -fs ${BUILDROOT}/libiconv-${ICONV_VERSION} ${BUILDROOT}/gcc-${GCC_VERSION}/libiconv
    ln -fs ${BUILDROOT}/isl-${MPC_VERSION} ${BUILDROOT}/gcc-${GCC_VERSION}/isl
    ln -fs ${BUILDROOT}/cloog-${CLOOG_VERSION} ${BUILDROOT}/gcc-${GCC_VERSION}/cloog
}

configure_binutils()
{
    echo "====================================="
    echo "Configuring binutils..."
    echo "====================================="

    rm -rf ${BUILDROOT}/binutils

    mkdir ${BUILDROOT}/binutils
    cd ${BUILDROOT}/binutils
    ${BUILDROOT}/binutils-${BINUTILS_VERSION}/configure --prefix=${PREFIX} --target=${TARGET} --enable-lto --enable-plugins --with-sysroot --disable-nls --disable-werror
}

build_binutils()
{
    cd ${BUILDROOT}/binutils
    make -j "$CORES"
}

configure_gcc()
{
    echo "====================================="
    echo "Configuring gcc..."
    echo "====================================="

    rm -rf ${BUILDROOT}/gcc

    mkdir ${BUILDROOT}/gcc
    cd ${BUILDROOT}/gcc
    ${BUILDROOT}/gcc-${GCC_VERSION}/configure --prefix=${PREFIX} --target=${TARGET} --disable-nls --enable-languages=c,c++ --without-headers --disable-werror
}

build_gcc()
{
    cd ${BUILDROOT}/gcc
    make -j "$CORES" all-gcc
    make -j "$CORES" all-target-libgcc
}

install_binutils()
{
    cd ${BUILDROOT}/binutils
    make DESTDIR=${DESTDIR} install
}

install_gcc()
{
    cd ${BUILDROOT}/gcc
    make DESTDIR=${DESTDIR} install-gcc
    make DESTDIR=${DESTDIR} install-target-libgcc
}

configure_bmake()
{
    rm -rf ${BUILDROOT}/bmake-build
    mkdir ${BUILDROOT}/bmake-build

    cd ${BUILDROOT}/bmake-build
    ${BUILDROOT}/bmake/configure --prefix=${PREFIX}
}

build_bmake()
{
    cd ${BUILDROOT}/bmake-build
    make
}

install_bmake()
{
    cd ${BUILDROOT}/bmake-build
    make DESTDIR=${DESTDIR} install
}

build_mkisofs()
{
    cd ${BUILDROOT}/cdrtools-3.02
    make
}

install_mkisofs()
{
    cd ${BUILDROOT}/cdrtools-3.02/mkisofs
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
	extract )
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
