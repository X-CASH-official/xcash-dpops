#!/bin/bash

# Color print variables
COLOR_PRINT_RED="\033[1;31m"
COLOR_PRINT_GREEN="\033[1;32m"
COLOR_PRINT_YELLOW="\033[1;33m"
END_COLOR_PRINT="\033[0m"

# Configuration settings
INSTALLATION_DIR="/root/x-network/"
XCASH_DPOPS_INSTALLATION_DIR="${INSTALLATION_DIR}XCASH_DPOPS/"
XCASH_WALLETS_INSTALLATION_DIR="${INSTALLATION_DIR}xcash_wallets/"
XCASH_INSTALLATION_DIR="${INSTALLATION_DIR}X-CASH/"
SHARED_DELEGATES_INSTALLATION_DIR="${INSTALLATION_DIR}XCASH_DPOPS_shared_delegates_website/"
XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR=${XCASH_DPOPS_INSTALLATION_DIR}shared_delegates_website/
LOGS_DIR="${INSTALLATION_DIR}logs/"

BLOCK_VERIFIER_KEY_SETTINGS=""
BLOCK_VERIFIER_SECRET_KEY="00"
BLOCK_VERIFIER_PUBLIC_KEY=""
WALLET_SETTINGS="0"
WALLET_SEED=""
WALLET_PASSWORD_SETTINGS=""
WALLET_PASSWORD=$(< /dev/urandom tr -dc 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789' | head -c"${1:-32}";echo;)
XCASH_DPOPS_PACKAGES="build-essential cmake pkg-config libboost-all-dev libssl-dev libzmq3-dev libunbound-dev libsodium-dev libminiupnpc-dev libunwind8-dev liblzma-dev libreadline6-dev libldns-dev libexpat1-dev libgtest-dev doxygen graphviz libpcsclite-dev git screen"
GIT_PULL_ALREADY_UPDATED_MESSAGE="Already up to date."

# Program Settings
MONGODB_CURRENT_VERSION=""
NODEJS_CURRENT_VERSION=""
MONGOC_DRIVER_CURRENT_VERSION=""

MONGODB_LATEST_VERSION="mongodb-linux-x86_64-ubuntu1804-4.2.2"
MONGOC_DRIVER_LATEST_VERSION="mongo-c-driver-1.15.3"
NODEJS_LATEST_VERSION="node-v13.5.0-linux-x64"

XCASH_URL="https://github.com/X-CASH-official/X-CASH.git"
XCASH_DPOPS_URL="https://github.com/X-CASH-official/XCASH_DPOPS.git"
SHARED_DELEGATES_WEBSITE_URL="https://github.com/X-CASH-official/XCASH_DPOPS_shared_delegates_website.git"
NODEJS_URL="https://nodejs.org/dist/${NODEJS_LATEST_VERSION:5:7}/${NODEJS_LATEST_VERSION}.tar.xz"
MONGODB_URL="http://downloads.mongodb.org/linux/${MONGODB_LATEST_VERSION}.tgz"
MONGOC_DRIVER_URL="https://github.com/mongodb/mongo-c-driver/releases/download/${MONGOC_DRIVER_LATEST_VERSION:15}/${MONGOC_DRIVER_LATEST_VERSION}.tar.gz"


# Files
BLOCK_VERIFIERS_SIGN_AND_VERIFY_MESSAGES_FILE=""

# Settings
BLOCK_VERIFIERS_PUBLIC_KEY_LENGTH=64
BLOCK_VERIFIERS_SECRET_KEY_LENGTH=128
CPU_THREADS=$(nproc)
RAM=$(awk '/MemTotal/ { printf "%d \n", $2/1024/1024 }' /proc/meminfo)
RAM_CPU_RATIO=$((RAM / CPU_THREADS))
RAM_CPU_RATIO_ALL_CPU_THREADS=4

# Regex
regex_MNEMONIC_SEED="^\b([a-z]+\s+){24}\b([a-z]+)$" # 25 words exactly
regex_DPOPS_FEE="\b(^[1-9]{1}[0-9]{0,1}.?[0-9]{0,6}$)\b$" # between 1 and 99 with up to 6 decimal places
regex_DPOPS_MINIMUM_AMOUNT="\b(^[1-9]{1}[0-9]{4,6}$)\b$" # between 10000 and 10000000-1



# Functions
function get_wallet_settings()
{
  while
    echo -ne "${COLOR_PRINT_YELLOW}Block Verifier Key Settings: (I)mport or (C)reate: ${END_COLOR_PRINT}"
    read -r WALLET_SETTINGS
    if [ "${WALLET_SETTINGS}" == "" ]; then
      WALLET_SETTINGS="0"
    fi
    echo -ne "\r"
    echo
    [ ! ${WALLET_SETTINGS^^} == "I" ] && [ ! ${WALLET_SETTINGS^^} == "C" ]
  do true; done
  if [ "${WALLET_SETTINGS^^}" == "I" ]; then
    while
      echo -ne "${COLOR_PRINT_YELLOW}Please Enter the Existing Wallets Mnemonic Seed: ${END_COLOR_PRINT}"
      read -r WALLET_SEED
      echo -ne "\r"
      echo
      [[ ! $WALLET_SEED =~ $regex_MNEMONIC_SEED ]]
    do true; done
  fi
}

function get_password_settings()
{
  while
    echo -ne "${COLOR_PRINT_YELLOW}Generate a New Wallet Password: (Y)es (N)o ${END_COLOR_PRINT}"
    read -r WALLET_PASSWORD_SETTINGS
    if [ "${WALLET_PASSWORD_SETTINGS}" == "" ]; then
      WALLET_PASSWORD_SETTINGS="0"
    fi
    echo -ne "\r"
    echo
    [ ! ${WALLET_PASSWORD_SETTINGS^^} == "Y" ] && [ ! ${WALLET_PASSWORD_SETTINGS^^} == "N" ]
  do true; done
  if [ ! "${WALLET_PASSWORD_SETTINGS^^}" == "Y" ]; then
  echo -ne "${COLOR_PRINT_YELLOW}Please Enter the Custom Password: ${END_COLOR_PRINT}"
  read -r WALLET_PASSWORD
  echo -ne "\r"
  echo
  fi
}

function get_block_verifier_key_settings()
{
  while
    echo -ne "${COLOR_PRINT_YELLOW}Block Verifier Key Settings: (I)mport or (C)reate: ${END_COLOR_PRINT}"
    read -r BLOCK_VERIFIER_KEY_SETTINGS
    if [ "${BLOCK_VERIFIER_KEY_SETTINGS}" == "" ]; then
      BLOCK_VERIFIER_KEY_SETTINGS="0"
    fi
    echo -ne "\r"
    echo
    [ ! ${BLOCK_VERIFIER_KEY_SETTINGS^^} == "I" ] && [ ! ${BLOCK_VERIFIER_KEY_SETTINGS^^} == "C" ]
  do true; done
  if [ ${BLOCK_VERIFIER_KEY_SETTINGS^^} == "I" ]; then
    while
      echo -ne "${COLOR_PRINT_YELLOW}Please Enter Block Verifiers Secret Key: ${END_COLOR_PRINT}"
      read -r BLOCK_VERIFIER_SECRET_KEY
      echo -ne "\r"
      echo
      [[ ! ${#BLOCK_VERIFIER_SECRET_KEY} -eq $BLOCK_VERIFIERS_SECRET_KEY_LENGTH ]]
    do true; done
    BLOCK_VERIFIER_PUBLIC_KEY="${BLOCK_VERIFIER_SECRET_KEY: -${BLOCK_VERIFIERS_PUBLIC_KEY_LENGTH}}"
  fi
}

function print_installation_settings()
{
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                    Configuration Settings                  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  data=$([ "${WALLET_SETTINGS^^}" == "I" ] && echo "Import" || echo "Create")
  echo -e "${COLOR_PRINT_GREEN}Wallet Settings: ${data} ${END_COLOR_PRINT}"
  data=$([ "${WALLET_PASSWORD_SETTINGS^^}" == "Y" ] && echo "Create" || echo "Custom")
  echo -e "${COLOR_PRINT_GREEN}Wallet Password Settings: ${data} ${END_COLOR_PRINT}"
  data=$([ "${BLOCK_VERIFIER_KEY_SETTINGS^^}" == "I" ] && echo "Import" || echo "Create")
  echo -e "${COLOR_PRINT_GREEN}Block Verifier Key Settings: ${data} ${END_COLOR_PRINT}"

  seconds=10
  while [ "$seconds" -ne 0 ]
  do
    echo -ne "${COLOR_PRINT_GREEN}Configuration will start in ${seconds} seconds, press Ctrl + C to cancel!${END_COLOR_PRINT}"
    seconds=$((seconds-1))
    sleep 1
    echo -ne "\r"
  done 
  echo -ne "${COLOR_PRINT_GREEN}Configuration will start in 0 seconds, press Ctrl + C to cancel!${END_COLOR_PRINT}"
  echo
  echo
}

function configuration_settings()
{
  get_wallet_settings
  get_password_settings
  get_block_verifier_key_settings
  print_installation_settings
}










function start_processes()
{
  echo -ne "${COLOR_PRINT_YELLOW}Starting Processes${END_COLOR_PRINT}"
  screen -dmS MongoDB /root/x-network/mongodb-*/bin/mongod --dbpath /data/db/
  screen -dmS XCASH_Daemon ${XCASH_INSTALLATION_DIR}build/release/bin/xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind
  sleep 30s
  screen -dmS XCASH_Wallet ${XCASH_INSTALLATION_DIR}build/release/bin/xcash-wallet-rpc --wallet-file ${XCASH_WALLETS_INSTALLATION_DIR}XCASH_DPOPS_WALLET --password "${WALLET_PASSWORD}" --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon
  sleep 30s
  echo -ne "\r${COLOR_PRINT_GREEN}Starting Processes${END_COLOR_PRINT}"
  echo
}

function stop_processes()
{
  echo -ne "${COLOR_PRINT_YELLOW}Stoping Programs${END_COLOR_PRINT}"
  screen -XS "XCASH_DPOPS_AUTO_RESTART" quit > /dev/null 2>&1
  screen -XS "XCASH_DPOPS" quit > /dev/null 2>&1
  screen -XS "MongoDB" quit > /dev/null 2>&1
  screen -XS "XCASH_Daemon" quit > /dev/null 2>&1
  screen -XS "XCASH_Wallet" quit > /dev/null 2>&1 
  sleep 30s
  echo -ne "\r${COLOR_PRINT_GREEN}Stoping Programs${END_COLOR_PRINT}"
  echo
}










function create_block_verifier_key()
{
  echo -ne "${COLOR_PRINT_YELLOW}Creating Block Verifiers Key${END_COLOR_PRINT}"
  cd ${XCASH_DPOPS_INSTALLATION_DIR} || exit
  data=$(build/XCASH_DPOPS --generate_key 2>&1 >/dev/null)
  BLOCK_VERIFIER_SECRET_KEY="${data: -132}"
  BLOCK_VERIFIER_SECRET_KEY="${BLOCK_VERIFIER_SECRET_KEY:0:128}"
  BLOCK_VERIFIER_PUBLIC_KEY="${BLOCK_VERIFIER_SECRET_KEY: -${BLOCK_VERIFIERS_PUBLIC_KEY_LENGTH}}"
  echo -ne "\r${COLOR_PRINT_GREEN}Creating Block Verifiers Key${END_COLOR_PRINT}"
  echo
}

function build_xcash()
{
  echo -ne "${COLOR_PRINT_YELLOW}Building X-CASH (This Might Take A While)${END_COLOR_PRINT}"
  cd "${XCASH_INSTALLATION_DIR}"
  echo "y" | make clean > /dev/null 2>&1
  if [ "$RAM_CPU_RATIO" -ge "$RAM_CPU_RATIO_ALL_CPU_THREADS" ]; then
    make release -j "${CPU_THREADS}" >> /dev/null 2>&1
  else
    make release -j $((CPU_THREADS / 2)) >> /dev/null 2>&1 
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Building X-CASH (This Might Take A While)${END_COLOR_PRINT}"
  echo
}










function create_xcash_wallet()
{
  echo -ne "${COLOR_PRINT_YELLOW}Creating X-CASH Wallet (This might take a while)${END_COLOR_PRINT}"
  screen -dmS XCASH_Daemon ${XCASH_INSTALLATION_DIR}build/release/bin/xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind
  sleep 30s
  echo "exit" | ${XCASH_INSTALLATION_DIR}build/release/bin/xcash-wallet-cli --generate-new-wallet ${XCASH_WALLETS_INSTALLATION_DIR}XCASH_DPOPS_WALLET --password "${WALLET_PASSWORD}" --mnemonic-language English --restore-height 0 > /dev/null 2>&1
  screen -XS "XCASH_Daemon" quit > /dev/null 2>&1
  sleep 30s
  echo -ne "\r${COLOR_PRINT_GREEN}Creating X-CASH Wallet (This might take a while)${END_COLOR_PRINT}"
  echo
}

function import_xcash_wallet()
{
  echo -ne "${COLOR_PRINT_YELLOW}Importing X-CASH Wallet (This might take a while)${END_COLOR_PRINT}"
  screen -dmS XCASH_Daemon ${XCASH_INSTALLATION_DIR}build/release/bin/xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind
  sleep 30s
  (echo -ne "\n"; echo "${WALLET_PASSWORD}"; echo "exit") | ${XCASH_INSTALLATION_DIR}build/release/bin/xcash-wallet-cli --restore-deterministic-wallet --electrum-seed "${WALLET_SEED}" --generate-new-wallet ${XCASH_WALLETS_INSTALLATION_DIR}XCASH_DPOPS_WALLET --password "${WALLET_PASSWORD}" --mnemonic-language English --restore-height 0 >> /dev/null 2>&1
  screen -XS "XCASH_Daemon" quit > /dev/null 2>&1
  sleep 30s
  echo -ne "\r${COLOR_PRINT_GREEN}Importing X-CASH Wallet (This might take a while)${END_COLOR_PRINT}"
  echo
}

function get_xcash_wallet_data()
{
  echo -ne "${COLOR_PRINT_YELLOW}Getting X-CASH Wallet Data${END_COLOR_PRINT}"
  PUBLIC_ADDRESS=$(curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"get_address"}' -H 'Content-Type: application/json' | grep \"address\" | head -1 | sed s"|    \"address\": ||g" | sed s"|\"||g" | sed s"|,||g")
  SPEND_KEY=$(curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"query_key","params":{"key_type":"spend_key"}}' -H 'Content-Type: application/json' | grep \"key\" | sed s"|    \"key\": ||g" | sed s"|\"||g")
  VIEW_KEY=$(curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"query_key","params":{"key_type":"view_key"}}' -H 'Content-Type: application/json' | grep \"key\" | sed s"|    \"key\": ||g" | sed s"|\"||g")
  MNEMONIC_SEED=$(curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"query_key","params":{"key_type":"mnemonic"}}' -H 'Content-Type: application/json' | grep \"key\" | sed s"|    \"key\": ||g" | sed s"|\"||g")
  echo -ne "\r${COLOR_PRINT_GREEN}Getting X-CASH Wallet Data${END_COLOR_PRINT}"
  echo
}










function configuration()
{
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                  Starting Configuration                    ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"

  configuration_settings

  build_xcash

  # Create or import the wallet
  if [ "${WALLET_SETTINGS^^}" == "C" ]; then
    create_xcash_wallet
  else
    import_xcash_wallet
  fi

  start_processes

  # Create the block verifier key if they choose to create a block verifier key
  if [ "${BLOCK_VERIFIER_KEY_SETTINGS^^}" == "C" ]; then
    create_block_verifier_key
  fi

  get_xcash_wallet_data

  stop_processes

  # Display Configuration data
  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}          Configuration Has Completed Successfully          ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo
  echo -e "${COLOR_PRINT_YELLOW}Public Address: $PUBLIC_ADDRESS${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_YELLOW}Mnemonic Seed: $MNEMONIC_SEED${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_YELLOW}Spend Key: $SPEND_KEY${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_YELLOW}View Key: $VIEW_KEY${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_YELLOW}Password: $WALLET_PASSWORD${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_YELLOW}Block Verifier Public Key: $BLOCK_VERIFIER_PUBLIC_KEY${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_YELLOW}Block Verifier Secret Key: $BLOCK_VERIFIER_SECRET_KEY${END_COLOR_PRINT}"
}










function get_dependencies_current_version()
{
  echo -ne "${COLOR_PRINT_YELLOW}Getting Dependencies Current Versions${END_COLOR_PRINT}"
  NODEJS_CURRENT_VERSION=$(sudo find / -type d -name "node-*-linux-x64" -exec basename {} \;)
  MONGODB_CURRENT_VERSION=$(sudo find / -type d -name "mongodb-linux-x86_64-ubuntu1804-*" -exec basename {} \;)
  MONGOC_DRIVER_CURRENT_VERSION=$(sudo find / -type d -name "mongo-c-driver-*" -exec basename {} \;)
  echo -ne "\r${COLOR_PRINT_GREEN}Getting Dependencies Current Versions${END_COLOR_PRINT}"
  echo
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
        echo && echo -en "\r${COLOR_PRINT_RED}[$j] Waiting for other package manager to finish...${END_COLOR_PRINT}" 
        sleep 0.25
        ((i=i+1))
    done
    echo -ne "${COLOR_PRINT_YELLOW}Updating Packages List${END_COLOR_PRINT}"
    sudo apt update -y >> /dev/null 2>&1
    echo -ne "\r${COLOR_PRINT_GREEN}Updating Packages List${END_COLOR_PRINT}"
    echo
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
        echo && echo -en "\r${COLOR_PRINT_RED}[$j] Waiting for other package manager to finish...${END_COLOR_PRINT}" 
        sleep 0.25
        ((i=i+1))
    done
    echo -ne "${COLOR_PRINT_YELLOW}Updating Packages${END_COLOR_PRINT}"
    sudo apt install --only-upgrade ${XCASH_DPOPS_PACKAGES} -y >> /dev/null 2>&1
    echo -ne "\r${COLOR_PRINT_GREEN}Updating Packages${END_COLOR_PRINT}"
    echo
}



function update_xcash()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating X-CASH (This Might Take A While)${END_COLOR_PRINT}"
  if [ ! -d "$XCASH_INSTALLATION_DIR" ]; then
    cd "${INSTALLATION_DIR}"
    git clone "${XCASH_URL}" >> /dev/null 2>&1
  fi
  cd "${XCASH_INSTALLATION_DIR}"
  data=$(git pull) >> /dev/null 2>&1
  if [ ! "$data" == "$GIT_PULL_ALREADY_UPDATED_MESSAGE" ]; then
    if [ "$RAM_CPU_RATIO" -ge "$RAM_CPU_RATIO_ALL_CPU_THREADS" ]; then
      make release -j "${CPU_THREADS}" >> /dev/null 2>&1
    else
      make release -j $((CPU_THREADS / 2)) >> /dev/null 2>&1
    fi 
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Updating X-CASH (This Might Take A While)${END_COLOR_PRINT}"
  echo
}

function update_xcash_dpops()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating XCASH_DPOPS${END_COLOR_PRINT}"
  if [ ! -d "$XCASH_DPOPS_INSTALLATION_DIR" ]; then
    cd "${INSTALLATION_DIR}"
    git clone "${XCASH_DPOPS_URL}" >> /dev/null 2>&1
  fi
  cd "${XCASH_DPOPS_INSTALLATION_DIR}"
  data=$(git pull) >> /dev/null 2>&1
  if [ ! "$data" == "$GIT_PULL_ALREADY_UPDATED_MESSAGE" ]; then
    if [ "$RAM_CPU_RATIO" -ge "$RAM_CPU_RATIO_ALL_CPU_THREADS" ]; then
      make release -j "${CPU_THREADS}" >> /dev/null 2>&1
    else
      make release -j $((CPU_THREADS / 2)) >> /dev/null 2>&1
    fi 
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Updating XCASH_DPOPS${END_COLOR_PRINT}"
  echo
}

function update_shared_delegates_website()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating Shared Delegates Website${END_COLOR_PRINT}"
  if [ ! -d "$SHARED_DELEGATES_INSTALLATION_DIR" ]; then
    cd "${INSTALLATION_DIR}"
    git clone "${SHARED_DELEGATES_WEBSITE_URL}" >> /dev/null 2>&1
  fi
  cd "${SHARED_DELEGATES_INSTALLATION_DIR}"
  data=$(git pull) >> /dev/null 2>&1
  if [ ! "$data" == "$GIT_PULL_ALREADY_UPDATED_MESSAGE" ]; then
    npm update >> /dev/null 2>&1
    ng build --prod --aot >> /dev/null 2>&1
    cd dist
    for f in *.js; do uglifyjs "$f" --compress --mangle --output "{$f}min"; rm "$f"; mv "{$f}min" "$f"; done
    if [ -d "$XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR" ]; then
      sudo rm -r "${XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR}"
    fi 
    cd ../
    cp -a dist "${XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR}"
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Updating Shared Delegates Website${END_COLOR_PRINT}"
  echo
}

function update_mongodb()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating MongoDB${END_COLOR_PRINT}"
  cd "${INSTALLATION_DIR}"
  sudo rm -r mongodb-*
  wget -q ${MONGODB_URL}
  tar -xf mongodb-linux-x86_64-*.tgz >> /dev/null 2>&1
  sudo rm mongodb-linux-x86_64-*.tgz >> /dev/null 2>&1
  MONGODB_DIR=$(sudo find / -type d -name "mongodb-linux-x86_64-ubuntu1804-*")/
  sudo sed '/mongodb-linux-x86_64-ubuntu1804-/d' -i /root/.profile
  sudo sed '/^[[:space:]]*$/d' -i /root/.profile
  sudo echo -ne "\nexport PATH=${MONGODB_DIR}bin:" >> /root/.profile 
  sudo echo -ne '$PATH' >> /root/.profile
  . /root/.profile
  echo -ne "\r${COLOR_PRINT_GREEN}Updating MongoDB${END_COLOR_PRINT}"
  echo
}

function update_mongoc_driver()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating Mongo C Driver${END_COLOR_PRINT}"
  cd "${INSTALLATION_DIR}"
  sudo rm -r mongo-c-driver-*
  wget -q ${MONGOC_DRIVER_URL}
  tar -xf mongo-c-driver-*.tar.gz >> /dev/null 2>&1
  sudo rm mongo-c-driver-*.tar.gz >> /dev/null 2>&1
  cd mongo-c-driver-*
  mkdir cmake-build >> /dev/null 2>&1
  cd cmake-build >> /dev/null 2>&1
  cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF .. >> /dev/null 2>&1
  sudo make -j "${CPU_THREADS}" >> /dev/null 2>&1
  sudo make install >> /dev/null 2>&1
  sudo ldconfig
  echo -ne "\r${COLOR_PRINT_GREEN}Updating Mongo C Driver${END_COLOR_PRINT}"
  echo
}

function update_nodejs()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating NodeJS${END_COLOR_PRINT}"
  cd "${INSTALLATION_DIR}"
  sudo rm -r node-*
  wget -q ${NODEJS_URL}
  tar -xf node*.tar.xz >> /dev/null 2>&1
  sudo rm node*.tar.xz >> /dev/null 2>&1
  NODEJS_DIR=$(sudo find / -type d -name "node-*-linux-x64")/
  sudo sed '/node-v/d' -i /root/.profile
  sudo sed '/PATH=\/bin:/d' -i /root/.profile
  sudo sed '/^[[:space:]]*$/d' -i /root/.profile
  sudo echo -ne "\nexport PATH=${NODEJS_DIR}bin:" >> /root/.profile 
  sudo echo -ne '$PATH' >> /root/.profile
  . /root/.profile
  echo -ne "\r${COLOR_PRINT_GREEN}Updating NodeJS${END_COLOR_PRINT}"
  echo
}



function update_npm()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating NPM${END_COLOR_PRINT}"
  npm install -g npm >> /dev/null 2>&1
  echo -ne "\r${COLOR_PRINT_GREEN}Updating NPM${END_COLOR_PRINT}"
  echo
}



function update()
{
  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                    Updating XCASH_DPOPS                    ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo

  # Get the current version of the dependencies
  get_dependencies_current_version

  stop_processes

  # Update the package list
  update_packages_list

  # Update all system packages that are XCASH_DPOPS dependencies
  update_packages

  # Update all repositories
  update_xcash
  update_xcash_dpops
  update_shared_delegates_website

  # Update all dependencies
  if [ ! "$MONGODB_CURRENT_VERSION" == "$MONGODB_LATEST_VERSION" ]; then
    update_mongodb
  else
    echo -e "${COLOR_PRINT_GREEN}MongoDB Is Already Up To Date${END_COLOR_PRINT}"
  fi
  if [ ! "$MONGOC_DRIVER_CURRENT_VERSION" == "$MONGOC_DRIVER_LATEST_VERSION" ]; then
    update_mongoc_driver
  else
    echo -e "${COLOR_PRINT_GREEN}Mongo C Driver Is Already Up To Date${END_COLOR_PRINT}"
  fi
  if [ ! "$NODEJS_CURRENT_VERSION" == "$NODEJS_LATEST_VERSION" ]; then
    update_nodejs
  else
    echo -e "${COLOR_PRINT_GREEN}NodeJS Is Already Up To Date${END_COLOR_PRINT}"
  fi
  update_npm

  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}          Update Has Completed Successfully  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo
  echo -e "${COLOR_PRINT_YELLOW}Make sure to run source ~/.profile in your terminal${END_COLOR_PRINT}"
}










function stop_programs()
{
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}              Stopping XCASH DPOPS Programs                 ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo

  stop_processes
}










function start_programs()
{
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}              Restarting XCASH DPOPS Programs                 ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo
  start_programs_wallet_password=""
  start_programs_shared_delegate_settings=""
  start_programs_color_output=""
  DPOPS_MINIMUM_AMOUNT=""
  DPOPS_FEE=""

  while
    echo -ne "${COLOR_PRINT_YELLOW}Color Output For Logs (YES or NO): ${END_COLOR_PRINT}"
    read -r start_programs_color_output
    echo -ne "\r"
    echo
    [ ! "${start_programs_color_output^^}" == "YES" ] && [ ! "${start_programs_color_output^^}" == "NO" ]
  do true; done

  while
    echo -ne "${COLOR_PRINT_YELLOW}Shared Delegate (YES or NO): ${END_COLOR_PRINT}"
    read -r start_programs_shared_delegate_settings
    echo -ne "\r"
    echo
    [ ! "${start_programs_shared_delegate_settings^^}" == "YES" ] && [ ! "${start_programs_shared_delegate_settings^^}" == "NO" ]
  do true; done

  if [ "${start_programs_shared_delegate_settings^^}" == "YES" ]; then
    while
      echo -ne "${COLOR_PRINT_YELLOW}Shared Delegate Fee (in percentage ex: 1 or 1.5 etc): ${END_COLOR_PRINT}"
      read -r DPOPS_FEE
      echo -ne "\r"
      echo
      [[ ! $DPOPS_FEE =~ $regex_DPOPS_FEE ]]
    do true; done
    
    while
      echo -ne "${COLOR_PRINT_YELLOW}Shared Delegate Minimum Payment Amount, minimum is 10K, maximum is 10M (ex: 10000 in whole numbers and not atomic units etc): ${END_COLOR_PRINT}"
      read -r DPOPS_MINIMUM_AMOUNT
      echo -ne "\r"
      echo
      [[ ! $DPOPS_MINIMUM_AMOUNT =~ $regex_DPOPS_MINIMUM_AMOUNT ]]
    do true; done
  fi

  echo -ne "${COLOR_PRINT_YELLOW}Wallet Password: ${END_COLOR_PRINT}"
  read -r start_programs_wallet_password
  echo -ne "\r"
  echo

  echo -ne "${COLOR_PRINT_YELLOW}Block Verifiers Secret Key: ${END_COLOR_PRINT}"
  read -r BLOCK_VERIFIER_SECRET_KEY
  echo -ne "\r"
  echo

  stop_processes

  echo -ne "${COLOR_PRINT_YELLOW}Starting Programs${END_COLOR_PRINT}"

  screen -dmS MongoDB /root/x-network/mongodb-*/bin/mongod --logpath ${LOGS_DIR}MongoDB_log.txt --logappend
  screen -dmS XCASH_Daemon ${XCASH_INSTALLATION_DIR}build/release/bin/xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind --log-file ${LOGS_DIR}XCASH_Daemon_log.txt --max-log-file-size 0
  sleep 30s
  screen -dmS XCASH_Wallet ${XCASH_INSTALLATION_DIR}build/release/bin/xcash-wallet-rpc --wallet-file ${XCASH_WALLETS_INSTALLATION_DIR}XCASH_DPOPS_WALLET --password "${start_programs_wallet_password}" --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon
  sleep 30s

  if [ "${start_programs_color_output^^}" == "YES" ]; then
    if [ "${start_programs_shared_delegate_settings^^}" == "YES" ]; then
      screen -dmS XCASH_DPOPS ${XCASH_DPOPS_INSTALLATION_DIR}build/XCASH_DPOPS --block_verifiers_secret_key ${BLOCK_VERIFIER_SECRET_KEY} --log_file_color ${LOGS_DIR}XCASH_DPOPS_log.txt --shared_delegates_website --fee "${DPOPS_FEE}" --minimum_amount "${DPOPS_MINIMUM_AMOUNT}"
    else
      screen -dmS XCASH_DPOPS ${XCASH_DPOPS_INSTALLATION_DIR}build/XCASH_DPOPS --block_verifiers_secret_key ${BLOCK_VERIFIER_SECRET_KEY} --log_file_color ${LOGS_DIR}XCASH_DPOPS_log.txt
    fi
  else
    if [ "${start_programs_shared_delegate_settings^^}" == "YES" ]; then
      screen -dmS XCASH_DPOPS ${XCASH_DPOPS_INSTALLATION_DIR}build/XCASH_DPOPS --block_verifiers_secret_key ${BLOCK_VERIFIER_SECRET_KEY} --log_file ${LOGS_DIR}XCASH_DPOPS_log.txt --shared_delegates_website --fee "${DPOPS_FEE}" --minimum_amount "${DPOPS_MINIMUM_AMOUNT}"
    else
      screen -dmS XCASH_DPOPS ${XCASH_DPOPS_INSTALLATION_DIR}build/XCASH_DPOPS --block_verifiers_secret_key ${BLOCK_VERIFIER_SECRET_KEY} --log_file ${LOGS_DIR}XCASH_DPOPS_log.txt
    fi
  fi 

  sleep 10s 
  
  # Check if all of the programs have started
  data=$(ps -eaf)
  if [[ ! $data =~ "SCREEN -dmS MongoDB" ]]; then
    echo -ne "\r${COLOR_PRINT_RED}Starting Programs${END_COLOR_PRINT}"
    echo -e "\n${COLOR_PRINT_RED}Could not start MongoDB${END_COLOR_PRINT}"
    stop_processes
    exit
  fi
  if [[ ! $data =~ "SCREEN -dmS XCASH_Daemon" ]]; then
    echo -ne "\r${COLOR_PRINT_RED}Starting Programs${END_COLOR_PRINT}"
    echo -e "\n${COLOR_PRINT_RED}Could not start XCASH_Daemon${END_COLOR_PRINT}"
    stop_processes
    exit
  fi
  if [[ ! $data =~ "SCREEN -dmS XCASH_Wallet" ]]; then
    echo -ne "\r${COLOR_PRINT_RED}Starting Programs${END_COLOR_PRINT}"
    echo -e "\n${COLOR_PRINT_RED}Could not start XCASH_Wallet${END_COLOR_PRINT}"
    stop_processes
    exit
  fi
  if [[ ! $data =~ "SCREEN -dmS XCASH_DPOPS" ]]; then
    echo -ne "\r${COLOR_PRINT_RED}Starting Programs${END_COLOR_PRINT}"
    echo -e "\n${COLOR_PRINT_RED}Could not start XCASH_DPOPS${END_COLOR_PRINT}"
    stop_processes
    exit
  fi

  # start the autorestarter script
  chmod +x ${XCASH_DPOPS_INSTALLATION_DIR}scripts/autoinstaller/docker_auto_restart.sh
  screen -dmS XCASH_DPOPS_AUTO_RESTART ${XCASH_DPOPS_INSTALLATION_DIR}scripts/autoinstaller/docker_auto_restart.sh
  sleep 10s

  echo -ne "\r${COLOR_PRINT_GREEN}Starting Programs${END_COLOR_PRINT}"
  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}          Restarted XCASH DPOPS Programs Successfully         ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo
  echo -e "${COLOR_PRINT_YELLOW}To view the log file\ntail -f -n 100 ${LOGS_DIR}XCASH_DPOPS_log.txt${END_COLOR_PRINT}"
}










function main()
{
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}          XCASH DPOPS Docker Configuration  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo
  while
    echo -ne "${COLOR_PRINT_YELLOW}(C)onfigure, (U)pdate, (R)estart Programs, (S)top Programs: ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    echo
    [ ! "${data^^}" == "C" ] && [ ! "${data^^}" == "U" ] &&  [ ! "${data^^}" == "R" ] && [ ! "${data^^}" == "S" ]
  do true; done
  if [ "${data^^}" == "C" ]; then
    configuration
    update
  elif [ "${data^^}" == "U" ]; then
    update
  elif [ "${data^^}" == "R" ]; then
    start_programs
  elif [ "${data^^}" == "S" ]; then
    stop_programs
  fi  
}



main