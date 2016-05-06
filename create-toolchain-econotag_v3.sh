#!/bin/bash

###############################################################################
# Variables del script                                                        #
###############################################################################

# Directorio de instalación de la toolchain
#export PREFIX=/fenix/depar/atc/se/toolchain 
export PREFIX=/opt/econotag

# Plataforma de destino
export TARGET=arm-econotag-eabi

# Componentes de la toolchain
BINUTILS_VER="2.24"
GCC_VER="4.9.1"
NEWLIB_VER="2.1.0"
GDB_VER="7.8"
OPENOCD_VER="0.8.0"

BINUTILS="binutils-$BINUTILS_VER"
GCC="gcc-$GCC_VER"
NEWLIB="newlib-$NEWLIB_VER"
GDB="gdb-$GDB_VER"
OPENOCD="openocd-$OPENOCD_VER"

# Directorio para almacenar los fuentes comprimidos
export TARBALLS_DIR=${PREFIX}/downloads

# Directorio para construir las herramientas
export BUILD_DIR=${PREFIX}/build

# Directorios de las fuentes
BINUTILS_DIR="$BUILD_DIR/$BINUTILS"
GCC_DIR="$BUILD_DIR/$GCC"
NEWLIB_DIR="$BUILD_DIR/$NEWLIB"
GDB_DIR="$BUILD_DIR/$GDB"
OPENOCD_DIR="$BUILD_DIR/$OPENOCD"

# Directorios para la construcción
BINUTILS_BUILD_DIR="$BUILD_DIR/binutils-build"
GCC_BUILD_DIR="$BUILD_DIR/gcc-build"
NEWLIB_BUILD_DIR="$BUILD_DIR/newlib-build"
GDB_BUILD_DIR="$BUILD_DIR/gdb-build"

# Opciones de construcción de la toolchain
CROSS_FLAGS="--target=$TARGET --prefix=$PREFIX --enable-interwork --enable-multilib --with-float=soft --disable-werror"

# Plataforma de instalación
platform="unknown"
package_manager="unknown"


###############################################################################
# Detección de la plataforma                                                  #
###############################################################################

detect_platform()
{
  case `uname -s` in
  Linux)
    platform="linux"
    if [ $(which apt-get 2> /dev/null | wc -c) -gt 0 ]
    then
      package_manager="apt-get"
      return 0
    fi

    if [ $(which yum 2> /dev/null | wc -c) -gt 0 ]
    then
      package_manager="yum"
      return 0
    fi

    if [ $(which pacman 2> /dev/null | wc -c) -gt 0 ]
    then
      package_manager="pacman"
      return 0
    fi

    echo ""
    echo "Gestor de paquetes desconocido"
    echo "No se ha detectado ni apt-get, ni yum, ni pacman"
    echo ""
    exit 1
    ;;
  Darwin)
    platform="osx"

    if [ $(which gcc 2> /dev/null | wc -c) -eq 0 ]
    then
      echo ""
      echo "Instale Xcode (App Store) y vuelva a ejecutar este script"
      echo ""
      exit 1
    fi

    if [ $(which Xquartz 2> /dev/null | wc -c) -eq 0 ]
    then
      echo ""
      echo "Instale XQuartz (http://xquartz.macosforge.org/) y vuelva a ejecutar este script"
      echo ""
      exit 1
    fi

    if [ $(which port 2> /dev/null | wc -c) -gt 0 ]
    then
      package_manager="port"
      return 0
    fi

    echo ""
    echo "Instale MacPorts (http://www.macports.org/) y vuelva a ejecutar este script"
    echo ""
    exit 1
    ;;
  CYGWIN*)
    platform="cygwin"
    if [ $(which apt-cyg 2> /dev/null | wc -c) -gt 0 ]
    then
      package_manager="apt-cyg"
      return 0
    fi

    echo ""
    echo "Instale apt-cyg (http://code.google.com/p/apt-cyg/) y vuelva a ejecutar este script"
    echo ""
    exit 1
    ;;
  *)
    echo ""
    echo "Plataforma de ejecución desconocida"
    echo "Actualmente sólo están soportadas: Linux y Mac OSX"
    echo ""

    exit 1
    ;;
  esac
}


###############################################################################
# Comprobación de root                                                        #
###############################################################################

check_root()
{
  if [ ${platform} == "cygwin" ]
  then
    return 0;
  fi

  if [ $(whoami) != "root" ]
  then
    echo ""
    echo "Es necesario ser root para ejecutar este script"
    echo ""
    exit 1
  fi

  return 0
}


###############################################################################
# Descarga un fichero si no ha sido previemente descargado                    #
###############################################################################

download_tarball()
{
  if [ ! -f ${TARBALLS_DIR}/$2 ]
  then
    wget --tries=5 -P ${TARBALLS_DIR} $1/$2
  else
    echo $2 ya está en ${TARBALLS_DIR}
  fi

  return 0
}


###############################################################################
# Prerrequisitos para distribuciones basadas en debian                        #
###############################################################################

install_prerrequisites_debian()
{
  # Para descargar las fuentes
  apt-get -y install wget

  # Para la toolchain
  apt-get -y install build-essential texinfo bison flex
  apt-get -y install libncurses5-dev libgmp3-dev libmpfr-dev libmpc-dev zlib1g-dev

  # Para openocd
  apt-get -y install libtool autoconf texinfo libftdi-dev libusb-1.0-0-dev

  # Para Eclipse
  apt-get -y install default-jre

  # Para los Makefiles de las prácticas
  apt-get -y install xterm putty

  return 0
}


###############################################################################
# Prerrequisitos para distribuciones basadas en fedora                        #
###############################################################################

install_prerrequisites_fedora()
{
  # Para descargar las fuentes
  yum -y install wget

  # Para la toolchain
  yum -y install texinfo bison flex
  yum -y install ncurses-devel gmp-devel mpfr-devel libmpc-devel zlib-devel

  # Para openocd
  yum -y install libtool autoconf texinfo libusb-devel libftdi-devel

  # Para Eclipse
  yum -y install java-1.8.0-openjdk

  # Para los Makefiles de las prácticas
  yum -y install xterm putty

  return 0
}

###############################################################################
# Prerrequisitos para distribuciones basadas en ArchLinux                     #
###############################################################################

install_prerrequisites_archlinux()
{
  # Para descargar las fuentes
  pacman -S --noconfirm wget

  # Para la toolchain
  pacman -S --noconfirm texinfo bison flex
  pacman -S --noconfirm ncurses gmp mpfr libmpc zlib

  # Para openocd
  pacman -S --noconfirm libtool autoconf texinfo libusb libftdi

  # Para Eclipse
  pacman -S --noconfirm jdk8-openjdk

  # Para los Makefiles de las prácticas
  pacman -S --noconfirm xterm putty

  return 0
}


###############################################################################
# Prerrequisitos para Mac OSX                                                 #
###############################################################################

install_prerrequisites_osx()
{
  # Dependencias para descargar las fuentes
  port selfupdate
  port install wget

  # Para la toolchain
  port install texinfo bison flex gmp mpfr libmpc zlib

  # Para openocd
  port install libtool autoconf texinfo libusb libftdi0

  # Indicamos dónde están las cabeceras y las bibliotecas
  export LDFLAGS="-L/opt/local/lib"
  export CPPFLAGS="-I/opt/local/include"
  export LD_LIBRARY_PATH="/opt/local/lib"
  export LD_INCLUDE_PATH="/opt/local/include"

  return 0
}


###############################################################################
# Prerrequisitos para cygwin                                                  #
###############################################################################

install_prerrequisites_cygwin()
{
  # Para descargar las fuentes
  apt-cyg install subversion wget tar gawk bzip2

  # Para la toolchain
  apt-cyg install binutils gcc4 make autoconf automake texinfo bison flex
  apt-cyg install libiconv libncurses-devel libgmp-devel libmpfr-devel libmpc-devel zlib-devel

  # Para openocd
  apt-cyg install libtool autoconf texinfo libusb-win32 python swig

  # OpenOCD necesita libftdi, que no está como paquete en cygwin, así que tenemos que instalarlo desde las fuentes
  LIBFTDI_VER="0.20"
  LIBFTDI="libftdi-$LIBFTDI_VER"
  download_tarball http://www.intra2net.com/en/developer/libftdi/download ${LIBFTDI}.tar.gz

  mkdir -p ${BUILD_DIR}

  # Descomprimimos las fuentes
  cd ${BUILD_DIR}
  tar zxvf ${TARBALLS_DIR}/${LIBFTDI}.tar.gz

  # Lo instalamos
  cd ${LIBFTDI}
  ./configure  --prefix="/usr"
  make all install

  return 0
}


###############################################################################
# Instalación de los prerrequisitos                                           #
###############################################################################

install_prerrequisites()
{
  case ${package_manager} in
  apt-get)
    install_prerrequisites_debian
    return 0
    ;;
  yum)
    install_prerrequisites_fedora
    return 0
    ;;
  pacman)
    install_prerrequisites_archlinux
    return 0
    ;;
  port)
    install_prerrequisites_osx
    return 0
    ;;
  apt-cyg)
    install_prerrequisites_cygwin
    return 0
    ;;
  *)
    echo ""
    echo "No se pueden instalar los prerrequisitos. Gestor de paquetes desconocido"
    echo ""
    exit 1
    ;;
  esac
}


###############################################################################
# Obtenemos los fuentes de la toolchain                                       #
###############################################################################

download_sources()
{
  download_tarball http://ftp.gnu.org/gnu/binutils           ${BINUTILS}.tar.bz2
  download_tarball http://ftp.gnu.org/gnu/gcc/${GCC}         ${GCC}.tar.bz2
  download_tarball ftp://sources.redhat.com/pub/newlib       ${NEWLIB}.tar.gz
  download_tarball http://ftp.gnu.org/gnu/gdb                ${GDB}.tar.gz
  download_tarball http://sourceforge.net/projects/openocd/files/openocd/$OPENOCD_VER $OPENOCD.tar.bz2

  mkdir -p ${BUILD_DIR}

  # Descomprimimos las fuentes
  cd ${BUILD_DIR}
  tar jxvf ${TARBALLS_DIR}/${BINUTILS}.tar.bz2
  tar jxvf ${TARBALLS_DIR}/${GCC}.tar.bz2
  tar zxvf ${TARBALLS_DIR}/${NEWLIB}.tar.gz
  tar zxvf ${TARBALLS_DIR}/${GDB}.tar.gz
  tar jxvf ${TARBALLS_DIR}/${OPENOCD}.tar.bz2

  return 0
}


###############################################################################
# Construcción                                                                #
###############################################################################

build_toolchain()
{
  # Construimos las binutils
  mkdir -p $BINUTILS_BUILD_DIR
  cd $BINUTILS_BUILD_DIR
  $BINUTILS_DIR/configure $CROSS_FLAGS
  make all install
  export PATH="$PATH:$PREFIX/bin"

  # Primera etapa del compilador
  # En máquina de 32 bits falla al construir zlib, así que usamos la del host
  mkdir -p $GCC_BUILD_DIR
  cd $GCC_BUILD_DIR
  $GCC_DIR/configure $CROSS_FLAGS --with-system-zlib --enable-languages="c,c++" --with-newlib --with-headers=$NEWLIB_DIR/newlib/libc/include
  make all-gcc install-gcc

  # Costrucción de newlib sin soporte para llamadas al sistema
  mkdir -p $NEWLIB_BUILD_DIR
  cd $NEWLIB_BUILD_DIR
  $NEWLIB_DIR/configure $CROSS_FLAGS --disable-newlib-supplied-syscalls
  make all install

  # Segunda etapa del compilador
  cd $GCC_BUILD_DIR
  make all install

  # Construcción del debugger
  mkdir -p $GDB_BUILD_DIR
  cd $GDB_BUILD_DIR
  $GDB_DIR/configure $CROSS_FLAGS
  make all install

  # Construcción de openocd
  cd $OPENOCD_DIR
  ./configure --prefix=$PREFIX --disable-werror --enable-libftdi
  make all install

  return 0
}


###############################################################################
# Ejecución del script                                                        #
###############################################################################

detect_platform                # Detectamos la plataforma
check_root                     # Es necesario ser root para instalar los paquetes
install_prerrequisites         # Instalamos lor prerrequisitos en el sistema
download_sources               # Descargamos las fuentes de las herramientas
build_toolchain                # Construimos la toolchain
