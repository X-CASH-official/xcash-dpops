#!/bin/bash

# Configuration settings
INSTALLATION_DIRECTORY="$HOME/xcash-official/"
PROGRAMS_INSTALLATION_DIRECTORY="${INSTALLATION_DIRECTORY}Installed-Programs/"

# Latest versions
XCASH_URL="https://github.com/X-CASH-official/xcash-core.git"
BOOST_URL="https://dl.bintray.com/boostorg/release/1.71.0/source/boost_1_71_0.tar.gz"
OPENSSL_URL="https://www.openssl.org/source/openssl-1.1.1d.tar.gz"
PCSC_LITE_URL="https://pcsclite.apdu.fr/files/pcsc-lite-1.8.25.tar.bz2"
ZEROMQ_URL="https://github.com/zeromq/libzmq/releases/download/v4.3.2/zeromq-4.3.2.tar.gz"

# Settings
XCASH_DIRECTORY="${PROGRAMS_INSTALLATION_DIRECTORY}xcash-core/"
BOOST_DIRECTORY="${PROGRAMS_INSTALLATION_DIRECTORY}boost_build/"
OPENSSL_DIRECTORY="${PROGRAMS_INSTALLATION_DIRECTORY}openssl_build/"
PCSC_LITE_DIRECTORY="${PROGRAMS_INSTALLATION_DIRECTORY}pcsc_lite_build/"

# Log Files
LOGFILE_DIRECTORY="${INSTALLATION_DIRECTORY}build/"
LOGFILE="${LOGFILE_DIRECTORY}STEP_10_XCASH_BUILD_STATIC.txt"

# System settings
CPU_THREADS=$(nproc)

# Functions
function reset_directories()
{
  rm -r "${PROGRAMS_INSTALLATION_DIRECTORY}"
  rm -r "${LOGFILE_DIRECTORY}"
  mkdir "${PROGRAMS_INSTALLATION_DIRECTORY}" || return 1
  mkdir "${LOGFILE_DIRECTORY}" || return 1
}



function update_packages_list()
{
    i=0
    while fuser /var/{lib/{dpkg,apt/lists},cache/apt/archives}/lock >/dev/null 2>&1; do
        case $((i % 4)) in
            0 ) j="-" ;;
            1 ) j="\\" ;;
            2 ) j="|" ;;
            3 ) j="/" ;;
        esac
        sleep 0.25
        ((i=i+1))
    done
    echo -e "Updating Packages List\n" &>"${LOGFILE}" || return 1
    sudo apt update -y &>"${LOGFILE}" || return 1
    echo -e "\n" &>"${LOGFILE}" || return 1
}



function update_packages()
{
    i=0
    while fuser /var/{lib/{dpkg,apt/lists},cache/apt/archives}/lock >/dev/null 2>&1; do
        case $((i % 4)) in
            0 ) j="-" ;;
            1 ) j="\\" ;;
            2 ) j="|" ;;
            3 ) j="/" ;;
        esac
        sleep 0.25
        ((i=i+1))
    done
    echo -e "Updating Packages\n" &>"${LOGFILE}" || return 1
    sudo apt dist-upgrade -y &>"${LOGFILE}" || return 1
    echo -e "\n" &>"${LOGFILE}" || return 1
}  



function build_boost()
{
  echo -e "Building Boost\n" &>"${LOGFILE}" || return 1
  cd "${PROGRAMS_INSTALLATION_DIRECTORY}" || return 1
  wget -q ${BOOST_URL} &>"${LOGFILE}" || return 1
  tar -xf boost*.tar.gz &>"${LOGFILE}" || return 1
  rm boost*.tar.gz || return 1
  cd boost* || return 1
  mkdir "${BOOST_DIRECTORY}" || return 1
  ./bootstrap.sh --prefix="${BOOST_DIRECTORY}" &>"${LOGFILE}" || return 1
  sudo ./b2 cxxflags=-fPIC cflags=-fPIC -a install -j "${CPU_THREADS}" &>"${LOGFILE}"
  echo -e "\n" &>"${LOGFILE}" || return 1
}



function build_openssl()
{
  echo -e "Building OpenSSL\n" &>"${LOGFILE}" || return 1
  cd "${PROGRAMS_INSTALLATION_DIRECTORY}" || return 1
  wget -q ${OPENSSL_URL} &>"${LOGFILE}" || return 1
  tar -xf openssl*.tar.gz &>"${LOGFILE}" || return 1
  rm openssl*.tar.gz || return 1
  cd openssl* || return 1
  mkdir "${OPENSSL_DIRECTORY}" || return 1
  ./config -fPIC --prefix="${OPENSSL_DIRECTORY}" --openssldir="${OPENSSL_DIRECTORY}" &>"${LOGFILE}" || return 1
  make depend &>"${LOGFILE}" || return 1
  make -j "${CPU_THREADS}" &>"${LOGFILE}" || return 1
  sudo make install &>"${LOGFILE}" || return 1
  echo -e "\n" &>"${LOGFILE}" || return 1
}



function build_pcsc_lite()
{
  echo -e "Building PCSC Lite\n" &>"${LOGFILE}" || return 1
  cd "${PROGRAMS_INSTALLATION_DIRECTORY}" || return 1
  wget -q ${PCSC_LITE_URL} &>"${LOGFILE}" || return 1
  tar -xf pcsc*.tar.bz2 &>"${LOGFILE}" || return 1
  rm pcsc*.tar.bz2 || return 1
  cd pcsc* || return 1
  mkdir "${PCSC_LITE_DIRECTORY}" || return 1
  ./configure CPPFLAGS=-DPIC CFLAGS=-fPIC CXXFLAGS=-fPIC LDFLAGS=-fPIC --enable-static --prefix="${PCSC_LITE_DIRECTORY}" &>"${LOGFILE}" || return 1
  make -j "${CPU_THREADS}" &>"${LOGFILE}" || return 1
  sudo make install &>"${LOGFILE}" || return 1
  echo -e "\n" &>"${LOGFILE}" || return 1
}



function build_zeromq()
{
  echo -e "Building ZeroMQ\n" &>"${LOGFILE}" || return 1
  cd "${PROGRAMS_INSTALLATION_DIRECTORY}" || return 1
  wget -q ${ZEROMQ_URL} &>"${LOGFILE}" || return 1
  tar -xf zeromq*.tar.gz &>"${LOGFILE}" || return 1
  rm zeromq*.tar.gz || return 1
  cd zeromq* || return 1
  ./autogen.sh &>"${LOGFILE}" || return 1
  ./configure CPPFLAGS=-DPIC CFLAGS=-fPIC CXXFLAGS=-fPIC LDFLAGS=-fPIC &>"${LOGFILE}" || return 1
  make -j "${CPU_THREADS}" &>"${LOGFILE}" || return 1
  sudo make install &>"${LOGFILE}" || return 1
  sudo ldconfig
  cd /usr/local/include/ || return 1
  rm zmq.hpp
  wget -q https://raw.githubusercontent.com/zeromq/cppzmq/master/zmq.hpp &>"${LOGFILE}" || return 1
  echo -e "\n" &>"${LOGFILE}" || return 1
}



function build_programs()
{
  build_boost || return 1
  build_openssl || return 1
  build_pcsc_lite || return 1
  build_zeromq || return 1
}



function build_xcash()
{
  echo -e "Building X-CASH\n" &>"${LOGFILE}" || return 1
  cd "${PROGRAMS_INSTALLATION_DIRECTORY}" || return 1
  git clone ${XCASH_URL} &>"${LOGFILE}" || return 1
  cd "${XCASH_DIRECTORY}" || return 1
  rm -r build
  mkdir -p build/release || return 1
  cd build/release || return 1
  cmake -D STATIC=ON -D ARCH="x86-64" -D BUILD_64=ON -D BUILD_TESTS=ON -D BOOST_ROOT="${BOOST_DIRECTORY}" -D OPENSSL_ROOT_DIR="${OPENSSL_DIRECTORY}" -D USE_READLINE=OFF -D CMAKE_BUILD_TYPE=release ../.. &>"${LOGFILE}" || return 1
  cd ../../
  make -I"${BOOST_DIRECTORY}"include -I"${OPENSSL_DIRECTORY}"include -I"${PCSC_LITE_DIRECTORY}"include LDFLAGS="-L${BOOST_DIRECTORY}lib -L${OPENSSL_DIRECTORY}lib -L${PCSC_LITE_DIRECTORY}lib" -j "${CPU_THREADS}" &>"${LOGFILE}" || return 1
  echo -e "\n" &>"${LOGFILE}" || return 1
}



function main()
{
  # Reset the directories
  reset_directories || return 1

  # Update the packages list
  update_packages_list || return 1

  # Update the packages
  update_packages || return 1

  # Build programs
  build_programs || return 1

  # Build XCASH
  build_xcash || return 1
}



main || echo "STEP_10_XCASH_BUILD_STATIC_ERROR" &>"${LOGFILE}"