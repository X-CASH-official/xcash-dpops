#!/bin/bash

# Configuration settings
XCASH_DPOPS_INSTALLATION_DIR="$HOME/xcash-official/"
XCASH_BLOCKCHAIN_INSTALLATION_DIR="$HOME/.X-CASH/"
MONGODB_INSTALLATION_DIR="/data/db/"

# Current versions
MONGODB_CURRENT_VERSION=""
MONGOC_DRIVER_CURRENT_VERSION=""
NODEJS_CURRENT_VERSION=""
CPP_CHECK_CURRENT_VERSION=""

# Latest versions
MONGODB_LATEST_VERSION="mongodb-linux-x86_64-ubuntu1804-4.2.3"
MONGOC_DRIVER_LATEST_VERSION="mongo-c-driver-1.16.2"
NODEJS_LATEST_VERSION="node-v13.9.0-linux-x64"
CPP_CHECK_LATEST_VERSION="cppcheck-1.89"

XCASH_URL="https://github.com/X-CASH-official/xcash-core.git"
XCASH_DPOPS_URL="https://github.com/X-CASH-official/xcash-dpops.git"
DELEGATES_WEBSITE_URL="https://github.com/X-CASH-official/delegates-explorer.git"
SHARED_DELEGATES_WEBSITE_URL="https://github.com/X-CASH-official/delegates-pool-website.git"
NODEJS_URL="https://nodejs.org/dist/${NODEJS_LATEST_VERSION:5:7}/${NODEJS_LATEST_VERSION}.tar.xz"
MONGODB_URL="http://downloads.mongodb.org/linux/${MONGODB_LATEST_VERSION}.tgz"
MONGOC_DRIVER_URL="https://github.com/mongodb/mongo-c-driver/releases/download/${MONGOC_DRIVER_LATEST_VERSION:15}/${MONGOC_DRIVER_LATEST_VERSION}.tar.gz"
CPP_CHECK_URL="https://github.com/danmar/cppcheck/archive/${CPP_CHECK_LATEST_VERSION:9}.tar.gz"

# Settings
XCASH_DIR="${XCASH_DPOPS_INSTALLATION_DIR}xcash-core/"
XCASH_DPOPS_DIR="${XCASH_DPOPS_INSTALLATION_DIR}xcash-dpops/"
XCASH_DPOPS_DELEGATE_FOLDER_DIR="${XCASH_DPOPS_DIR}delegates-explorer/"
DELEGATES_WEBSITE_DIR="${XCASH_DPOPS_INSTALLATION_DIR}delegates-explorer/"
XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR="${XCASH_DPOPS_DIR}delegates-pool-website/"
SHARED_DELEGATES_WEBSITE_DIR="${XCASH_DPOPS_INSTALLATION_DIR}delegates-pool-website/"
NODEJS_DIR="${XCASH_DPOPS_INSTALLATION_DIR}${NODEJS_LATEST_VERSION}/"
MONGODB_DIR="${XCASH_DPOPS_INSTALLATION_DIR}${MONGODB_LATEST_VERSION}/"
CPP_CHECK_DIR="${XCASH_DPOPS_INSTALLATION_DIR}${CPP_CHECK_LATEST_VERSION}/"

# Log Files
LOGFILE_DIR="${XCASH_DPOPS_INSTALLATION_DIR}build/"
LOGFILE_STEP_1_PROGRAMS="${LOGFILE_DIR}STEP_1_PROGRAMS.txt"
LOGFILE_STEP_2_DELEGATES_WEBSITE="${LOGFILE_DIR}STEP_2_DELEGATES_WEBSITE.txt"
LOGFILE_STEP_3_SHARED_DELEGATES_WEBSITE="${LOGFILE_DIR}STEP_3_SHARED_DELEGATES_WEBSITE.txt"
LOGFILE_STEP_4_XCASH_BUILD_MAINNET="${LOGFILE_DIR}STEP_4_XCASH_BUILD_MAINNET.txt"
LOGFILE_STEP_5_XCASH_BLOCKCHAIN_SYNCING="${LOGFILE_DIR}STEP_5_XCASH_BLOCKCHAIN_SYNCING.txt"
LOGFILE_STEP_6_XCASH_BUILD_DEVELOPMENT="${LOGFILE_DIR}STEP_6_XCASH_BUILD_DEVELOPMENT.txt"
LOGFILE_STEP_7_XCASH_DPOPS_BUILD="${LOGFILE_DIR}STEP_7_XCASH_DPOPS_BUILD.txt"
LOGFILE_STEP_8_CPP_CHECK="${LOGFILE_DIR}STEP_8_CPP_CHECK.txt"
LOGFILE_STEP_9_XCASH_DPOPS_TEST="${LOGFILE_DIR}STEP_9_XCASH_DPOPS_TEST.txt"
LOGFILE_STEP_10_MINE_BLOCKS="${LOGFILE_DIR}STEP_10_MINE_BLOCKS.txt"
LOGFILE_STEP_11_MINE_BLOCKS_ANALYZE="${LOGFILE_DIR}STEP_11_MINE_BLOCKS_ANALYZE.txt"
LOGFILE_STEP_12_MINE_BLOCKS_ANALYZE_THREADS="${LOGFILE_DIR}STEP_12_MINE_BLOCKS_ANALYZE_THREADS.txt"

# System settings
CPU_THREADS=$(nproc)

# Functions
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
    echo -e "Updating Packages List\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
    sudo apt update -y >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
    echo -e "\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
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
    echo -e "Updating Packages\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
    sudo apt dist-upgrade -y >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
   echo -e "\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
}



function update_all_repositories()
{
  cd "${XCASH_DPOPS_INSTALLATION_DIR}" || return 1
  sudo rm -r "${XCASH_DIR}"
  sudo rm -r "${XCASH_DPOPS_DIR}"
  sudo rm -r "${DELEGATES_WEBSITE_DIR}"
  sudo rm -r "${SHARED_DELEGATES_WEBSITE_DIR}"
  git clone ${XCASH_URL} >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  git clone ${XCASH_DPOPS_URL} >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  git clone ${DELEGATES_WEBSITE_URL} >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  git clone ${SHARED_DELEGATES_WEBSITE_URL} >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
}
  



function get_dependencies_current_version()
{
  echo -e "Getting Dependencies Current Versions\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  NODEJS_CURRENT_VERSION=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -type d -name "node-*-linux-x64" -exec basename {} \;) || return 1
  MONGODB_CURRENT_VERSION=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -type d -name "mongodb-linux-x86_64-ubuntu1804-*" -exec basename {} \;) || return 1
  MONGOC_DRIVER_CURRENT_VERSION=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -type d -name "mongo-c-driver-*" -exec basename {} \;) || return 1
  CPP_CHECK_CURRENT_VERSION=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -type d -name "cppcheck-*" -exec basename {} \;) || return 1
  echo -e "\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
}



function start_processes()
{
  echo -e "Starting Processes\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  screen -dmS MongoDB "${MONGODB_DIR}"bin/mongod --dbpath ${MONGODB_INSTALLATION_DIR} || return 1
  screen -dmS XCASH_Daemon "${XCASH_DIR}"build/release/bin/xcashd --data-dir "${XCASH_BLOCKCHAIN_INSTALLATION_DIR}" --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind || return 1
  sleep 10s || return 1
  screen -dmS XCASH_Wallet "${XCASH_DIR}"build/release/bin/xcash-wallet-rpc --wallet-file "${XCASH_DPOPS_INSTALLATION_DIR}"xcash-wallets/delegate-wallet --password password --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon || return 1
  sleep 10s || return 1
  echo -e "\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
}

function stop_processes()
{
  echo -e "Stoping Processes\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  screen -XS "MongoDB" quit || return 1
  screen -XS "XCASH_Daemon" quit || return 1
  screen -XS "XCASH_Wallet" quit || return 1
  echo -e "\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
}



function update_mongodb()
{
  echo -e "Updating MongoDB\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  sudo rm -r "${MONGODB_DIR}"   || return 1
  cd "${XCASH_DPOPS_INSTALLATION_DIR}" || return 1
  wget -q ${MONGODB_URL} || return 1
  tar -xf mongodb-linux-x86_64-*.tgz >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  sudo rm mongodb-linux-x86_64-*.tgz >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  MONGODB_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -type d -name "mongodb-linux-x86_64-ubuntu1804-*")/ || return 1
  sudo sed '/mongodb-linux-x86_64-ubuntu1804-/d' -i ~/.profile || return 1
  sudo sed '/^[[:space:]]*$/d' -i ~/.profile || return 1
  sudo echo -ne "\nexport PATH=${MONGODB_DIR}bin:" >> ~/.profile  || return 1
  sudo echo -ne '$PATH' >> ~/.profile || return 1
  source ~/.profile || return 1
  echo -e "\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
}



function update_mongoc_driver()
{
  echo -e "Updating Mongo C Driver\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  sudo rm -r "${MONGOC_DRIVER_DIR}" || return 1
  cd "${XCASH_DPOPS_INSTALLATION_DIR}" || return 1
  wget -q ${MONGOC_DRIVER_URL} || return 1
  tar -xf mongo-c-driver-*.tar.gz >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  sudo rm mongo-c-driver-*.tar.gz >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  cd mongo-c-driver-* || return 1
  mkdir cmake-build >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  cd cmake-build >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  sudo make -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF .. >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  sudo make -j "${CPU_THREADS}" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  sudo make install >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  sudo ldconfig || return 1
  MONGOC_DRIVER_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -type d -name "mongo-c-driver-*")/ || return 1
  echo -e "\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
}



function update_nodejs()
{
  echo -e "Updating NodeJS\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  NODEJS_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -type d -name "node-*-linux-x64")/ || return 1
  sudo rm -r "${NODEJS_DIR}"   || return 1
  cd "${XCASH_DPOPS_INSTALLATION_DIR}" || return 1
  wget -q ${NODEJS_URL} || return 1
  tar -xf node*.tar.xz >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  sudo rm node*.tar.xz >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  NODEJS_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -type d -name "node-*-linux-x64")/ || return 1
  sudo sed '/node-v/d' -i ~/.profile || return 1
  sudo sed '/PATH=\/bin:/d' -i ~/.profile || return 1
  sudo sed '/^[[:space:]]*$/d' -i ~/.profile || return 1
  sudo echo -ne "\nexport PATH=${NODEJS_DIR}bin:" >> ~/.profile  || return 1
  sudo echo -ne '$PATH' >> ~/.profile || return 1
  source ~/.profile || return 1
  echo -e "\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
}



function update_cppcheck()
{
  echo -e "Updating CPP Check\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  sudo rm -r "${CPP_CHECK_DIR}"
  cd "${XCASH_DPOPS_INSTALLATION_DIR}" || return 1
  wget -q ${CPP_CHECK_URL} || return 1
  tar -xf ${CPP_CHECK_LATEST_VERSION:9}.tar.gz >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  sudo rm ${CPP_CHECK_LATEST_VERSION:9}.tar.gz >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  CPP_CHECK_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -type d -name "cppcheck-*")/ || return 1
  cd "${CPP_CHECK_DIR}" || return 1
  make clean ; make -j "${CPU_THREADS}" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  echo -e "\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
}



function update_npm()
{
  echo -e "Updating NPM\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  npm install -g npm >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  echo -e "\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
}



function STEP_1_PROGRAMS()
{
  # Reset the log Files
  rm -r "${LOGFILE_DIR}"
  mkdir "${LOGFILE_DIR}"

  # Update the profile
  source ~/.profile

  # Get the dependencies current version
  get_dependencies_current_version || return 1

  # Update the repositories
  update_all_repositories || return 1

  # Update the package list
  update_packages_list || return 1

  # Update all system packages
  update_packages || return 1

  # Update all dependencies
  if [ ! "$MONGODB_CURRENT_VERSION" == "$MONGODB_LATEST_VERSION" ]; then
    update_mongodb || return 1
  else
    echo -e "MongoDB Is Already Up To Date\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  fi
  if [ ! "$MONGOC_DRIVER_CURRENT_VERSION" == "$MONGOC_DRIVER_LATEST_VERSION" ]; then
    update_mongoc_driver || return 1
  else
    echo -e "MongoC Driver Is Already Up To Date\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  fi
  if [ ! "$NODEJS_CURRENT_VERSION" == "$NODEJS_LATEST_VERSION" ]; then
    update_nodejs || return 1
  else
    echo -e "NodeJS Is Already Up To Date\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  fi
  if [ ! "$CPP_CHECK_CURRENT_VERSION" == "$CPP_CHECK_LATEST_VERSION" ]; then
    update_cppcheck || return 1
  else
    echo -e "CPP Check Is Already Up To Date\n" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1 || return 1
  fi
  update_npm || return 1
}



function STEP_2_DELEGATES_WEBSITE()
{
  echo -e "Updating Delegates Website\n" >> "${LOGFILE_STEP_2_DELEGATES_WEBSITE}" 2>&1 || return 1
  cd "${DELEGATES_WEBSITE_DIR}" || return 1
  source ~/.profile
  npm update >> "${LOGFILE_STEP_2_DELEGATES_WEBSITE}" 2>&1 || return 1
  npm run build >> "${LOGFILE_STEP_2_DELEGATES_WEBSITE}" 2>&1 || return 1
  cd dist || return 1
  for f in *.js; do uglifyjs "$f" --compress --mangle --output "{$f}min"; rm "$f"; mv "{$f}min" "$f"; done || return 1
  if [ -d "$XCASH_DPOPS_DELEGATE_FOLDER_DIR" ]; then
    sudo rm -r "${XCASH_DPOPS_DELEGATE_FOLDER_DIR}" || return 1
  fi 
  cd ../ || return 1
  cp -a dist "${XCASH_DPOPS_DELEGATE_FOLDER_DIR}" || return 1
  echo -e "\n" >> "${LOGFILE_STEP_2_DELEGATES_WEBSITE}" 2>&1 || return 1
}



function STEP_3_SHARED_DELEGATES_WEBSITE()
{
  echo -e "Updating Shared Delegates Website\n" >> "${LOGFILE_STEP_3_SHARED_DELEGATES_WEBSITE}" 2>&1 || return 1
  cd "${SHARED_DELEGATES_WEBSITE_DIR}" || return 1
  source ~/.profile
  npm update >> "${LOGFILE_STEP_3_SHARED_DELEGATES_WEBSITE}" 2>&1 || return 1
  npm run build >> "${LOGFILE_STEP_3_SHARED_DELEGATES_WEBSITE}" 2>&1 || return 1
  cd dist || exit
  for f in *.js; do uglifyjs "$f" --compress --mangle --output "{$f}min"; rm "$f"; mv "{$f}min" "$f"; done
  if [ -d "$XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR" ]; then
    sudo rm -r "${XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR}" || return 1
  fi 
  cd ../ || return 1
  cp -a dist "${XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR}" || return 1
  echo -e "\n" >> "${LOGFILE_STEP_3_SHARED_DELEGATES_WEBSITE}" 2>&1 || return 1
}



function STEP_4_XCASH_BUILD_MAINNET()
{
  echo -e "Updating X-CASH Mainnet\n" >> "${LOGFILE_STEP_4_XCASH_BUILD_MAINNET}" 2>&1 || return 1
  cd "${XCASH_DIR}" || return 1
  git checkout master >> "${LOGFILE_STEP_4_XCASH_BUILD_MAINNET}" 2>&1 || return 1
  echo "y" | make clean  >> "${LOGFILE_STEP_4_XCASH_BUILD_MAINNET}" 2>&1 || return 1
  make -j "${CPU_THREADS}" >> "${LOGFILE_STEP_4_XCASH_BUILD_MAINNET}" 2>&1 || return 1
  echo -e "\n" >> "${LOGFILE_STEP_4_XCASH_BUILD_MAINNET}" 2>&1 || return 1
}



function STEP_5_XCASH_BLOCKCHAIN_SYNCING()
{
  echo -e "Testing X-CASH Blockchain Syncing\n" >> "${LOGFILE_STEP_5_XCASH_BLOCKCHAIN_SYNCING}" 2>&1 || return 1
  cd "${XCASH_DIR}"build/release/bin || return 1
  mkdir xcash || return 1
  screen -dmS xcash_daemon ./xcashd --data-dir ./xcash || return 1
  sleep 10m || return 1
  curl -X POST http://127.0.0.1:18281/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"get_block_count"}' -H 'Content-Type: application/json' >> "${LOGFILE_STEP_5_XCASH_BLOCKCHAIN_SYNCING}" 2>&1 || return 1
  screen -XS "xcash_daemon" quit || return 1
  sudo rm -r xcash || return 1
  echo -e "\n" >> "${LOGFILE_STEP_5_XCASH_BLOCKCHAIN_SYNCING}" 2>&1 || return 1
}



function STEP_6_XCASH_BUILD_DEVELOPMENT()
{
  echo -e "Updating X-CASH Development\n" >> "${LOGFILE_STEP_6_XCASH_BUILD_DEVELOPMENT}" 2>&1 || return 1
  cd "${XCASH_DIR}" || return 1
  git checkout xcash_proof_of_stake >> "${LOGFILE_STEP_6_XCASH_BUILD_DEVELOPMENT}" 2>&1 || return 1
  echo "y" | make clean  >> "${LOGFILE_STEP_6_XCASH_BUILD_DEVELOPMENT}" 2>&1 || return 1
  make -j "${CPU_THREADS}" >> "${LOGFILE_STEP_6_XCASH_BUILD_DEVELOPMENT}" 2>&1 || return 1
  echo -e "\n" >> "${LOGFILE_STEP_6_XCASH_BUILD_DEVELOPMENT}" 2>&1 || return 1
}



function STEP_7_XCASH_DPOPS_BUILD()
{
  echo -e "Updating xcash-dpops\n" >> "${LOGFILE_STEP_7_XCASH_DPOPS_BUILD}" 2>&1 || return 1
  cd "${XCASH_DPOPS_DIR}" || return 1
  make clean  >> "${LOGFILE_STEP_7_XCASH_DPOPS_BUILD}" 2>&1 || return 1
  make release -j "${CPU_THREADS}" >> "${LOGFILE_STEP_7_XCASH_DPOPS_BUILD}" 2>&1 || return 1
  echo -e "\n" >> "${LOGFILE_STEP_7_XCASH_DPOPS_BUILD}" 2>&1 || return 1
}



function STEP_8_CPP_CHECK()
{
  echo -e "CPP Check\n" >> "${LOGFILE_STEP_8_CPP_CHECK}" 2>&1 || return 1
  cd "${CPP_CHECK_DIR}" || return 1
  ./cppcheck --enable=all --suppress=unusedFunction --suppress=variableScope "${XCASH_DPOPS_DIR}" >> "${LOGFILE_STEP_8_CPP_CHECK}" 2>&1 || return 1
  echo -e "\n" >> "${LOGFILE_STEP_8_CPP_CHECK}" 2>&1 || return 1
}



function STEP_9_XCASH_DPOPS_TEST()
{
  echo -e "xcash-dpops Test\n" >> "${LOGFILE_STEP_9_XCASH_DPOPS_TEST}" 2>&1 || return 1
  cd "${XCASH_DPOPS_DIR}" || return 1
  systemctl start MongoDB XCASH_DPOPS && sleep 30s && systemctl start XCASH_Wallet
  build/xcash-dpops --block-verifiers-secret-key c8c066b90e8059c505971e710267a48d01191a3d2be233f9081cde0e08f30ccfad98cadb3b13229c78709876955247cbef40d5b15c4842be605b0e8b30c97a7a --quick-test >> "${LOGFILE_STEP_9_XCASH_DPOPS_TEST}" 2>&1 || return 1
  echo -e "\n" >> "${LOGFILE_STEP_9_XCASH_DPOPS_TEST}" 2>&1 || return 1
}



STEP_1_PROGRAMS || echo "STEP_1_PROGRAMS_ERROR" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1
STEP_2_DELEGATES_WEBSITE || echo "STEP_2_DELEGATES_WEBSITE_ERROR" >> "${LOGFILE_STEP_2_DELEGATES_WEBSITE}" 2>&1
STEP_3_SHARED_DELEGATES_WEBSITE || echo "STEP_3_SHARED_DELEGATES_WEBSITE_ERROR" >> "${LOGFILE_STEP_3_SHARED_DELEGATES_WEBSITE}" 2>&1
STEP_4_XCASH_BUILD_MAINNET || echo "STEP_4_XCASH_BUILD_MAINNET_ERROR" >> "${LOGFILE_STEP_4_XCASH_BUILD_MAINNET}" 2>&1
STEP_5_XCASH_BLOCKCHAIN_SYNCING || echo "STEP_5_XCASH_BLOCKCHAIN_SYNCING_ERROR" >> "${LOGFILE_STEP_5_XCASH_BLOCKCHAIN_SYNCING}" 2>&1
STEP_6_XCASH_BUILD_DEVELOPMENT || echo "STEP_6_XCASH_BUILD_DEVELOPMENT_ERROR" >> "${LOGFILE_STEP_6_XCASH_BUILD_DEVELOPMENT}" 2>&1
STEP_7_XCASH_DPOPS_BUILD || echo "STEP_7_XCASH_DPOPS_BUILD_ERROR" >> "${LOGFILE_STEP_7_XCASH_DPOPS_BUILD}" 2>&1
STEP_8_CPP_CHECK || echo "STEP_8_CPP_CHECK_ERROR" >> "${LOGFILE_STEP_8_CPP_CHECK}" 2>&1
start_processes || echo "STEP_1_PROGRAMS_ERROR" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1
STEP_9_XCASH_DPOPS_TEST || echo "STEP_9_XCASH_DPOPS_TEST_ERROR" >> "${LOGFILE_STEP_9_XCASH_DPOPS_TEST}" 2>&1
stop_processes || echo "STEP_1_PROGRAMS_ERROR" >> "${LOGFILE_STEP_1_PROGRAMS}" 2>&1
