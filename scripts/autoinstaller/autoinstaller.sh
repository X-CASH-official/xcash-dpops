#!/bin/bash

# Set the script to exit if any command fails
set -e

# Color print variables
COLOR_PRINT_RED="\033[1;31m"
COLOR_PRINT_GREEN="\033[1;32m"
COLOR_PRINT_YELLOW="\033[1;33m"
END_COLOR_PRINT="\033[0m"

# Configuration settings
INSTALLATION_TYPE_SETTINGS=1
INSTALLATION_TYPE=""
XCASH_DPOPS_INSTALLATION_DIR="$HOME/xcash-official/"
XCASH_BLOCKCHAIN_INSTALLATION_DIR="$HOME/.X-CASH/"
MONGODB_INSTALLATION_DIR="/data/db/"
SHARED_DELEGATE="YES"
WALLET_SETTINGS="YES"
AUTOSTART_SETTINGS="NO"
WALLET_SEED=""
WALLET_PASSWORD=$(< /dev/urandom tr -dc 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789' | head -c"${1:-32}";echo;)
BLOCK_VERIFIER_KEY_SETTINGS=""
BLOCK_VERIFIER_SECRET_KEY=""
BLOCK_VERIFIER_PUBLIC_KEY=""
BLOCK_VERIFIERS_SECRET_KEY_LENGTH=128
BLOCK_VERIFIERS_PUBLIC_KEY_LENGTH=64
DPOPS_FEE=0
DPOPS_MINIMUM_AMOUNT=0
# XCASH_DPOPS_BLOCK_HEIGHT=521850
XCASH_DPOPS_BLOCK_HEIGHT=640000

# Latest versions
MONGODB_LATEST_VERSION="mongodb-linux-x86_64-ubuntu1804-4.4.1"
MONGOC_DRIVER_LATEST_VERSION="mongo-c-driver-1.17.0"
NODEJS_LATEST_VERSION="node-v14.10.1-linux-x64"

# Settings
XCASH_URL="https://github.com/X-CASH-official/xcash-core.git"
XCASH_CORE_BRANCH="xcash_proof_of_stake"
XCASH_BLOCKCHAIN_BOOTSTRAP_URL="http://94.130.59.172/xcash-blockchain.7z"
XCASH_DIR=""
XCASH_WALLET_DIR=""
XCASH_SYSTEMPID_DIR=""
XCASH_LOGS_DIR=""
XCASH_DPOPS_URL="https://github.com/X-CASH-official/xcash-dpops.git"
XCASH_DPOPS_DIR=""
XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR=""
SHARED_DELEGATES_WEBSITE_URL="https://github.com/X-CASH-official/delegates-pool-website.git"
SHARED_DELEGATES_WEBSITE_DIR=""
SSH_PORT_NUMBER=22
NODEJS_URL="https://nodejs.org/dist/${NODEJS_LATEST_VERSION:5:8}/${NODEJS_LATEST_VERSION}.tar.xz"
NODEJS_DIR=""
NODEJS_CURRENT_VERSION=""
MONGODB_URL="http://fastdl.mongodb.org/linux/${MONGODB_LATEST_VERSION}.tgz"
MONGODB_DIR=""
MONGODB_CURRENT_VERSION=""
MONGOC_DRIVER_URL="https://github.com/mongodb/mongo-c-driver/releases/download/${MONGOC_DRIVER_LATEST_VERSION:15}/${MONGOC_DRIVER_LATEST_VERSION}.tar.gz"
MONGOC_DRIVER_DIR=""
MONGOC_DRIVER_CURRENT_VERSION=""
XCASH_DPOPS_PACKAGES="build-essential cmake pkg-config libboost-all-dev libssl-dev libzmq3-dev libunbound-dev libsodium-dev libminiupnpc-dev libunwind8-dev liblzma-dev libreadline6-dev libldns-dev libexpat1-dev libgtest-dev doxygen graphviz libpcsclite-dev git screen p7zip-full moreutils wget iptables"
CURRENT_XCASH_WALLET_INFORMATION=""
PUBLIC_ADDRESS=""

# Files
FIREWALL=""
FIREWALL_SHARED_DELEGATES=""
SYSTEMD_SERVICE_FILE_FIREWALL=""
SYSTEMD_SERVICE_FILE_MONGODB=""
SYSTEMD_SERVICE_FILE_XCASH_DAEMON=""
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE=""
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE=""
SYSTEMD_SERVICE_FILE_XCASH_WALLET=""
SYSTEMD_TIMER_FILE_XCASH_DPOPS=""
SYSTEMD_TIMER_FILE_XCASH_WALLET=""

# File URLs
FIREWALL_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/firewall/firewall_script.sh"
FIREWALL_SHARED_DELEGATES_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/firewall/firewall_shared_delegates_script.sh"
FIREWALL_XCASH_NODE_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/firewall/firewall_script_xcash_node.sh"
SYSTEMD_SERVICE_FILE_FIREWALL_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/systemd/firewall.service"
SYSTEMD_SERVICE_FILE_MONGODB_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/systemd/MongoDB.service"
SYSTEMD_SERVICE_FILE_XCASH_DAEMON_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/systemd/xcash-daemon.service"
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/systemd/xcash-dpops.service"
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/systemd/xcash-dpops-shared-delegate.service"
SYSTEMD_SERVICE_FILE_XCASH_WALLET_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/systemd/xcash-rpc-wallet.service"
SYSTEMD_TIMER_FILE_XCASH_DPOPS_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/systemd/xcash-dpops.timer"
SYSTEMD_TIMER_FILE_XCASH_WALLET_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/systemd/xcash-rpc-wallet.timer"

# System settings
CPU_THREADS=$(nproc)
DEFAULT_NETWORK_DEVICE=$(ip route | grep default | sed -e "s/^.*dev.//" -e "s/.proto.*//")
RAM=$(awk '/MemTotal/ { printf "%1.f \n", $2/1024/1024 }' /proc/meminfo)
RAM_CPU_RATIO=$((RAM / CPU_THREADS))
RAM_CPU_RATIO_ALL_CPU_THREADS=4

# Regex
regex_XCASH_DPOPS_INSTALLATION_DIR="(^\/(.*?)\/$)|(^$)" # anything that starts with / and ends with / and does not contain a space
regex_MNEMONIC_SEED="^\b([a-z]+\s+){24}\b([a-z]+)$" # 25 words exactly
regex_DPOPS_FEE="\b(^[1-9]{1}[0-9]{0,1}.?[0-9]{0,6}$)\b$" # between 1 and 99 with up to 6 decimal places
regex_DPOPS_MINIMUM_AMOUNT="\b(^[1-9]{1}[0-9]{4,6}$)\b$" # between 10000 and 10000000-1



# Disable script execution with sudo and warns the user if root install
if [ $SUDO_USER ] ; then
  # execution with sudo not allowed
  echo -e "\n${COLOR_PRINT_RED}Please don't use sudo with this script!${END_COLOR_PRINT}"
  exit 1
else
  # If not inside a LXC container (isolated, with official LXC installer) 
  if [ ! "$container" == "lxc" ]; then
    if [ $UID -eq 0 ] ; then
      echo -e "\n${COLOR_PRINT_RED}WARNING: You are running the script with a root user! This is NOT secure and NOT suggested. Please use a dedicated xcash user following the documentation. If you want to proceed with root and you know what you are doing press ENTER, otherwise press CTRL+C to exit.${END_COLOR_PRINT}"
      read -r data
      echo -ne "\r"
      echo
      if [ ! "${data}" == "" ]; then
        exit 1
      fi
    fi
  fi
fi



# Functions

# Sed used when modifying service files (if used inside containers use sponge utility - moreutils package)
function sed_services()
{
  if [ "$container" == "lxc" ]; then
    cat $2 | command sed "$1" | sponge $2
  else
    sudo sed -i "$1" $2
  fi
}


function get_installation_settings()
{
  echo -ne "${COLOR_PRINT_GREEN}X-Cash DPoPS Installation Settings\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_YELLOW}1 = Install\n2 = Update\n3 = Uninstall\n\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_GREEN}X-Cash Node (Daemon Only) Installation Settings\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_YELLOW}4 = Install\n5 = Update\n6 = Uninstall\n\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_GREEN}X-Cash Blockchain Management\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_YELLOW}7 = Install / Update Blockchain\n\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_GREEN}X-Cash DPoPS Delegate Configuration\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_YELLOW}8 = Change Solo Delegate or Shared Delegate\n9 = Edit Shared Delegate Settings\n10 = Register / Update Delegate\n\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_GREEN}X-Cash DPoPS Delegate Management\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_YELLOW}11 = Restart Programs\n12 = Stop Programs\n\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_GREEN}X-Cash DPoPS Testing\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_YELLOW}13 = Test Update\n14 = Test Update Reset Delegates\n\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_GREEN}Miscellaneous\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_YELLOW}15 = Configure Installation\n16 = Firewall\n17 = Shared Delegates Firewall\n\n${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_GREEN}Enter the number of the chosen option (default 1): ${END_COLOR_PRINT}"
  read -r data
  INSTALLATION_TYPE_SETTINGS=$([ "$data" == "2" ] || [ "$data" == "3" ] || [ "$data" == "4" ] || [ "$data" == "5" ] || [ "$data" == "6" ] || [ "$data" == "7" ] || [ "$data" == "8" ] || [ "$data" == "9" ] || [ "$data" == "10" ] || [ "$data" == "11" ] || [ "$data" == "12" ] || [ "$data" == "13" ] || [ "$data" == "14" ] || [ "$data" == "15" ] || [ "$data" == "16" ] || [ "$data" == "17" ] && echo "$data" || echo "1")
  echo -ne "\r"
  # Check if xcash-dpops is already installed, if the user choose to install
  if [ "$INSTALLATION_TYPE_SETTINGS" -eq "1" ]; then
    echo -ne "${COLOR_PRINT_YELLOW}Checking if xcash-dpops is already installed${END_COLOR_PRINT}"
    data=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "xcash-dpops" -print | wc -l)
    if [ "$data" -ne "0" ]; then
      echo -e "\n${COLOR_PRINT_RED}xcash-dpops is already installed. You can either update or uninstall${END_COLOR_PRINT}"
      exit 1
    fi
    echo -ne "\r                                                    "
    echo
  fi

  # Check if xcash-dpops is not installed, and if the user choose an option where xcash-dpops needed to be installed
  if [ "$INSTALLATION_TYPE_SETTINGS" -eq "2" ] || [ "$INSTALLATION_TYPE_SETTINGS" -eq "3" ] || [ "$INSTALLATION_TYPE_SETTINGS" -eq "8" ] || [ "$INSTALLATION_TYPE_SETTINGS" -eq "9" ] || [ "$INSTALLATION_TYPE_SETTINGS" -eq "10" ] || [ "$INSTALLATION_TYPE_SETTINGS" -eq "11" ] || [ "$INSTALLATION_TYPE_SETTINGS" -eq "12" ] || [ "$INSTALLATION_TYPE_SETTINGS" -eq "13" ] || [ "$INSTALLATION_TYPE_SETTINGS" -eq "14" ] || [ "$INSTALLATION_TYPE_SETTINGS" -eq "15" ]; then
    data=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "xcash-dpops" -print | wc -l)
    if [ "$data" -eq "0" ]; then
      echo -e "\n${COLOR_PRINT_RED}This is an invalid option since xcash-dpops is not installed${END_COLOR_PRINT}"
      exit 1
    fi
    echo -ne "\r                                                     "
    echo
  fi
}

function get_xcash_dpops_installation_directory()
{
  while
    echo -ne "${COLOR_PRINT_YELLOW}Installation Directory, must be in the form of /directory/ (leave empty for default: $HOME/xcash-official/): ${END_COLOR_PRINT}"
    read -r data    
    echo -ne "\r"
    echo
    [[ ! $data =~ $regex_XCASH_DPOPS_INSTALLATION_DIR ]]
  do true; done
  
  XCASH_DPOPS_INSTALLATION_DIR=$([ "$data" == "" ] && echo "$XCASH_DPOPS_INSTALLATION_DIR" || echo "$data")
}

function get_xcash_blockchain_xcash_dpops_installation_directory()
{
  while
    echo -ne "${COLOR_PRINT_YELLOW}X-CASH Blockchain Installation Directory, must be in the form of /directory/ (leave empty for default: $HOME/.X-CASH/): ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    echo
    [[ ! $data =~ $regex_XCASH_DPOPS_INSTALLATION_DIR ]]
  do true; done

  XCASH_BLOCKCHAIN_INSTALLATION_DIR=$([ "$data" == "" ] && echo "$XCASH_BLOCKCHAIN_INSTALLATION_DIR" || echo "$data")
}

function get_mongodb_installation_directory()
{
  while
    echo -ne "${COLOR_PRINT_YELLOW}MongoDB Installation Directory, must be in the form of /directory/ (leave empty for default: /data/db/): ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    echo
    [[ ! $data =~ $regex_XCASH_DPOPS_INSTALLATION_DIR ]]
  do true; done

  MONGODB_INSTALLATION_DIR=$([ "$data" == "" ] && echo "$MONGODB_INSTALLATION_DIR" || echo "$data")
}

function update_global_variables()
{
  XCASH_DIR=${XCASH_DPOPS_INSTALLATION_DIR}xcash-core/
  XCASH_WALLET_DIR=${XCASH_DPOPS_INSTALLATION_DIR}xcash-wallets/
  XCASH_SYSTEMPID_DIR=${XCASH_DPOPS_INSTALLATION_DIR}systemdpid/
  XCASH_LOGS_DIR=${XCASH_DPOPS_INSTALLATION_DIR}logs/
  XCASH_DPOPS_DIR=${XCASH_DPOPS_INSTALLATION_DIR}xcash-dpops/
  XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR=${XCASH_DPOPS_DIR}delegates-pool-website/
  SHARED_DELEGATES_WEBSITE_DIR=${XCASH_DPOPS_INSTALLATION_DIR}delegates-pool-website/
  NODEJS_DIR=${XCASH_DPOPS_INSTALLATION_DIR}${NODEJS_LATEST_VERSION}/
  MONGODB_DIR=${XCASH_DPOPS_INSTALLATION_DIR}${MONGODB_LATEST_VERSION}/
}

function get_shared_delegate_installation_settings()
{
  echo -ne "${COLOR_PRINT_YELLOW}Shared Delegate (YES): ${END_COLOR_PRINT}"
  read -r data
  SHARED_DELEGATE=$([ "$data" == "" ] && echo "$SHARED_DELEGATE" || echo "$data")
  echo -ne "\r"
  echo
  if [ "${SHARED_DELEGATE^^}" == "YES" ]; then    
    while
      echo -ne "${COLOR_PRINT_YELLOW}Shared Delegate Fee (expressed as a percentage, ex: 1 or 1.52): ${END_COLOR_PRINT}"
      read -r DPOPS_FEE
      echo -ne "\r"
      echo
      [[ ! $DPOPS_FEE =~ $regex_DPOPS_FEE ]]
    do true; done
    
    while
      echo -ne "${COLOR_PRINT_YELLOW}Shared Delegate Minimum Payment Amount (whole number between 10000 and 10000000): ${END_COLOR_PRINT}"
      read -r DPOPS_MINIMUM_AMOUNT
      echo -ne "\r"
      echo
      [[ ! $DPOPS_MINIMUM_AMOUNT =~ $regex_DPOPS_MINIMUM_AMOUNT ]]
    do true; done
  fi
}

function update_systemd_service_files()
{
# Files
FIREWALL=$(cat <(curl -sSL $FIREWALL_URL))
FIREWALL="${FIREWALL//'${SSH_PORT_NUMBER}'/$SSH_PORT_NUMBER}"

FIREWALL_SHARED_DELEGATES=$(cat <(curl -sSL $FIREWALL_SHARED_DELEGATES_URL))
FIREWALL_SHARED_DELEGATES="${FIREWALL_SHARED_DELEGATES//'${SSH_PORT_NUMBER}'/$SSH_PORT_NUMBER}"
FIREWALL_SHARED_DELEGATES="${FIREWALL_SHARED_DELEGATES//'${DEFAULT_NETWORK_DEVICE}'/$DEFAULT_NETWORK_DEVICE}"

SYSTEMD_SERVICE_FILE_FIREWALL=$(cat <(curl -sSL $SYSTEMD_SERVICE_FILE_FIREWALL_URL))
SYSTEMD_SERVICE_FILE_FIREWALL="${SYSTEMD_SERVICE_FILE_FIREWALL//'${HOME}'/$HOME}"

SYSTEMD_SERVICE_FILE_MONGODB=$(cat <(curl -sSL $SYSTEMD_SERVICE_FILE_MONGODB_URL))
SYSTEMD_SERVICE_FILE_MONGODB="${SYSTEMD_SERVICE_FILE_MONGODB//'${USER}'/$USER}"
SYSTEMD_SERVICE_FILE_MONGODB="${SYSTEMD_SERVICE_FILE_MONGODB//'${XCASH_DPOPS_INSTALLATION_DIR}'/$XCASH_DPOPS_INSTALLATION_DIR}"
SYSTEMD_SERVICE_FILE_MONGODB="${SYSTEMD_SERVICE_FILE_MONGODB//'${MONGODB_DIR}'/$MONGODB_DIR}"
SYSTEMD_SERVICE_FILE_MONGODB="${SYSTEMD_SERVICE_FILE_MONGODB//'${MONGODB_INSTALLATION_DIR}'/$MONGODB_INSTALLATION_DIR}"

SYSTEMD_SERVICE_FILE_XCASH_DAEMON=$(cat <(curl -sSL $SYSTEMD_SERVICE_FILE_XCASH_DAEMON_URL))
SYSTEMD_SERVICE_FILE_XCASH_DAEMON="${SYSTEMD_SERVICE_FILE_XCASH_DAEMON//'${USER}'/$USER}"
SYSTEMD_SERVICE_FILE_XCASH_DAEMON="${SYSTEMD_SERVICE_FILE_XCASH_DAEMON//'${XCASH_DPOPS_INSTALLATION_DIR}'/$XCASH_DPOPS_INSTALLATION_DIR}"
SYSTEMD_SERVICE_FILE_XCASH_DAEMON="${SYSTEMD_SERVICE_FILE_XCASH_DAEMON//'${XCASH_DIR}'/$XCASH_DIR}"
SYSTEMD_SERVICE_FILE_XCASH_DAEMON="${SYSTEMD_SERVICE_FILE_XCASH_DAEMON//'${XCASH_BLOCKCHAIN_INSTALLATION_DIR}'/$XCASH_BLOCKCHAIN_INSTALLATION_DIR}"
SYSTEMD_SERVICE_FILE_XCASH_DAEMON="${SYSTEMD_SERVICE_FILE_XCASH_DAEMON//'${XCASH_LOGS_DIR}'/$XCASH_LOGS_DIR}"
SYSTEMD_SERVICE_FILE_XCASH_DAEMON="${SYSTEMD_SERVICE_FILE_XCASH_DAEMON//'${XCASH_SYSTEMPID_DIR}'/$XCASH_SYSTEMPID_DIR}"

SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE=$(cat <(curl -sSL $SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE_URL))
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE="${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE//'${USER}'/$USER}"
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE="${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE//'${XCASH_DPOPS_DIR}'/$XCASH_DPOPS_DIR}"
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE="${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE//'${BLOCK_VERIFIER_SECRET_KEY}'/$BLOCK_VERIFIER_SECRET_KEY}"

SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE=$(cat <(curl -sSL $SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE_URL))
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE="${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE//'${USER}'/$USER}"
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE="${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE//'${XCASH_DPOPS_DIR}'/$XCASH_DPOPS_DIR}"
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE="${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE//'${BLOCK_VERIFIER_SECRET_KEY}'/$BLOCK_VERIFIER_SECRET_KEY}"
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE="${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE//'${BLOCK_VERIFIER_SECRET_KEY}'/$BLOCK_VERIFIER_SECRET_KEY}"
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE="${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE//'${DPOPS_FEE}'/$DPOPS_FEE}"
SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE="${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE//'${DPOPS_MINIMUM_AMOUNT}'/$DPOPS_MINIMUM_AMOUNT}"

SYSTEMD_TIMER_FILE_XCASH_DPOPS=$(cat <(curl -sSL $SYSTEMD_TIMER_FILE_XCASH_DPOPS_URL))

SYSTEMD_SERVICE_FILE_XCASH_WALLET=$(cat <(curl -sSL $SYSTEMD_SERVICE_FILE_XCASH_WALLET_URL))
SYSTEMD_SERVICE_FILE_XCASH_WALLET="${SYSTEMD_SERVICE_FILE_XCASH_WALLET//'${USER}'/$USER}"
SYSTEMD_SERVICE_FILE_XCASH_WALLET="${SYSTEMD_SERVICE_FILE_XCASH_WALLET//'${XCASH_DIR}'/$XCASH_DIR}"
SYSTEMD_SERVICE_FILE_XCASH_WALLET="${SYSTEMD_SERVICE_FILE_XCASH_WALLET//'${XCASH_WALLET_DIR}'/$XCASH_WALLET_DIR}"
SYSTEMD_SERVICE_FILE_XCASH_WALLET="${SYSTEMD_SERVICE_FILE_XCASH_WALLET//'${WALLET_PASSWORD}'/$WALLET_PASSWORD}"
SYSTEMD_SERVICE_FILE_XCASH_WALLET="${SYSTEMD_SERVICE_FILE_XCASH_WALLET//'${XCASH_LOGS_DIR}'/$XCASH_LOGS_DIR}"

SYSTEMD_TIMER_FILE_XCASH_WALLET=$(cat <(curl -sSL $SYSTEMD_TIMER_FILE_XCASH_WALLET_URL))

}

function setup_lxc_container_profile()
{
  if [ "$container" == "lxc" ]; then
    sudo sed '/mesg n/d' -i "${HOME}"/.profile
    sudo sed -e :a -e '/^\n*$/{$d;N;};/\n$/ba' -i "${HOME}"/.profile
    source ~/.profile || true
  fi
}

function get_wallet_settings()
{
  echo -ne "${COLOR_PRINT_YELLOW}Create a New Wallet to Collect Block Rewards? Leave empty for YES, write N for NO: ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  WALLET_SETTINGS=$([ "$data" == "" ] && echo "$WALLET_SETTINGS" || echo "NO")
  if [ "$WALLET_SETTINGS" == "NO" ]; then
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
  echo -ne "${COLOR_PRINT_YELLOW}Generate a New Password? Leave empty for YES, write N for NO: ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  WALLET_PASSWORD=$([ "$data" == "" ] && echo "$WALLET_PASSWORD" || echo "NO")
  if [ "$WALLET_PASSWORD" == "NO" ]; then
  echo -ne "${COLOR_PRINT_YELLOW}Enter the Custom Password: ${END_COLOR_PRINT}"
  read -r WALLET_PASSWORD
  echo -ne "\r"
  echo
  fi
}

function get_password()
{
  # ask for the root password if not root so it wont ask again when installing
  if [ "$EUID" -ne 0 ]; then
    sudo echo
  fi
}

function relogin_user()
{
  # This will force profile reload for the current user shell
  get_password
  exec sudo su -l $USER
}


function get_block_verifier_key_settings()
{
  while
    echo -ne "${COLOR_PRINT_YELLOW}Block Verifier Key: (I)mport or (C)reate new: ${END_COLOR_PRINT}"
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
      echo -ne "${COLOR_PRINT_YELLOW}Enter Block Verifiers Secret Key: ${END_COLOR_PRINT}"
      read -r BLOCK_VERIFIER_SECRET_KEY
      echo -ne "\r"
      echo
      [[ ! ${#BLOCK_VERIFIER_SECRET_KEY} -eq $BLOCK_VERIFIERS_SECRET_KEY_LENGTH ]]
    do true; done
    BLOCK_VERIFIER_PUBLIC_KEY="${BLOCK_VERIFIER_SECRET_KEY: -${BLOCK_VERIFIERS_PUBLIC_KEY_LENGTH}}"
  fi
}

function get_autostart_services_settings()
{
  echo -ne "${COLOR_PRINT_YELLOW}Do you want to autostart the services when you restart the server? (leave empty for default: NO): ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  AUTOSTART_SETTINGS=$([ "$data" == "" ] && echo "$AUTOSTART_SETTINGS" || echo "YES")

}

function print_installation_settings()
{
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}             Installation/Configuration Settings${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo -e "${COLOR_PRINT_GREEN}Installation Type: Installation${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}Installation Directory: ${XCASH_DPOPS_INSTALLATION_DIR} ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}X-CASH Blockchain Installation Directory: ${XCASH_BLOCKCHAIN_INSTALLATION_DIR} ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}MongoDB Installation Directory: ${MONGODB_INSTALLATION_DIR} ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}Shared Delegate: ${SHARED_DELEGATE} ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}Create New Wallet: ${WALLET_SETTINGS} ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}Wallet Password: ${WALLET_PASSWORD} ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}User: ${USER} ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}DPOPS Fee: ${DPOPS_FEE} ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}DPOPS Minimum Payment Amount: ${DPOPS_MINIMUM_AMOUNT} ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}Autostart services when reboot: ${AUTOSTART_SETTINGS} ${END_COLOR_PRINT}"
  if [ ! "$container" == "lxc" ]; then
    echo -e "${COLOR_PRINT_GREEN}SSH Port (used to configure the firewall): ${SSH_PORT_NUMBER} ${END_COLOR_PRINT}"
  fi

  seconds=10
  while [ "$seconds" -ne 0 ]
  do
    echo -ne "${COLOR_PRINT_GREEN}${INSTALLATION_TYPE} will start in ${seconds} seconds, press Ctrl + C to cancel!${END_COLOR_PRINT}"
    seconds=$((seconds-1))
    sleep 1
    echo -ne "\r"
  done 
  echo -ne "${COLOR_PRINT_GREEN}${INSTALLATION_TYPE} will start in 0 seconds, press Ctrl + C to cancel!${END_COLOR_PRINT}"
  echo
  echo
}

function installation_settings()
{
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}         Welcome to X-Cash DPoPS auto-install script  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo -e "${COLOR_PRINT_GREEN}SUGGESTION: if your are running the script through SSH, it's suggested the use of a terminal multiplexer like 'byobu'. You can install it with 'sudo apt install byobu' and then enter the session with 'byobu' command. If your SSH connection crashes you can reconnect to your byobu session using the same command. Please consider running this script inside the byobu session to prevent corrupted installations due to SSH disconnection.${END_COLOR_PRINT}"
  echo
  echo -e "${COLOR_PRINT_YELLOW}The available options of the script are:${END_COLOR_PRINT}"
  echo
  get_password
  get_installation_settings
  if [ "$INSTALLATION_TYPE_SETTINGS" -eq "1" ]; then 
    get_xcash_dpops_installation_directory
    get_xcash_blockchain_xcash_dpops_installation_directory
    get_mongodb_installation_directory
    update_global_variables
    get_shared_delegate_installation_settings
    get_wallet_settings
    get_password_settings
    get_block_verifier_key_settings
    if [ "$container" == "lxc" ]; then
      echo -e "${COLOR_PRINT_YELLOW}Autostart enabled as default for services (because this is a container installation)${END_COLOR_PRINT}"
      AUTOSTART_SETTINGS="YES"
    else
      get_autostart_services_settings
      get_ssh_port
    fi
    INSTALLATION_TYPE="Installation"
    print_installation_settings
  fi
  if [ "$INSTALLATION_TYPE_SETTINGS" -eq "15" ]; then 
    echo
    echo -e "${COLOR_PRINT_RED}WARNING: Old wallet, old block verifier key and old settings (service files)${END_COLOR_PRINT}"
    echo -e "${COLOR_PRINT_RED}will be overwritten if already existent! Please make a backup if required!${END_COLOR_PRINT}"
    echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    echo
    get_installation_directory
    get_shared_delegate_installation_settings
    get_wallet_settings
    get_password_settings
    get_block_verifier_key_settings
    if [ "$container" == "lxc" ]; then
      echo -e "${COLOR_PRINT_YELLOW}Autostart enabled as default for services (because this is a container installation)${END_COLOR_PRINT}"
      AUTOSTART_SETTINGS="YES"
    else
      get_autostart_services_settings
    fi
    INSTALLATION_TYPE="Configuration"
    print_installation_settings
  fi
}





function get_current_xcash_wallet_data()
{
  echo
  echo -ne "${COLOR_PRINT_YELLOW}Refreshing and Getting Current X-CASH Wallet Data${END_COLOR_PRINT}"

  sudo systemctl stop xcash-daemon &>/dev/null
  sleep 10s
  sudo systemctl start xcash-daemon &>/dev/null
  sleep 20s

  screen -dmS XCASH_RPC_Wallet "${XCASH_DIR}"build/release/bin/xcash-wallet-rpc --wallet-file "${XCASH_WALLET_DIR}"delegate-wallet --password "${WALLET_PASSWORD}" --rpc-bind-port 18288 --confirm-external-bind --disable-rpc-login --trusted-daemon --log-file "${XCASH_LOGS_DIR}"xcash-wallet-rpc.log
  sleep 10s
  
   while
    data=$(curl -s -X POST http://127.0.0.1:18288/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"get_address"}' -H 'Content-Type: application/json') 
    sleep 10s
    [[ "$data" == "" ]]
  do true; done

  PUBLIC_ADDRESS=$(curl -s -X POST http://127.0.0.1:18288/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"get_address"}' -H 'Content-Type: application/json' | grep \"address\" | head -1 | sed s"|    \"address\": ||g" | sed s"|\"||g" | sed s"|,||g")
  SPEND_KEY=$(curl -s -X POST http://127.0.0.1:18288/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"query_key","params":{"key_type":"spend_key"}}' -H 'Content-Type: application/json' | grep \"key\" | sed s"|    \"key\": ||g" | sed s"|\"||g")
  VIEW_KEY=$(curl -s -X POST http://127.0.0.1:18288/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"query_key","params":{"key_type":"view_key"}}' -H 'Content-Type: application/json' | grep \"key\" | sed s"|    \"key\": ||g" | sed s"|\"||g")
  WALLET_SEED=$(curl -s -X POST http://127.0.0.1:18288/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"query_key","params":{"key_type":"mnemonic"}}' -H 'Content-Type: application/json' | grep \"key\" | sed s"|    \"key\": ||g" | sed s"|\"||g")
  CURRENT_XCASH_WALLET_INFORMATION="${COLOR_PRINT_GREEN}############################################################\n                 X-CASH Wallet Data  \n############################################################${END_COLOR_PRINT}\n\n${COLOR_PRINT_YELLOW}Public Address: $PUBLIC_ADDRESS\nMnemonic Seed: $WALLET_SEED\nSpend Key: $SPEND_KEY\nView Key: $VIEW_KEY\nWallet Password: $WALLET_PASSWORD\nBlock Verifiers Public Key: $BLOCK_VERIFIER_PUBLIC_KEY\nBlock Verifiers Secret Key: $BLOCK_VERIFIER_SECRET_KEY${END_COLOR_PRINT}"
  PUBLIC_ADDRESS=${PUBLIC_ADDRESS%?}

  curl -s -X POST http://127.0.0.1:18288/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"stop_wallet"}' -H 'Content-Type: application/json' &>/dev/null
  sleep 10s
  
  sudo systemctl stop xcash-daemon &>/dev/null
  sleep 10s
  
  echo -ne "\r${COLOR_PRINT_GREEN}Refreshing and Getting Current X-CASH Wallet Data${END_COLOR_PRINT}"
  echo
  echo
}

function start_systemd_service_files()
{
  echo -ne "${COLOR_PRINT_YELLOW}Starting Systemd Service Files${END_COLOR_PRINT}"
  sudo systemctl start mongodb &>/dev/null
  sudo systemctl start xcash-daemon &>/dev/null
  sleep 30s
  sudo systemctl start xcash-rpc-wallet &>/dev/null
  sleep 30s
  sudo systemctl start xcash-dpops &>/dev/null
  echo -ne "\r${COLOR_PRINT_GREEN}Starting Systemd Service Files${END_COLOR_PRINT}"
  echo
}

function stop_systemd_service_files()
{
  echo -ne "${COLOR_PRINT_YELLOW}Stopping Systemd Service Files${END_COLOR_PRINT}"
  sudo systemctl stop mongodb xcash-daemon xcash-rpc-wallet xcash-dpops &>/dev/null
  echo -ne "\r${COLOR_PRINT_GREEN}Stopping Systemd Service Files${END_COLOR_PRINT}"
  echo
}

function enable_service_files_at_startup()
{
  echo -ne "${COLOR_PRINT_YELLOW}Enabling services to autostart on reboot${END_COLOR_PRINT}"
  sudo systemctl enable mongodb.service xcash-daemon.service xcash-rpc-wallet.timer xcash-dpops.timer 2> /dev/null
  echo -ne "\r${COLOR_PRINT_GREEN}Enabling services to autostart on reboot${END_COLOR_PRINT}"
}








function check_if_solo_node()
{
  echo -ne "${COLOR_PRINT_YELLOW}Checking If Solo Node${END_COLOR_PRINT}"
  data=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "delegates-pool-website" -print | wc -l)
  if [ "$data" -gt 0 ]; then
    SHARED_DELEGATE="YES"
  else
    SHARED_DELEGATE="NO"
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Checking If Solo Node${END_COLOR_PRINT}"
  echo
}

function check_if_upgrade_solo_delegate_and_shared_delegate()
{
  # get the block verifiers secret key from the systemd service file
  BLOCK_VERIFIER_SECRET_KEY=$(cat /lib/systemd/system/xcash-dpops.service)
  BLOCK_VERIFIER_SECRET_KEY=$(echo $BLOCK_VERIFIER_SECRET_KEY | awk -F '--block-verifiers-secret-key' '{print $2}')
  BLOCK_VERIFIER_SECRET_KEY=${BLOCK_VERIFIER_SECRET_KEY:1:$BLOCK_VERIFIERS_SECRET_KEY_LENGTH}

  if [ "${SHARED_DELEGATE^^}" == "YES" ]; then
    echo -ne "The current delegate setting is shared delegate. If you would like to change the settings to a solo delegate type \"YES\" otherwise press enter:"
    read -r data
    echo -ne "\r"
    echo
    if [ "${data^^}" == "YES" ]; then
      SHARED_DELEGATE="NO"
      uninstall_shared_delegates_website
      update_systemd_service_files
      sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE}' > /lib/systemd/system/xcash-dpops.service"
      sed_services 's/\r$//g' /lib/systemd/system/xcash-dpops.service
      sudo systemctl daemon-reload
      sudo sed '/node-v/d' -i "${HOME}"/.profile
      sudo sed '/PATH=\/bin:/d' -i "${HOME}"/.profile
      sudo sed '/^[[:space:]]*$/d' -i "${HOME}"/.profile
      sudo sed -e :a -e '/^\n*$/{$d;N;};/\n$/ba' -i "${HOME}"/.profile
      source ~/.profile || true
      get_installation_directory
      get_dependencies_current_version
    fi
  else
    echo -ne "The current delegate setting is solo delegate. If you would like to change the settings to a shared delegate type \"YES\" otherwise press enter:"
    read -r data
    echo -ne "\r"
    echo
    if [ "${data^^}" == "YES" ]; then
      SHARED_DELEGATE="YES"

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

      NODEJS_DIR=${XCASH_DPOPS_INSTALLATION_DIR}${NODEJS_LATEST_VERSION}/ 
      echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
      echo -e "${COLOR_PRINT_GREEN}            Installing Shared Delegate Website${END_COLOR_PRINT}"
      echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
      install_nodejs
      configure_npm
      update_npm
      install_npm_global_packages
      download_shared_delegate_website
      get_installation_directory
      install_shared_delegates_website_npm_packages
      build_shared_delegates_website
      source ~/.profile || true
      echo
      echo
      update_systemd_service_files
      sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE}' > /lib/systemd/system/xcash-dpops.service"
      sed_services 's/\r$//g' /lib/systemd/system/xcash-dpops.service
      sudo systemctl daemon-reload
      get_installation_directory
      get_dependencies_current_version
    fi
  fi
}


function check_if_remove_shared_delegate_configure_install()
{
  if [ "${SHARED_DELEGATE^^}" == "NO" ]; then
    echo -ne "Installation configured as Solo delegate. Removing all the preinstalled Shared Delegate website"
    echo
    uninstall_shared_delegates_website
    update_systemd_service_files
    sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE}' > /lib/systemd/system/xcash-dpops.service"
    sed_services 's/\r$//g' /lib/systemd/system/xcash-dpops.service
    sudo systemctl daemon-reload
    sudo sed '/node-v/d' -i "${HOME}"/.profile
    sudo sed '/PATH=\/bin:/d' -i "${HOME}"/.profile
    sudo sed '/^[[:space:]]*$/d' -i "${HOME}"/.profile
    sudo sed -e :a -e '/^\n*$/{$d;N;};/\n$/ba' -i "${HOME}"/.profile
    source ~/.profile || true
    get_installation_directory
    get_dependencies_current_version
  fi
}


function check_ubuntu_version()
{   
    command -v lsb_release > /dev/null 2>&1 ||
    {
        echo -e "${COLOR_PRINT_RED}FAIL${END_COLOR_PRINT}"
        echo 
        echo -e "${COLOR_PRINT_RED}############################################################${END_COLOR_PRINT}"
        echo -e "${COLOR_PRINT_RED}           !!! CANNOT CHECK YOUR UBUNTU VERSION !!!${END_COLOR_PRINT}"
        echo -e "${COLOR_PRINT_RED}############################################################${END_COLOR_PRINT}"
        echo
        exit
    }

    UBUNTU_VERSION=$(lsb_release -r | awk '{print $2}' | sed s"|\.||g")
    if [ "$UBUNTU_VERSION" -lt 1804 ]; then
      echo -e "${COLOR_PRINT_RED}FAIL${END_COLOR_PRINT}"
      echo 
      echo -e "${COLOR_PRINT_RED}############################################################${END_COLOR_PRINT}"
      echo -e "${COLOR_PRINT_RED}          !!!  YOUR UBUNTU VERSION IS NOT SUPPORTED !!!${END_COLOR_PRINT}"
      echo -e "${COLOR_PRINT_RED}############################################################${END_COLOR_PRINT}"
      echo
      exit
    fi
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
    sudo apt update -y &>/dev/null
    echo -ne "\r${COLOR_PRINT_GREEN}Updating Packages List${END_COLOR_PRINT}"
    echo
}

function install_packages()
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
    echo -ne "${COLOR_PRINT_YELLOW}Installing Packages (This Might Take A While)${END_COLOR_PRINT}"
    sudo apt install ${XCASH_DPOPS_PACKAGES} -y &>/dev/null
    build_libgtest
    echo -ne "\r${COLOR_PRINT_GREEN}Installing Packages (This Might Take A While)${END_COLOR_PRINT}"
    echo
}

function build_libgtest()
{
  cd /usr/src/gtest &>/dev/null
  sudo cmake . &>/dev/null
  sudo make &>/dev/null
  if [ ! -f /usr/src/gtest/lib/libgtest.a ]; then
    sudo mv libg* /usr/lib/ &>/dev/null
  else
    sudo mv lib/libg* /usr/lib/ &>/dev/null
  fi
}





function download_xcash()
{
  echo -ne "${COLOR_PRINT_YELLOW}Downloading X-CASH${END_COLOR_PRINT}"
  cd "${XCASH_DPOPS_INSTALLATION_DIR}"
  git clone --quiet ${XCASH_URL}
  echo -ne "\r${COLOR_PRINT_GREEN}Downloading X-CASH${END_COLOR_PRINT}"
  echo
}

function build_xcash()
{
  echo -ne "${COLOR_PRINT_YELLOW}Building X-CASH (This Might Take A While)${END_COLOR_PRINT}"
  cd "${XCASH_DIR}"
  git checkout --quiet ${XCASH_CORE_BRANCH}
  if [ "$RAM_CPU_RATIO" -ge "$RAM_CPU_RATIO_ALL_CPU_THREADS" ]; then
    echo "y" | make clean &>/dev/null
    make release -j "${CPU_THREADS}" &>/dev/null
  else
    echo "y" | make clean &>/dev/null
    if [ "$RAM_CPU_RATIO" -eq 0 ]; then
        make release &>/dev/null
    else
        make release -j $((CPU_THREADS / 2)) &>/dev/null
    fi
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Building X-CASH (This Might Take A While)${END_COLOR_PRINT}"
  echo
}

function install_xcash()
{
  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                   Installing X-CASH${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  download_xcash
  build_xcash
}







function create_directories()
{
  echo -ne "${COLOR_PRINT_YELLOW}Creating Directories${END_COLOR_PRINT}"
  if [ ! -d "$XCASH_DPOPS_INSTALLATION_DIR" ]; then
    mkdir -p "${XCASH_DPOPS_INSTALLATION_DIR}"
  fi
  if [ ! -d "$MONGODB_INSTALLATION_DIR" ]; then
    sudo mkdir -p "${MONGODB_INSTALLATION_DIR}"
    sudo chmod 770 "${MONGODB_INSTALLATION_DIR}"
    sudo chown "$USER" "${MONGODB_INSTALLATION_DIR}"
  fi 
  if [ ! -d "$XCASH_WALLET_DIR" ]; then
    mkdir -p "${XCASH_WALLET_DIR}"
  fi
  if [ ! -d "$XCASH_SYSTEMPID_DIR" ]; then
    mkdir -p "${XCASH_SYSTEMPID_DIR}"
  fi
  if [ ! -d "$XCASH_LOGS_DIR" ]; then
    mkdir -p "${XCASH_LOGS_DIR}"
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Creating Directories${END_COLOR_PRINT}"
  echo
}

function create_files()
{
  touch "${XCASH_SYSTEMPID_DIR}"mongod.pid "${XCASH_SYSTEMPID_DIR}"xcash-daemon.pid
}

function create_systemd_service_files()
{
  echo -ne "${COLOR_PRINT_YELLOW}Creating Systemd Service Files${END_COLOR_PRINT}"
  sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_FIREWALL}' > /lib/systemd/system/firewall.service"
  sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_MONGODB}' > /lib/systemd/system/mongodb.service"
  sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_XCASH_DAEMON}' > /lib/systemd/system/xcash-daemon.service"
  sudo bash -c "echo '${SYSTEMD_TIMER_FILE_XCASH_DPOPS}' > /lib/systemd/system/xcash-dpops.timer"

  if [ ! "${SHARED_DELEGATE^^}" == "YES" ]; then
    sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SOLO_DELEGATE}' > /lib/systemd/system/xcash-dpops.service"
  else
    sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_XCASH_DPOPS_SHARED_DELEGATE}' > /lib/systemd/system/xcash-dpops.service"
  fi
  sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_XCASH_WALLET}' > /lib/systemd/system/xcash-rpc-wallet.service"
  sudo bash -c "echo '${SYSTEMD_TIMER_FILE_XCASH_WALLET}' > /lib/systemd/system/xcash-rpc-wallet.timer"

  sed_services 's/\r$//g' /lib/systemd/system/firewall.service
  sed_services 's/\r$//g' /lib/systemd/system/mongodb.service
  sed_services 's/\r$//g' /lib/systemd/system/xcash-daemon.service
  sed_services 's/\r$//g' /lib/systemd/system/xcash-dpops.timer
  sed_services 's/\r$//g' /lib/systemd/system/xcash-dpops.service
  sed_services 's/\r$//g' /lib/systemd/system/xcash-rpc-wallet.service
  sed_services 's/\r$//g' /lib/systemd/system/xcash-rpc-wallet.timer

  sudo systemctl daemon-reload
  echo -ne "\r${COLOR_PRINT_GREEN}Creating Systemd Service Files${END_COLOR_PRINT}"
  echo
}

function install_mongodb()
{
  echo -ne "${COLOR_PRINT_YELLOW}Installing MongoDB${END_COLOR_PRINT}"
  cd "${XCASH_DPOPS_INSTALLATION_DIR}"
  wget -q ${MONGODB_URL}
  tar -xf mongodb-linux-x86_64-*.tgz &>/dev/null
  sudo rm mongodb-linux-x86_64-*.tgz &>/dev/null
  sudo chown -R "$USER":"$USER" ${MONGODB_DIR}
  echo -ne "\nexport PATH=${MONGODB_DIR}bin:" >> "${HOME}"/.profile 
  echo -ne '$PATH' >> "${HOME}"/.profile
  source ~/.profile || true
  echo -ne "\r${COLOR_PRINT_GREEN}Installing MongoDB${END_COLOR_PRINT}"
  echo
}

function install_mongoc_driver()
{
  echo -ne "${COLOR_PRINT_YELLOW}Installing MongoC Driver${END_COLOR_PRINT}"
  cd "${XCASH_DPOPS_INSTALLATION_DIR}"
  wget -q ${MONGOC_DRIVER_URL}
  tar -xf mongo-c-driver-*.tar.gz &>/dev/null
  sudo rm mongo-c-driver-*.tar.gz &>/dev/null
  sudo chown -R "$USER":"$USER" mongo-c-driver-*
  cd mongo-c-driver-*
  mkdir cmake-build &>/dev/null
  cd cmake-build &>/dev/null
  sudo cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_BUILD_TYPE=Release .. &>/dev/null
  sudo make -j "${CPU_THREADS}" &>/dev/null
  sudo make install &>/dev/null
  sudo ldconfig
  echo -ne "\r${COLOR_PRINT_GREEN}Installing MongoC Driver${END_COLOR_PRINT}"
  echo
}

function download_xcash_dpops()
{
  echo -ne "${COLOR_PRINT_YELLOW}Downloading xcash-dpops${END_COLOR_PRINT}"
  cd "${XCASH_DPOPS_INSTALLATION_DIR}"
  git clone --quiet ${XCASH_DPOPS_URL}
  echo -ne "\r${COLOR_PRINT_GREEN}Downloading xcash-dpops${END_COLOR_PRINT}"
  echo
}

function build_xcash_dpops()
{
  echo -ne "${COLOR_PRINT_YELLOW}Building xcash-dpops${END_COLOR_PRINT}"
  cd "${XCASH_DPOPS_DIR}"
  if [ "$RAM_CPU_RATIO" -ge "$RAM_CPU_RATIO_ALL_CPU_THREADS" ]; then
    echo "y" | make clean &>/dev/null
    make release -j "${CPU_THREADS}" &>/dev/null
  else
    echo "y" | make clean &>/dev/null
    if [ "$RAM_CPU_RATIO" -eq 0 ]; then
        make release &>/dev/null
    else
        make release -j $((CPU_THREADS / 2)) &>/dev/null
    fi
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Building xcash-dpops${END_COLOR_PRINT}"
  echo
}

function create_block_verifier_key()
{
  echo -ne "${COLOR_PRINT_YELLOW}Creating Block Verifiers Key${END_COLOR_PRINT}"
  cd "${XCASH_DPOPS_DIR}"
  data=$(build/xcash-dpops --generate-key 2>&1 >/dev/null)
  BLOCK_VERIFIER_SECRET_KEY="${data: -132}"
  BLOCK_VERIFIER_SECRET_KEY="${BLOCK_VERIFIER_SECRET_KEY:0:128}"
  BLOCK_VERIFIER_PUBLIC_KEY="${BLOCK_VERIFIER_SECRET_KEY: -${BLOCK_VERIFIERS_PUBLIC_KEY_LENGTH}}"
  echo -ne "\r${COLOR_PRINT_GREEN}Creating Block Verifiers Key${END_COLOR_PRINT}"
  echo
}

function install_firewall()
{
  echo -ne "${COLOR_PRINT_YELLOW}Installing The Firewall${END_COLOR_PRINT}"
  # Reinstall iptables (solves some issues with some VPS)
  sudo apt-get install --reinstall iptables &>/dev/null
  if [ "${SHARED_DELEGATE^^}" == "YES" ]; then
    echo "$FIREWALL_SHARED_DELEGATES" > ${HOME}/firewall_script.sh
  else
    echo "$FIREWALL" > ${HOME}/firewall_script.sh
  fi
  sudo sed -i 's/\r$//g' ${HOME}/firewall_script.sh
  sudo chmod +x ${HOME}/firewall_script.sh
  sudo ${HOME}/firewall_script.sh
  sudo systemctl enable firewall &>/dev/null
  sudo systemctl start firewall &>/dev/null
  echo -ne "\r${COLOR_PRINT_GREEN}Installing The Firewall${END_COLOR_PRINT}"
  echo
}

function install_xcash_dpops()
{
  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                Installing xcash-dpops${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  install_mongodb
  install_mongoc_driver
  download_xcash_dpops
  build_xcash_dpops

  # Create the block verifier key if they choose to create a block verifier key
  if [ "${BLOCK_VERIFIER_KEY_SETTINGS^^}" == "C" ]; then
    create_block_verifier_key
  fi

  update_systemd_service_files
  create_systemd_service_files
  if [ ! "$container" == "lxc" ]; then
    install_firewall
  fi
  echo
  echo
}

function configure_xcash_dpops()
{
  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                Configuring xcash-dpops Installation${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"

  # Create the block verifier key if they choose to create a block verifier key
  if [ "${BLOCK_VERIFIER_KEY_SETTINGS^^}" == "C" ]; then
    create_block_verifier_key
  fi
  update_systemd_service_files
  create_systemd_service_files
  echo
  echo
}


function create_xcash_wallet()
{
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}      Creating X-CASH Wallet (This Might Take A While)  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"

  cd "${XCASH_DPOPS_INSTALLATION_DIR}"
  sudo rm -f "${XCASH_DPOPS_INSTALLATION_DIR}"xcash-wallets/delegate-wallet* 2&> /dev/null

  echo -ne "${COLOR_PRINT_YELLOW}Starting local daemon${END_COLOR_PRINT}"
  sudo systemctl stop xcash-daemon &>/dev/null
  sleep 10s
  sudo systemctl start xcash-daemon &>/dev/null
  sleep 20s
  echo -ne "\r${COLOR_PRINT_GREEN}Starting local daemon${END_COLOR_PRINT}"
  echo

  echo -e "${COLOR_PRINT_GREEN}Starting Wallet Refresh${END_COLOR_PRINT}"
  echo "exit" | "${XCASH_DIR}"build/release/bin/xcash-wallet-cli --generate-new-wallet "${XCASH_DPOPS_INSTALLATION_DIR}"xcash-wallets/delegate-wallet --password "${WALLET_PASSWORD}" --mnemonic-language English --restore-height 0 --trusted-daemon | stdbuf -oL tr '\r' '\n' | stdbuf -o 0 grep -C 1 "Height" | stdbuf -o 0 awk '{print "Processing: ",$1,$2,$3,$4}' ORS="\r"
  echo -ne "                                                                              \r"
  echo -e "${COLOR_PRINT_GREEN}Wallet Refresh Completed${END_COLOR_PRINT}"

  echo -ne "${COLOR_PRINT_YELLOW}Stopping local daemon${END_COLOR_PRINT}"
  sudo systemctl stop xcash-daemon &>/dev/null
  sleep 10s
  echo -ne "\r${COLOR_PRINT_GREEN}Stopping local daemon${END_COLOR_PRINT}"
  echo
}

function import_xcash_wallet()
{
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}     Importing X-CASH Wallet (This Might Take A While) ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"

  cd "${XCASH_DPOPS_INSTALLATION_DIR}"
  sudo rm -f "${XCASH_DPOPS_INSTALLATION_DIR}"xcash-wallets/delegate-wallet* 2&> /dev/null

  echo -ne "${COLOR_PRINT_YELLOW}Starting local daemon${END_COLOR_PRINT}"
  sudo systemctl stop xcash-daemon &>/dev/null
  sleep 10s
  sudo systemctl start xcash-daemon &>/dev/null
  sleep 20s
  echo -ne "\r${COLOR_PRINT_GREEN}Starting local daemon${END_COLOR_PRINT}"
  echo
  
  echo -e "${COLOR_PRINT_GREEN}Starting Wallet Refresh${END_COLOR_PRINT}"
  (echo -ne "\n"; echo "${WALLET_PASSWORD}"; echo "exit") | "${XCASH_DIR}"build/release/bin/xcash-wallet-cli --restore-deterministic-wallet --electrum-seed "${WALLET_SEED}" --generate-new-wallet "${XCASH_DPOPS_INSTALLATION_DIR}"xcash-wallets/delegate-wallet --password "${WALLET_PASSWORD}" --mnemonic-language English --restore-height 0 --trusted-daemon | stdbuf -oL tr '\r' '\n' | stdbuf -o 0 grep -C 1 "Height" | stdbuf -o 0 awk '{print "Processing: ",$1,$2,$3,$4}' ORS="\r"
  echo -ne "                                                                              \r"
  echo -e "${COLOR_PRINT_GREEN}Wallet Refresh Completed${END_COLOR_PRINT}"

  echo -ne "${COLOR_PRINT_YELLOW}Stopping local daemon${END_COLOR_PRINT}"
  sudo systemctl stop xcash-daemon &>/dev/null
  sleep 10s
  echo -ne "\r${COLOR_PRINT_GREEN}Stopping local daemon${END_COLOR_PRINT}"
  echo
  
  echo
  echo
}










function install_nodejs()
{
  echo -ne "${COLOR_PRINT_YELLOW}Installing Node.js${END_COLOR_PRINT}"
  cd "${XCASH_DPOPS_INSTALLATION_DIR}"
  wget -q ${NODEJS_URL}
  tar -xf node*.tar.xz &>/dev/null
  sudo rm node*.tar.xz &>/dev/null
  sudo chown -R "$USER":"$USER" ${NODEJS_DIR}
  echo -ne "\nexport PATH=${NODEJS_DIR}bin:" >> "${HOME}"/.profile 
  echo -ne '$PATH' >> "${HOME}"/.profile
  source ~/.profile || true
  echo -ne "\r${COLOR_PRINT_GREEN}Installing Node.js${END_COLOR_PRINT}"
  echo
}

function configure_npm()
{
  if [ "$EUID" -eq 0 ]; then
    echo -ne "${COLOR_PRINT_YELLOW}Configuring NPM For Root User${END_COLOR_PRINT}"
    source ~/.profile || true
    npm config set user 0 &>/dev/null
    npm config set unsafe-perm true &>/dev/null
    echo -ne "\r${COLOR_PRINT_GREEN}Configuring NPM For Root User${END_COLOR_PRINT}"
    echo
  fi
}

function update_npm()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating NPM${END_COLOR_PRINT}"
  source ~/.profile || true
  npm install -g npm &>/dev/null
  echo -ne "\r${COLOR_PRINT_GREEN}Updating NPM${END_COLOR_PRINT}"
  echo
}

function install_npm_global_packages()
{
  echo -ne "${COLOR_PRINT_YELLOW}Installing Global NPM Packages${END_COLOR_PRINT}"
  npm install -g @angular/cli@latest uglify-js &>/dev/null
  echo -ne "\r${COLOR_PRINT_GREEN}Installing Global NPM Packages${END_COLOR_PRINT}"
  echo
}

function download_shared_delegate_website()
{
  echo -ne "${COLOR_PRINT_YELLOW}Downloading Shared Delegates Website${END_COLOR_PRINT}"
  cd "${XCASH_DPOPS_INSTALLATION_DIR}"
  git clone --quiet ${SHARED_DELEGATES_WEBSITE_URL}
  echo -ne "\r${COLOR_PRINT_GREEN}Downloading Shared Delegates Website${END_COLOR_PRINT}"
  echo
}

function install_shared_delegates_website_npm_packages()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating node_modules${END_COLOR_PRINT}"
  cd "${SHARED_DELEGATES_WEBSITE_DIR}"
  npm update &>/dev/null
  echo -ne "\r${COLOR_PRINT_GREEN}Updating node_modules${END_COLOR_PRINT}"
  echo
}

function build_shared_delegates_website()
{
  echo -ne "${COLOR_PRINT_YELLOW}Building shared delegates website${END_COLOR_PRINT}"
  cd "${SHARED_DELEGATES_WEBSITE_DIR}"
  source ~/.profile || true
  npm run build &>/dev/null
  cd dist
  for f in *.js; do uglifyjs "$f" --compress --mangle --output "{$f}min"; rm "$f"; mv "{$f}min" "$f"; done
  if [ -d "$XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR" ]; then
    sudo rm -r "${XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR}"
  fi 
  cd ../
  cp -a dist "${XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR}" 
  echo -ne "\r${COLOR_PRINT_GREEN}Building shared delegates website${END_COLOR_PRINT}"
  echo
}

function install_shared_delegates_website()
{
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}            Installing Shared Delegate Website${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  install_nodejs
  configure_npm
  update_npm
  install_npm_global_packages
  download_shared_delegate_website
  install_shared_delegates_website_npm_packages
  build_shared_delegates_website
  source ~/.profile || true
  echo
  echo
}







function get_installation_directory()
{
  echo -ne "${COLOR_PRINT_YELLOW}Getting Installation Directories${END_COLOR_PRINT}"
  XCASH_DPOPS_INSTALLATION_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "xcash-dpops" -exec dirname {} \;)/
  XCASH_BLOCKCHAIN_INSTALLATION_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name ".X-CASH" -print)/
  WALLET_PASSWORD=$(cat /lib/systemd/system/xcash-rpc-wallet.service | awk '/password/ {print $5}')
  XCASH_DIR=${XCASH_DPOPS_INSTALLATION_DIR}xcash-core/
  XCASH_WALLET_DIR=${XCASH_DPOPS_INSTALLATION_DIR}xcash-wallets/
  XCASH_SYSTEMPID_DIR=${XCASH_DPOPS_INSTALLATION_DIR}systemdpid/
  XCASH_LOGS_DIR=${XCASH_DPOPS_INSTALLATION_DIR}logs/
  XCASH_DPOPS_DIR=${XCASH_DPOPS_INSTALLATION_DIR}xcash-dpops/
  XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR=${XCASH_DPOPS_DIR}delegates-pool-website/
  SHARED_DELEGATES_WEBSITE_DIR=${XCASH_DPOPS_INSTALLATION_DIR}delegates-pool-website/
  NODEJS_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "node-*-linux-x64" -print)/
  MONGODB_INSTALLATION_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -path "*/data/db" -print)/
  MONGODB_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "mongodb-linux-x86_64-ubuntu1804-*" -print)/
  MONGOC_DRIVER_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "mongo-c-driver-*" -print)/
  echo -ne "\r${COLOR_PRINT_GREEN}Getting Installation Directories${END_COLOR_PRINT}"
  echo

  # check to make sure it found the installed programs
  if [ "$XCASH_DPOPS_INSTALLATION_DIR" == "/" ]; then
    echo -e "${COLOR_PRINT_RED}Can not find the installation directory, Please run the auto installer in installation mode${END_COLOR_PRINT}"
  fi
  if [ "$XCASH_DIR" == "xcash-core/" ]; then
    echo -e "${COLOR_PRINT_RED}Can not find X-CASH${END_COLOR_PRINT}"
    install_xcash
  fi
  if [ "$XCASH_DPOPS_DIR" == "xcash-dpops/" ]; then
    echo -e "${COLOR_PRINT_RED}Can not find xcash-dpops${END_COLOR_PRINT}"
    download_xcash_dpops
    build_xcash_dpops
  fi
  if [ "$MONGODB_INSTALLATION_DIR" == "/" ]; then
    echo -e "${COLOR_PRINT_RED}Can not find the MongoDB installation directory, Please run the auto installer in installation mode${END_COLOR_PRINT}"
    install_mongodb
  fi
  if [ "$MONGODB_DIR" == "/" ]; then
    echo -e "${COLOR_PRINT_RED}Can not find MongoDB${END_COLOR_PRINT}"
    install_mongodb
  fi
  if [ "$MONGOC_DRIVER_DIR" == "/" ]; then
    echo -e "${COLOR_PRINT_RED}Can not find Mongo C Driver${END_COLOR_PRINT}"
    install_mongoc_driver
  fi
}

function get_dependencies_current_version()
{
  echo -ne "${COLOR_PRINT_YELLOW}Getting Dependencies Current Versions${END_COLOR_PRINT}"
  NODEJS_CURRENT_VERSION=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "node-*-linux-x64" -exec basename {} \;)
  MONGODB_CURRENT_VERSION=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "mongodb-linux-x86_64-ubuntu1804-*" -exec basename {} \;)
  MONGOC_DRIVER_CURRENT_VERSION=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "mongo-c-driver-*" -exec basename {} \;)
  echo -ne "\r${COLOR_PRINT_GREEN}Getting Dependencies Current Versions${END_COLOR_PRINT}"
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
    sudo apt install --only-upgrade ${XCASH_DPOPS_PACKAGES} -y &>/dev/null
    echo -ne "\r${COLOR_PRINT_GREEN}Updating Packages${END_COLOR_PRINT}"
    echo
}

function set_installation_dir_owner()
{
  sudo chown -R "$USER":"$USER" ${XCASH_DPOPS_INSTALLATION_DIR} 2&> /dev/null
  sudo chown -R "$USER":"$USER" ${XCASH_BLOCKCHAIN_INSTALLATION_DIR} 2&> /dev/null
}

function update_xcash()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating X-CASH (This Might Take A While)${END_COLOR_PRINT}"
  if [ ! -d "$XCASH_DIR" ]; then
    cd "${XCASH_DPOPS_INSTALLATION_DIR}"
    git clone --quiet "${XCASH_URL}"
  fi
  cd "${XCASH_DIR}"
  data=$([ $(git rev-parse HEAD) = $(git ls-remote $(git rev-parse --abbrev-ref @{u} | sed 's/\// /g') | cut -f1) ] && echo "1" || echo "0")
  if [ "$data" == "0" ]; then
    git reset --hard HEAD --quiet
    git pull --quiet
    if [ "$RAM_CPU_RATIO" -ge "$RAM_CPU_RATIO_ALL_CPU_THREADS" ]; then
      echo "y" | make clean &>/dev/null
      make release -j "${CPU_THREADS}" &>/dev/null
    else
      echo "y" | make clean &>/dev/null
      if [ "$RAM_CPU_RATIO" -eq 0 ]; then
          make release &>/dev/null
      else
          make release -j $((CPU_THREADS / 2)) &>/dev/null
      fi
    fi 
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Updating X-CASH (This Might Take A While)${END_COLOR_PRINT}"
  echo
}

function update_xcash_dpops()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating xcash-dpops${END_COLOR_PRINT}"
  if [ ! -d "$XCASH_DPOPS_DIR" ]; then
    cd "${XCASH_DPOPS_INSTALLATION_DIR}"
    git clone --quiet "${XCASH_DPOPS_URL}"
  fi
  cd "${XCASH_DPOPS_DIR}"
  data=$([ $(git rev-parse HEAD) = $(git ls-remote $(git rev-parse --abbrev-ref @{u} | sed 's/\// /g') | cut -f1) ] && echo "1" || echo "0")
  if [ "$data" == "0" ]; then
    git reset --hard HEAD --quiet
    git pull --quiet
    if [ "$RAM_CPU_RATIO" -ge "$RAM_CPU_RATIO_ALL_CPU_THREADS" ]; then
      echo "y" | make clean &>/dev/null
      make release -j "${CPU_THREADS}" &>/dev/null
    else
      echo "y" | make clean &>/dev/null
      if [ "$RAM_CPU_RATIO" -eq 0 ]; then
          make release &>/dev/null
      else
          make release -j $((CPU_THREADS / 2)) &>/dev/null
      fi
    fi
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Updating xcash-dpops${END_COLOR_PRINT}"
  echo
}

function update_shared_delegates_website()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating Shared Delegates Website${END_COLOR_PRINT}"
  if [ ! -d "$SHARED_DELEGATES_WEBSITE_DIR" ]; then
    cd "${XCASH_DPOPS_INSTALLATION_DIR}"
    git clone --quiet "${SHARED_DELEGATES_WEBSITE_URL}"
  fi
  cd "${SHARED_DELEGATES_WEBSITE_DIR}"
  data=$([ $(git rev-parse HEAD) = $(git ls-remote $(git rev-parse --abbrev-ref @{u} | sed 's/\// /g') | cut -f1) ] && echo "1" || echo "0")
  if [ "$data" == "0" ]; then
    git reset --hard HEAD --quiet
    git pull --quiet
    npm update &>/dev/null
    source ~/.profile || true
    npm run build &>/dev/null
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
  sudo rm -r "${MONGODB_DIR}"  
  cd "${XCASH_DPOPS_INSTALLATION_DIR}"
  wget -q ${MONGODB_URL}
  tar -xf mongodb-linux-x86_64-*.tgz &>/dev/null
  sudo rm mongodb-linux-x86_64-*.tgz &>/dev/null
  MONGODB_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "mongodb-linux-x86_64-ubuntu1804-*" -print)/
  sudo chown -R "$USER":"$USER" ${MONGODB_DIR}
  update_systemd_service_files
  sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_MONGODB}' > /lib/systemd/system/mongodb.service"
  sed_services 's/\r$//g' /lib/systemd/system/mongodb.service
  sudo systemctl daemon-reload
  sudo sed '/mongodb-linux-x86_64-ubuntu1804-/d' -i "${HOME}"/.profile
  sudo sed '/^[[:space:]]*$/d' -i "${HOME}"/.profile
  echo -ne "\nexport PATH=${MONGODB_DIR}bin:" >> "${HOME}"/.profile 
  echo -ne '$PATH' >> "${HOME}"/.profile
  sudo sed -e :a -e '/^\n*$/{$d;N;};/\n$/ba' -i "${HOME}"/.profile
  source ~/.profile || true
  echo -ne "\r${COLOR_PRINT_GREEN}Updating MongoDB${END_COLOR_PRINT}"
  echo
}

function update_mongoc_driver()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating Mongo C Driver${END_COLOR_PRINT}"
  cd "${XCASH_DPOPS_INSTALLATION_DIR}"
  sudo rm -r "${MONGOC_DRIVER_DIR}"
  wget -q ${MONGOC_DRIVER_URL}
  tar -xf mongo-c-driver-*.tar.gz &>/dev/null
  sudo rm mongo-c-driver-*.tar.gz &>/dev/null
  sudo chown -R "$USER":"$USER" mongo-c-driver-*
  cd mongo-c-driver-*
  mkdir cmake-build
  cd cmake-build
  cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_BUILD_TYPE=Release .. &>/dev/null
  sudo make -j "${CPU_THREADS}" &>/dev/null
  sudo make install &>/dev/null
  sudo ldconfig
  MONGOC_DRIVER_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "mongo-c-driver-*" -print)/
  echo -ne "\r${COLOR_PRINT_GREEN}Updating Mongo C Driver${END_COLOR_PRINT}"
  echo
}

function update_nodejs()
{
  echo -ne "${COLOR_PRINT_YELLOW}Updating NodeJS${END_COLOR_PRINT}"
  sudo rm -r "${NODEJS_DIR}"  
  cd "${XCASH_DPOPS_INSTALLATION_DIR}"
  wget -q ${NODEJS_URL}
  tar -xf node*.tar.xz &>/dev/null
  sudo rm node*.tar.xz &>/dev/null
  NODEJS_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name "node-*-linux-x64" -print)/
  sudo chown -R "$USER":"$USER" ${NODEJS_DIR}
  sudo sed '/node-v/d' -i "${HOME}"/.profile
  sudo sed '/PATH=\/bin:/d' -i "${HOME}"/.profile
  sudo sed '/^[[:space:]]*$/d' -i "${HOME}"/.profile
  echo -ne "\nexport PATH=${NODEJS_DIR}bin:" >> "${HOME}"/.profile 
  echo -ne '$PATH' >> "${HOME}"/.profile
  sudo sed -e :a -e '/^\n*$/{$d;N;};/\n$/ba' -i "${HOME}"/.profile
  source ~/.profile || true
  echo -ne "\r${COLOR_PRINT_GREEN}Updating NodeJS${END_COLOR_PRINT}"
  echo
}








function uninstall_packages()
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
    echo -ne "${COLOR_PRINT_YELLOW}Uninstalling Packages${END_COLOR_PRINT}"
    sudo apt --purge remove ${XCASH_DPOPS_PACKAGES} -y &>/dev/null
    echo -ne "\r${COLOR_PRINT_GREEN}Uninstalling Packages${END_COLOR_PRINT}"
    echo
}

function uninstall_systemd_service_files()
{
  echo -ne "${COLOR_PRINT_YELLOW}Uninstall Systemd Service Files${END_COLOR_PRINT}"
  if [ "$container" == "lxc" ]; then
    sudo truncate --size 0 /lib/systemd/system/firewall.service /lib/systemd/system/mongodb.service /lib/systemd/system/xcash-daemon.service /lib/systemd/system/xcash-dpops.service /lib/systemd/system/xcash-rpc-wallet.service 
    sudo rm -f /lib/systemd/system/xcash-dpops.timer /lib/systemd/system/xcash-rpc-wallet.timer ${HOME}/firewall_script.sh
  else
    sudo rm -f /lib/systemd/system/firewall.service /lib/systemd/system/mongodb.service /lib/systemd/system/xcash-daemon.service /lib/systemd/system/xcash-dpops.service /lib/systemd/system/xcash-rpc-wallet.service /lib/systemd/system/xcash-dpops.timer /lib/systemd/system/xcash-rpc-wallet.timer ${HOME}/firewall_script.sh
  fi
  sudo systemctl daemon-reload
  echo -ne "\r${COLOR_PRINT_GREEN}Uninstall Systemd Service Files${END_COLOR_PRINT}"
  echo
}

function uninstall_shared_delegates_website()
{
  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}            Uninstalling Shared Delegate Website${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"

  get_installation_directory

  if [ -d "${XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR}" ]; then
    sudo rm -r "${XCASH_DPOPS_SHARED_DELEGATE_FOLDER_DIR}"
  fi
  if [ -d "${SHARED_DELEGATES_WEBSITE_DIR}" ]; then
    sudo rm -r "${SHARED_DELEGATES_WEBSITE_DIR}"
  fi
  if [ -d "${NODEJS_DIR}" ]; then
    sudo rm -r "${NODEJS_DIR}"
  fi

  sudo sed '/node-v/d' -i "${HOME}"/.profile
  sudo sed '/PATH=\/bin:/d' -i "${HOME}"/.profile
  sudo sed '/^[[:space:]]*$/d' -i "${HOME}"/.profile
  sudo sed -e :a -e '/^\n*$/{$d;N;};/\n$/ba' -i "${HOME}"/.profile
  source ~/.profile || true
  echo
  echo
}











function install()
{
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                  Starting Installation${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"

  # Create directories
  create_directories

  # Create files
  create_files

  # Update the package list
  update_packages_list
 
  # Install packages
  install_packages

  # Install the blockchain
  install_blockchain

  # Install X-CASH
  install_xcash

  # Install xcash-dpops
  install_xcash_dpops

  # Install shared delegates website
  if [ "${SHARED_DELEGATE^^}" == "YES" ]; then
    install_shared_delegates_website
  fi

  # Create or import the wallet
  if [ "${WALLET_SETTINGS^^}" == "YES" ]; then
    create_xcash_wallet
  else
    import_xcash_wallet
  fi

  # Get the current xcash wallet data
  get_current_xcash_wallet_data

  # import the wallet if they created the wallet before. This should fix any 0 balance error
  if [ "${WALLET_SETTINGS^^}" == "YES" ]; then
    import_xcash_wallet
  fi

  # add the public address and block verifiers secret key to the XCASH_Daemon systemd service file
  sed_services "s/xcash-core\/build\/release\/bin\/xcashd/xcash-core\/build\/release\/bin\/xcashd --xcash-dpops-delegates-public-address $PUBLIC_ADDRESS --xcash-dpops-delegates-secret-key $BLOCK_VERIFIER_SECRET_KEY/g" /lib/systemd/system/xcash-daemon.service

  # Create a swap file if they don't already have one and have low ram, if not inside container
  if [ ! "$container" == "lxc" ]; then
    SWAP_FILE=$(sudo swapon --show)
    if [ -z "$SWAP_FILE" ] && [ "$RAM" -lt 10 ]; then
      create_swap_file
    fi
  fi
  
  # Create xcash wallet log symlink to old location
  touch "${XCASH_LOGS_DIR}xcash-wallet-rpc.log" && sudo rm -f "${XCASH_DIR}build/release/bin/xcash-wallet-rpc.log" && ln -s "${XCASH_LOGS_DIR}xcash-wallet-rpc.log" "${XCASH_DIR}build/release/bin/xcash-wallet-rpc.log"

  # Start the systemd service files
  start_systemd_service_files


  if [ "${AUTOSTART_SETTINGS^^}" == "YES" ]; then
    enable_service_files_at_startup
  fi

  cd ~

  # Display X-CASH current wallet data  
  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}          Installation Has Completed Successfully  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo
  echo -e "${CURRENT_XCASH_WALLET_INFORMATION}"
  echo
  echo -e "${COLOR_PRINT_YELLOW}Please make sure to save the above information in a secure place and press enter when done${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  relogin_user
}


function configure()
{
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                  Starting Configure Installation${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"

  # Stop service files
  stop_systemd_service_files

  # Remove shared website if solo 
  check_if_remove_shared_delegate_configure_install

  # Re-set the owner of the install directory (can fix some "edge" issues for some users)
  set_installation_dir_owner

  # Ask if use already present blockchain or use bootstrap file
  echo -ne "${COLOR_PRINT_YELLOW}Download and use the blockchain bootstrap? Leave empty for YES, write N for NO: ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  BOOTSTRAP_BLOCKCHAIN_OPTION=$([ "$data" == "" ] && echo "YES" || echo "NO")
  if [ "$BOOTSTRAP_BLOCKCHAIN_OPTION" == "YES" ]; then
    install_or_update_blockchain
  fi

  # Configure xcash-dpops
  configure_xcash_dpops

  # Create or import the wallet
  if [ "${WALLET_SETTINGS^^}" == "YES" ]; then
    create_xcash_wallet
  else
    import_xcash_wallet
  fi

  # Get the current xcash wallet data
  get_current_xcash_wallet_data

  # import the wallet if they created the wallet before. This should fix any 0 balance error
  if [ "${WALLET_SETTINGS^^}" == "YES" ]; then
    import_xcash_wallet
  fi

  # add the public address and block verifiers secret key to the XCASH_Daemon systemd service file
  sed_services "s/xcash-core\/build\/release\/bin\/xcashd/xcash-core\/build\/release\/bin\/xcashd --xcash-dpops-delegates-public-address $PUBLIC_ADDRESS --xcash-dpops-delegates-secret-key $BLOCK_VERIFIER_SECRET_KEY/g" /lib/systemd/system/xcash-daemon.service

  # Start the systemd service files
  start_systemd_service_files

  if [ "${AUTOSTART_SETTINGS^^}" == "YES" ]; then
    enable_service_files_at_startup
  fi

  # Display X-CASH current wallet data  
  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}          Configuration Has Completed Successfully  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo
  echo -e "${CURRENT_XCASH_WALLET_INFORMATION}"
  echo
  echo -e "${COLOR_PRINT_YELLOW}Please make sure to save the above information in a secure place and press enter when done${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  relogin_user
}

function update()
{
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                  Updating xcash-dpops${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo

  # Check if solo node
  check_if_solo_node

  # Get the installation directory
  get_installation_directory

  # Get the current version of the dependencies
  get_dependencies_current_version

  # Stop the systemd service files
  stop_systemd_service_files

  # Update the package list
  update_packages_list

  # Update all system packages that are xcash-dpops dependencies
  update_packages

  # Re-set the owner of the install directory (can fix some "edge" issues for some users)
  set_installation_dir_owner

  # Update all repositories
  update_xcash
  update_xcash_dpops
  if [ "${SHARED_DELEGATE^^}" == "YES" ]; then
    update_shared_delegates_website
  fi

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
  if [ "${SHARED_DELEGATE^^}" == "YES" ]; then
    if [ ! "$NODEJS_CURRENT_VERSION" == "$NODEJS_LATEST_VERSION" ]; then
      update_nodejs
      install_npm_global_packages
    else
      echo -e "${COLOR_PRINT_GREEN}NodeJS Is Already Up To Date${END_COLOR_PRINT}"
    fi
    configure_npm
    update_npm
  fi
  
  # Create xcash wallet log symlink to old location
  touch "${XCASH_LOGS_DIR}xcash-wallet-rpc.log" && sudo rm -f "${XCASH_DIR}build/release/bin/xcash-wallet-rpc.log" && ln -s "${XCASH_LOGS_DIR}xcash-wallet-rpc.log" "${XCASH_DIR}build/release/bin/xcash-wallet-rpc.log"

  # Start the systemd service files
  start_systemd_service_files

  cd ~

  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}          Update Has Completed Successfully  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
}

function uninstall()
{
  echo -ne "${COLOR_PRINT_RED}Please Confirm You Want To Uninstall By Typing \"Uninstall\":${END_COLOR_PRINT}"
  read -r data
  if [ ! "$data" == "Uninstall" ]; then
    exit
  fi
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                Uninstalling xcash-dpops${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo

  # Get the installation directory
  get_installation_directory

  # Re-set the owner of the install directory (can fix some "edge" issues for some users)
  set_installation_dir_owner

  # Restart the X-CASH Daemon and stop the X-CASH Wallet RPC
  echo -ne "${COLOR_PRINT_YELLOW}Shutting Down X-CASH Wallet Systemd Service File and Restarting XCASH Daemon Systemd Service File${END_COLOR_PRINT}"
  sudo systemctl stop xcash-daemon &>/dev/null
  sleep 10s
  sudo systemctl stop xcash-rpc-wallet &>/dev/null
  sleep 10s
  echo -ne "\r${COLOR_PRINT_GREEN}Shutting Down X-CASH Wallet Systemd Service File and Restarting XCASH Daemon Systemd Service File${END_COLOR_PRINT}"
  echo
  
  # get the block verifiers secret key from the systemd service file
  BLOCK_VERIFIER_SECRET_KEY=$(cat /lib/systemd/system/xcash-dpops.service)
  BLOCK_VERIFIER_SECRET_KEY=$(echo $BLOCK_VERIFIER_SECRET_KEY | awk -F '--block-verifiers-secret-key' '{print $2}')
  BLOCK_VERIFIER_SECRET_KEY=${BLOCK_VERIFIER_SECRET_KEY:1:$BLOCK_VERIFIERS_SECRET_KEY_LENGTH}
  BLOCK_VERIFIER_PUBLIC_KEY="${BLOCK_VERIFIER_SECRET_KEY: -${BLOCK_VERIFIERS_PUBLIC_KEY_LENGTH}}"

  # Get the current xcash wallet data
  get_current_xcash_wallet_data

  # Stop the systemd service files
  stop_systemd_service_files

  # Uninstall packages
  uninstall_packages

  # Uninstall Systemd Service Files (and remove firewall script from home)
  uninstall_systemd_service_files

  # Uninstall the Mongo C Driver  
  echo -ne "${COLOR_PRINT_YELLOW}Uninstalling Mongo C Driver${END_COLOR_PRINT}"
  sudo /usr/local/share/mongo-c-driver/uninstall.sh  &>/dev/null
  sudo ldconfig
  echo -ne "\r${COLOR_PRINT_GREEN}Uninstalling Mongo C Driver${END_COLOR_PRINT}"
  echo


  # Uninstall the installation folder and the blockchain folder
  echo -ne "${COLOR_PRINT_YELLOW}Uninstalling xcash-dpops Installation Directory${END_COLOR_PRINT}"
  sudo rm -rf "${XCASH_DPOPS_INSTALLATION_DIR}" 2&> /dev/null || true
  echo -ne "\r${COLOR_PRINT_GREEN}Uninstalling xcash-dpops Installation Directory${END_COLOR_PRINT}"
  echo
  echo -ne "${COLOR_PRINT_YELLOW}Removing the blockchain folder${END_COLOR_PRINT}"
  sudo rm -rf "${XCASH_BLOCKCHAIN_INSTALLATION_DIR}" 2&> /dev/null || true
  echo -ne "\r${COLOR_PRINT_GREEN}Removing the blockchain folder${END_COLOR_PRINT}"
  echo
  if [ "$container" == "lxc" ]; then
    echo -e "${COLOR_PRINT_YELLOW}This is a container installation, please remove the container and also the host data files (bind mounts) to complete the uninstall${END_COLOR_PRINT}"
  fi


  # Update profile
  echo -ne "${COLOR_PRINT_YELLOW}Updating Profile${END_COLOR_PRINT}"
  sudo sed '/mongodb-linux-x86_64-ubuntu1804-/d' -i "${HOME}"/.profile
  sudo sed '/node-v/d' -i "${HOME}"/.profile
  sudo sed '/PATH=\/bin:/d' -i "${HOME}"/.profile
  sudo sed '/^[[:space:]]*$/d' -i "${HOME}"/.profile
  sudo sed -e :a -e '/^\n*$/{$d;N;};/\n$/ba' -i "${HOME}"/.profile
  source ~/.profile || true
  echo -ne "\r${COLOR_PRINT_GREEN}Updating Profile${END_COLOR_PRINT}"
  echo

  cd ~

  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}          Uninstall Has Completed Successfully  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  
  # Display X-CASH current wallet data  
  echo
  echo
  echo -e "${CURRENT_XCASH_WALLET_INFORMATION}"
  echo
  echo -e "${COLOR_PRINT_YELLOW}Please make sure to save (if needed) the above information in a secure place and press enter when done${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  relogin_user
}



function install_node()
{
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                  Starting Installation${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"

  get_ssh_port

  # Update global variables
  XCASH_DIR=${XCASH_DPOPS_INSTALLATION_DIR}xcash-core/
  XCASH_SYSTEMPID_DIR=${XCASH_DPOPS_INSTALLATION_DIR}systemdpid/
  XCASH_LOGS_DIR=${XCASH_DPOPS_INSTALLATION_DIR}logs/

  # Create directories
  echo -ne "${COLOR_PRINT_YELLOW}Creating Directories${END_COLOR_PRINT}"
  cd ~
  if [ ! -d "$XCASH_DPOPS_INSTALLATION_DIR" ]; then
    mkdir -p "${XCASH_DPOPS_INSTALLATION_DIR}"
  fi
  if [ ! -d "$XCASH_SYSTEMPID_DIR" ]; then
    mkdir -p "${XCASH_SYSTEMPID_DIR}"
  fi
  if [ ! -d "$XCASH_LOGS_DIR" ]; then
    mkdir -p "${XCASH_LOGS_DIR}"
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Creating Directories${END_COLOR_PRINT}"
  echo

  # Create files
  touch $HOME/xcash-official/systemdpid/xcash-daemon.pid

  # Update the package list
  update_packages_list
 
  # Install packages
  install_packages

  # Install the blockchain
  install_blockchain

  # Install X-CASH
  install_xcash

  # install the systemd service files
  echo -ne "${COLOR_PRINT_YELLOW}Creating Systemd Service Files${END_COLOR_PRINT}"

  FIREWALL=$(cat <(curl -sSL $FIREWALL_XCASH_NODE_URL))
  FIREWALL="${FIREWALL//'${SSH_PORT_NUMBER}'/$SSH_PORT_NUMBER}"
  SYSTEMD_SERVICE_FILE_FIREWALL=$(cat <(curl -sSL $SYSTEMD_SERVICE_FILE_FIREWALL_URL))
  SYSTEMD_SERVICE_FILE_FIREWALL="${SYSTEMD_SERVICE_FILE_FIREWALL//'${HOME}'/$HOME}"
  SYSTEMD_SERVICE_FILE_XCASH_DAEMON=$(cat <(curl -sSL $SYSTEMD_SERVICE_FILE_XCASH_DAEMON_URL))
  SYSTEMD_SERVICE_FILE_XCASH_DAEMON="${SYSTEMD_SERVICE_FILE_XCASH_DAEMON//'${USER}'/$USER}"
  SYSTEMD_SERVICE_FILE_XCASH_DAEMON="${SYSTEMD_SERVICE_FILE_XCASH_DAEMON//'${XCASH_DPOPS_INSTALLATION_DIR}'/$XCASH_DPOPS_INSTALLATION_DIR}"
  SYSTEMD_SERVICE_FILE_XCASH_DAEMON="${SYSTEMD_SERVICE_FILE_XCASH_DAEMON//'${XCASH_DIR}'/$XCASH_DIR}"
  SYSTEMD_SERVICE_FILE_XCASH_DAEMON="${SYSTEMD_SERVICE_FILE_XCASH_DAEMON//'${XCASH_BLOCKCHAIN_INSTALLATION_DIR}'/$XCASH_BLOCKCHAIN_INSTALLATION_DIR}"
  SYSTEMD_SERVICE_FILE_XCASH_DAEMON="${SYSTEMD_SERVICE_FILE_XCASH_DAEMON//'${XCASH_LOGS_DIR}'/$XCASH_LOGS_DIR}"
  SYSTEMD_SERVICE_FILE_XCASH_DAEMON="${SYSTEMD_SERVICE_FILE_XCASH_DAEMON//'${XCASH_SYSTEMPID_DIR}'/$XCASH_SYSTEMPID_DIR}"

  sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_FIREWALL}' > /lib/systemd/system/firewall.service"
  sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_XCASH_DAEMON}' > /lib/systemd/system/xcash-daemon.service"

  sed_services 's/\r$//g' /lib/systemd/system/firewall.service
  sed_services 's/\r$//g' /lib/systemd/system/xcash-daemon.service

  sudo systemctl daemon-reload
  echo -ne "\r${COLOR_PRINT_GREEN}Creating Systemd Service Files${END_COLOR_PRINT}"
  echo

  # Install the firewall
  echo -ne "${COLOR_PRINT_YELLOW}Installing The Firewall${END_COLOR_PRINT}"
  # Reinstall iptables (solves some issues with some VPS)
  sudo apt-get install --reinstall iptables &>/dev/null
  echo "$FIREWALL" > ${HOME}/firewall_script.sh
  sudo sed -i 's/\r$//g' ${HOME}/firewall_script.sh
  sudo chmod +x ${HOME}/firewall_script.sh
  sudo ${HOME}/firewall_script.sh
  sudo systemctl enable firewall &>/dev/null
  sudo systemctl start firewall &>/dev/null
  echo -ne "\r${COLOR_PRINT_GREEN}Installing The Firewall${END_COLOR_PRINT}"
  echo
  
  # Start the systemd service files and enable them at startup
  sudo systemctl enable xcash-daemon &>/dev/null
  sudo systemctl start xcash-daemon &>/dev/null

  cd ~

  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}          Installation Has Completed Successfully  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo
}

function update_node()
{
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                  Updating xcash node${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo

  # Update global variables
  XCASH_DIR=${XCASH_DPOPS_INSTALLATION_DIR}xcash-core/

  # Stop the systemd service files
  sudo systemctl stop xcash-daemon

  # Update the package list
  update_packages_list

  # Update all system packages that are xcash-dpops dependencies
  update_packages

  # Re-set the owner of the install directory (can fix some "edge" issues for some users)
  set_installation_dir_owner

  # Update all repositories
  update_xcash

  # Start the systemd service files
  sudo systemctl start xcash-daemon

  cd ~

  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}          Update Has Completed Successfully  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
}

function uninstall_node()
{
  echo -ne "${COLOR_PRINT_RED}Please Confirm You Want To Uninstall By Typing \"Uninstall\":${END_COLOR_PRINT}"
  read -r data
  if [ ! "$data" == "Uninstall" ]; then
    exit
  fi
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}                Uninstalling xcash node${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo

  sudo systemctl stop xcash-daemon &>/dev/null

  # Re-set the owner of the install directory (can fix some "edge" issues for some users)
  set_installation_dir_owner

  # Uninstall packages
  uninstall_packages

  # Uninstall Systemd Service Files (and remove firewall script from home)
  if [ "$container" == "lxc" ]; then
    sudo truncate --size 0 /lib/systemd/system/firewall.service /lib/systemd/system/mongodb.service /lib/systemd/system/xcash-daemon.service
    sudo rm -f ${HOME}/firewall_script.sh
  else
    sudo rm -f /lib/systemd/system/firewall.service /lib/systemd/system/mongodb.service /lib/systemd/system/xcash-daemon.service ${HOME}/firewall_script.sh
  fi
  sudo systemctl daemon-reload

  # Uninstall the installation folder and blockchain folder
  sudo rm -rf "${XCASH_DPOPS_INSTALLATION_DIR}" 2&> /dev/null || true
  sudo rm -rf "${XCASH_BLOCKCHAIN_INSTALLATION_DIR}" 2&> /dev/null || true
  if [ "$container" == "lxc" ]; then
    echo -e "${COLOR_PRINT_YELLOW}This is a container installation, please remove the container and also the host data files (bind mounts) to complete the uninstall${END_COLOR_PRINT}"
  fi

  cd ~

  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}          Uninstall Has Completed Successfully  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
}






function change_solo_or_shared_delegate()
{
  check_if_solo_node
  check_if_upgrade_solo_delegate_and_shared_delegate
  echo
}

function test_update()
{
  get_installation_directory
  stop_systemd_service_files
  echo -ne "${COLOR_PRINT_YELLOW}Resetting the Blockchain${END_COLOR_PRINT}"
  sudo systemctl start xcash-daemon mongodb
  sleep 30s
  data=$(curl -s -X POST http://127.0.0.1:18281/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"get_block_count"}' -H 'Content-Type: application/json')
  data="${data:66:6}"
  data="${data%,*}"
  data=$((data-XCASH_DPOPS_BLOCK_HEIGHT))
  sudo systemctl stop xcash-daemon
  sleep 30s
  if [ $data -ne 0 ]; then
    "${XCASH_DIR}"build/release/bin/xcash-blockchain-import --data-dir "${XCASH_BLOCKCHAIN_INSTALLATION_DIR}" --pop-blocks ${data} &>/dev/null
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Resetting the Blockchain${END_COLOR_PRINT}"
  echo
  source ~/.profile || true
  echo -ne "${COLOR_PRINT_YELLOW}Resetting the Database${END_COLOR_PRINT}"
  (echo "use XCASH_PROOF_OF_STAKE"; echo "db.reserve_bytes_1.drop()"; echo "exit";) | mongo &>/dev/null
  (echo "use XCASH_PROOF_OF_STAKE"; echo "db.reserve_bytes_2.drop()"; echo "exit";) | mongo &>/dev/null
  (echo "use XCASH_PROOF_OF_STAKE"; echo "db.reserve_bytes_3.drop()"; echo "exit";) | mongo &>/dev/null
  (echo "use XCASH_PROOF_OF_STAKE"; echo "db.reserve_bytes_4.drop()"; echo "exit";) | mongo &>/dev/null
  (echo "use XCASH_PROOF_OF_STAKE"; echo "db.reserve_bytes_5.drop()"; echo "exit";) | mongo &>/dev/null
  (echo "use XCASH_PROOF_OF_STAKE_DELEGATES"; echo "db.dropDatabase()"; echo "exit";) | mongo &>/dev/null
  echo -ne "\r${COLOR_PRINT_GREEN}Resetting the Database${END_COLOR_PRINT}"
  echo
  update
}

function test_update_reset_delegates()
{
  get_installation_directory
  stop_systemd_service_files
  echo -ne "${COLOR_PRINT_YELLOW}Resetting the Blockchain${END_COLOR_PRINT}"
  sudo systemctl start xcash-daemon mongodb &>/dev/null
  sleep 30s
  data=$(curl -s -X POST http://127.0.0.1:18281/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"get_block_count"}' -H 'Content-Type: application/json')
  data="${data:66:6}"
  data=$((data-XCASH_DPOPS_BLOCK_HEIGHT))
  sudo systemctl stop xcash-daemon &>/dev/null
  sleep 30s
  if [ $data -ne 0 ]; then
    "${XCASH_DIR}"build/release/bin/xcash-blockchain-import --data-dir "${XCASH_BLOCKCHAIN_INSTALLATION_DIR}" --pop-blocks ${data} &>/dev/null
  fi
  echo -ne "\r${COLOR_PRINT_GREEN}Resetting the Blockchain${END_COLOR_PRINT}"
  echo
  # Source profile to fix some strange "edge" behaviors
  source ~/.profile || true
  echo -ne "${COLOR_PRINT_YELLOW}Resetting the Database${END_COLOR_PRINT}"
  (echo "use XCASH_PROOF_OF_STAKE"; echo "db.dropDatabase()"; echo "exit";) | mongo &>/dev/null
  (echo "use XCASH_PROOF_OF_STAKE_DELEGATES"; echo "db.dropDatabase()"; echo "exit";) | mongo &>/dev/null
  echo -ne "\r${COLOR_PRINT_GREEN}Resetting the Database${END_COLOR_PRINT}"
  echo
  echo
  update
}

function get_ssh_port()
{
  echo
  echo -ne "${COLOR_PRINT_YELLOW}What port do you use to connect through SSH with your server (press enter for default port: 22): ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  SSH_PORT_NUMBER=$([ ! "$data" == "" ] && echo "$data" || echo "$SSH_PORT_NUMBER")
}

function install_firewall_script()
{
  get_ssh_port
  echo -ne "${COLOR_PRINT_YELLOW}Installing The Firewall${END_COLOR_PRINT}"
  # Reinstall iptables (solves some issues with some VPS)
  sudo apt-get install --reinstall iptables &>/dev/null
  update_systemd_service_files
  sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_FIREWALL}' > /lib/systemd/system/firewall.service"
  sed_services 's/\r$//g' /lib/systemd/system/firewall.service
  sudo systemctl daemon-reload
  echo "$FIREWALL" > ${HOME}/firewall_script.sh
  sudo chmod +x ${HOME}/firewall_script.sh
  sudo ${HOME}/firewall_script.sh
  sudo systemctl enable firewall &>/dev/null
  sudo systemctl start firewall &>/dev/null
  echo -ne "\r${COLOR_PRINT_GREEN}Installing The Firewall${END_COLOR_PRINT}"
  echo
}

function install_firewall_script_shared_delegates()
{
  get_ssh_port
  echo -ne "${COLOR_PRINT_YELLOW}Installing The Firewall${END_COLOR_PRINT}"
  # Reinstall iptables (solves some issues with some VPS)
  sudo apt-get install --reinstall iptables &>/dev/null
  update_systemd_service_files
  sudo bash -c "echo '${SYSTEMD_SERVICE_FILE_FIREWALL}' > /lib/systemd/system/firewall.service"
  sed_services 's/\r$//g' /lib/systemd/system/firewall.service
  sudo systemctl daemon-reload
  echo "$FIREWALL_SHARED_DELEGATES" > ${HOME}/firewall_script.sh
  sudo chmod +x ${HOME}/firewall_script.sh
  sudo ${HOME}/firewall_script.sh
  sudo systemctl enable firewall &>/dev/null
  sudo systemctl start firewall &>/dev/null
  echo -ne "\r${COLOR_PRINT_GREEN}Installing The Firewall${END_COLOR_PRINT}"
  echo
}

function install_or_update_blockchain()
{
  echo -e "${COLOR_PRINT_GREEN}Installing / Updating The BlockChain (This Might Take a While, please follow the progress)${END_COLOR_PRINT}"
  cd $HOME
  XCASH_BLOCKCHAIN_INSTALLATION_DIR=$(sudo find / -path /sys -prune -o -path /proc -prune -o -path /dev -prune -o -path /var -prune -o -type d -name ".X-CASH" -print)/
  if [ $XCASH_BLOCKCHAIN_INSTALLATION_DIR = "/" ]; then
    XCASH_BLOCKCHAIN_INSTALLATION_DIR="${HOME}/.X-CASH/"
  fi
  # Install 7z and wget if not already installed
  sudo apt install -y p7zip-full wget &>/dev/null
  cd && test -f xcash-blockchain.7z && sudo rm -rf xcash-blockchain.7z*
  echo -e "${COLOR_PRINT_GREEN}Starting the Download${END_COLOR_PRINT}"
  wget -q --show-progress ${XCASH_BLOCKCHAIN_BOOTSTRAP_URL}
  echo -e "${COLOR_PRINT_GREEN}Starting Extraction${END_COLOR_PRINT}"
  7z x xcash-blockchain.7z -bso0 -bse0 -o${XCASH_BLOCKCHAIN_INSTALLATION_DIR}
  cd ${XCASH_BLOCKCHAIN_INSTALLATION_DIR}
  cp -a .X-CASH/* ./
  sudo rm -r .X-CASH
  cd $HOME
  sudo rm xcash-blockchain.7z
  echo -e "${COLOR_PRINT_GREEN}Installing / Updating The BlockChain Completed${END_COLOR_PRINT}"
  echo
}

function install_blockchain()
{
  if [ ! -d ${XCASH_BLOCKCHAIN_INSTALLATION_DIR} ] || [ ! -d ${XCASH_BLOCKCHAIN_INSTALLATION_DIR}lmdb/ ]; then
    echo -e "${COLOR_PRINT_GREEN}Installing The BlockChain (This Might Take a While, please follow the progress)${END_COLOR_PRINT}"
    cd $HOME
    cd && test -f xcash-blockchain.7z && sudo rm -rf xcash-blockchain.7z*
    echo -e "${COLOR_PRINT_GREEN}Starting the Download${END_COLOR_PRINT}"
    wget -q --show-progress ${XCASH_BLOCKCHAIN_BOOTSTRAP_URL}
    echo -e "${COLOR_PRINT_GREEN}Starting Extraction${END_COLOR_PRINT}"
    7z x xcash-blockchain.7z -bso0 -bse0 -o${XCASH_BLOCKCHAIN_INSTALLATION_DIR}
    cd ${XCASH_BLOCKCHAIN_INSTALLATION_DIR}
    cp -a .X-CASH/* ./
    sudo rm -r .X-CASH
    cd $HOME
    sudo rm xcash-blockchain.7z
    echo -e "${COLOR_PRINT_GREEN}Installing The BlockChain Completed${END_COLOR_PRINT}"
    echo
  fi
}

function edit_shared_delegate_settings()
{
  # check if they are already a shared delegate
  if grep -q "shared-delegates-website" /lib/systemd/system/xcash-dpops.service; then
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

    echo -ne "${COLOR_PRINT_YELLOW}Updating Shared Delegate Settings${END_COLOR_PRINT}"
    sed_services "s/--fee.*--minimum-amount/--fee $DPOPS_FEE --minimum-amount/g" /lib/systemd/system/xcash-dpops.service
    sed_services "s/--minimum-amount.*/--minimum-amount $DPOPS_MINIMUM_AMOUNT/g" /lib/systemd/system/xcash-dpops.service
    sudo systemctl daemon-reload
    echo -ne "\r${COLOR_PRINT_GREEN}Updating Shared Delegate Settings${END_COLOR_PRINT}"
    echo
  else
    echo -ne "\r${COLOR_PRINT_RED}Your delegate is not setup as a shared delegate${END_COLOR_PRINT}"
    echo
  fi
}


function register_update_delegate()
{
  XCASH_DELEGATE_NAME=""
  echo
  echo -ne "${COLOR_PRINT_YELLOW}Do you want to register a new delegate? Leave empty for YES, write N for NO: ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  if [ "$data" == "" ]; then
    echo -ne "${COLOR_PRINT_YELLOW}Enter your delegate name: ${END_COLOR_PRINT}"
    read -r XCASH_DELEGATE_NAME
    echo -ne "\r"
    echo
    echo -ne "${COLOR_PRINT_YELLOW}Enter your domain name or IP address: ${END_COLOR_PRINT}"
    read -r XCASH_DELEGATE_DOMAIN
    echo -ne "\r"
    echo
    # Stop the rpc wallet service
    sudo systemctl stop xcash-rpc-wallet &>/dev/null
    # Get required information
    get_installation_directory
    # get the block verifiers secret key from the systemd service file
    BLOCK_VERIFIER_SECRET_KEY=$(cat /lib/systemd/system/xcash-dpops.service)
    BLOCK_VERIFIER_SECRET_KEY=$(echo $BLOCK_VERIFIER_SECRET_KEY | awk -F '--block-verifiers-secret-key' '{print $2}')
    BLOCK_VERIFIER_SECRET_KEY=${BLOCK_VERIFIER_SECRET_KEY:1:$BLOCK_VERIFIERS_SECRET_KEY_LENGTH}
    BLOCK_VERIFIER_PUBLIC_KEY="${BLOCK_VERIFIER_SECRET_KEY: -${BLOCK_VERIFIERS_PUBLIC_KEY_LENGTH}}"
    # Run the wallet passing the registration information  
    (echo "delegate_register ${XCASH_DELEGATE_NAME} ${XCASH_DELEGATE_DOMAIN} ${BLOCK_VERIFIER_PUBLIC_KEY}"; echo "${WALLET_PASSWORD}"; echo "exit" ) | ${XCASH_DIR}build/release/bin/xcash-wallet-cli --wallet-file ${XCASH_WALLET_DIR}delegate-wallet --password ${WALLET_PASSWORD} --trusted-daemon --log-file ${XCASH_LOGS_DIR}xcash-wallet-rpc.log
    # Start the rpc wallet service
    sudo systemctl start xcash-rpc-wallet &>/dev/null
  fi
  echo
  echo -ne "${COLOR_PRINT_YELLOW}Do you want to update the delegate information? Leave empty for YES, write N for NO: ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  if [ "$data" == "" ]; then
    if [ "$XCASH_DELEGATE_NAME" == "" ]; then
      # Get required information
      get_installation_directory
      echo -ne "${COLOR_PRINT_YELLOW}Enter your delegate name: ${END_COLOR_PRINT}"
      read -r XCASH_DELEGATE_NAME
      echo -ne "\r"
      echo
    fi
    echo -e "${COLOR_PRINT_GREEN}Please see https://docs.xcash.foundation/dpops/register-delegate${END_COLOR_PRINT}"
    echo -e "${COLOR_PRINT_GREEN}For best compatibility use only alphanumeric characters and . , ! ? - _${END_COLOR_PRINT}"
    echo -ne "${COLOR_PRINT_YELLOW}Enter new domain name or IP (leave empty to skip): ${END_COLOR_PRINT}"
    read -r UPDATE_NEW_DOMAIN_IP
    echo -ne "\r"
    echo
    echo -ne "${COLOR_PRINT_YELLOW}Enter About description (leave empty to skip): ${END_COLOR_PRINT}"
    read -r UPDATE_ABOUT_DESCRIPTION
    echo -ne "\r"
    echo
    echo -ne "${COLOR_PRINT_YELLOW}Enter Website - Landing page (leave empty to skip): ${END_COLOR_PRINT}"
    read -r UPDATE_WEBSITE
    echo -ne "\r"
    echo
    echo -ne "${COLOR_PRINT_YELLOW}Enter Shared delegate status, true or false (leave empty to skip): ${END_COLOR_PRINT}"
    read -r UPDATE_SHARED_DELEGATE_STATUS
    echo -ne "\r"
    echo
    echo -ne "${COLOR_PRINT_YELLOW}Enter Shared delegate Fee (leave empty to skip): ${END_COLOR_PRINT}"
    read -r UPDATE_SHARED_DELEGATE_FEE
    echo -ne "\r"
    echo
    echo -ne "${COLOR_PRINT_YELLOW}Enter Shared Delegate Team Info (leave empty to skip): ${END_COLOR_PRINT}"
    read -r UPDATE_TEAM
    echo -ne "\r"
    echo
    echo -ne "${COLOR_PRINT_YELLOW}Enter Server Specifications (leave empty to skip): ${END_COLOR_PRINT}"
    read -r UPDATE_SERVER_SPECS
    echo -ne "\r"
    echo
    # Stop the rpc wallet service
    sudo systemctl stop xcash-rpc-wallet &>/dev/null
    # Disable ask-password inside the wallet (will be re-enabled at the end of the configuration)
    COMMAND_STRING="${COMMAND_STRING}set ask-password 0\n${WALLET_PASSWORD}\n"
    (echo -ne ${COMMAND_STRING}; echo "exit" ) | ${XCASH_DIR}build/release/bin/xcash-wallet-cli --wallet-file ${XCASH_WALLET_DIR}delegate-wallet --password ${WALLET_PASSWORD} --trusted-daemon --log-file ${XCASH_LOGS_DIR}xcash-wallet-rpc.log
    echo
    # Create the update sequence
    COMMAND_STRING=""
    if [ ! "$UPDATE_NEW_DOMAIN_IP" == "" ]; then COMMAND_STRING="${COMMAND_STRING}delegate_update IP_address ${UPDATE_NEW_DOMAIN_IP}\n"; fi
    if [ ! "$UPDATE_ABOUT_DESCRIPTION" == "" ]; then COMMAND_STRING="${COMMAND_STRING}delegate_update about ${UPDATE_ABOUT_DESCRIPTION}\n"; fi
    if [ ! "$UPDATE_WEBSITE" == "" ]; then COMMAND_STRING="${COMMAND_STRING}delegate_update website ${UPDATE_WEBSITE}\n"; fi
    if [ ! "$UPDATE_SHARED_DELEGATE_STATUS" == "" ]; then COMMAND_STRING="${COMMAND_STRING}delegate_update shared_delegate_status ${UPDATE_SHARED_DELEGATE_STATUS}\n"; fi
    if [ ! "$UPDATE_SHARED_DELEGATE_FEE" == "" ]; then COMMAND_STRING="${COMMAND_STRING}delegate_update delegate_fee ${UPDATE_SHARED_DELEGATE_FEE}\n"; fi
    if [ ! "$UPDATE_TEAM" == "" ]; then COMMAND_STRING="${COMMAND_STRING}delegate_update team ${UPDATE_TEAM}\n"; fi
    if [ ! "$UPDATE_SERVER_SPECS" == "" ]; then COMMAND_STRING="${COMMAND_STRING}delegate_update server_specs ${UPDATE_SERVER_SPECS}\n"; fi
    COMMAND_STRING="${COMMAND_STRING}set ask-password 1\n${WALLET_PASSWORD}\n"
    # Run the wallet passing the registration information  
    (echo -ne ${COMMAND_STRING}; echo "exit" ) | ${XCASH_DIR}build/release/bin/xcash-wallet-cli --wallet-file ${XCASH_WALLET_DIR}delegate-wallet --password ${WALLET_PASSWORD} --trusted-daemon --log-file ${XCASH_LOGS_DIR}xcash-wallet-rpc.log
    # Start the rpc wallet service
    sudo systemctl start xcash-rpc-wallet &>/dev/null
  fi
  echo
  echo -e "${COLOR_PRINT_GREEN}Operation completed!${END_COLOR_PRINT}"
}



function create_swap_file()
{
  echo -ne "${COLOR_PRINT_YELLOW}Creating Swap File${END_COLOR_PRINT}"
  cd $HOME
  sudo fallocate -l 8G /swapfile || true
  sudo chmod 600 /swapfile || true
  sudo mkswap /swapfile &>/dev/null || true
  sudo swapon /swapfile || true
  echo "/swapfile swap swap defaults 0 0" | sudo tee -a /etc/fstab &>/dev/null || true
  sudo sysctl -w vm.swappiness=1 &>/dev/null || true
  echo -ne "\r${COLOR_PRINT_GREEN}Creating Swap File${END_COLOR_PRINT}"
  echo
}

# Check for a compatible OS
check_ubuntu_version

# Setup profile if running from an LXC container
setup_lxc_container_profile

# Get the installation settings
installation_settings

if [ "$INSTALLATION_TYPE_SETTINGS" -eq "1" ]; then
  install
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "2" ]; then
  update
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "3" ]; then
  uninstall
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "4" ]; then
  install_node
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "5" ]; then
  update_node
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "6" ]; then
  uninstall_node
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "7" ]; then
  install_or_update_blockchain
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "8" ]; then
  change_solo_or_shared_delegate
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "9" ]; then
  edit_shared_delegate_settings
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "10" ]; then
  register_update_delegate
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "11" ]; then
  stop_systemd_service_files
  start_systemd_service_files
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "12" ]; then
  stop_systemd_service_files
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "13" ]; then
  test_update
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "14" ]; then
  test_update_reset_delegates
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "15" ]; then
  configure
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "16" ]; then
  install_firewall_script
elif [ "$INSTALLATION_TYPE_SETTINGS" -eq "17" ]; then
  install_firewall_script_shared_delegates
fi

