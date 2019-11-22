#!/bin/bash

# Color print variables
COLOR_PRINT_GREEN="\033[1;32m"
COLOR_PRINT_YELLOW="\033[1;33m"
END_COLOR_PRINT="\033[0m"

# Configuration settings
MONGODB_INSTALLATION_DIR="/root/x-network/mongodb-linux-x86_64-ubuntu1804-4.2.1/"
XCASH_DPOPS_INSTALLATION_DIR="/root/x-network/XCASH_DPOPS/"
XCASH_WALLETS_INSTALLATION_DIR="/root/x-network/xcash_wallets/"
XCASH_INSTALLATION_DIR="/root/x-network/X-CASH/"
BLOCK_VERIFIER_KEY_SETTINGS=""
BLOCK_VERIFIER_SECRET_KEY="00"
BLOCK_VERIFIER_PUBLIC_KEY=""
WALLET_SETTINGS="0"
WALLET_SEED=""
WALLET_PASSWORD_SETTINGS=""
WALLET_PASSWORD=$(< /dev/urandom tr -dc 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789' | head -c"${1:-32}";echo;)

# Files
BLOCK_VERIFIERS_SIGN_AND_VERIFY_MESSAGES_FILE=""

# Settings
BLOCK_VERIFIERS_PUBLIC_KEY_LENGTH=64
BLOCK_VERIFIERS_SECRET_KEY_LENGTH=128
CPU_THREADS=$(nproc)

# Regex
regex_MNEMONIC_SEED="^\b([a-z]+\s+){24}\b([a-z]+)$" # 25 words exactly



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
  echo -e "${COLOR_PRINT_GREEN}                    Configuration Settings${END_COLOR_PRINT}"
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

function installation_settings()
{
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}    Welcome to X-Cash DPoPS Docker Configuration Script ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  get_wallet_settings
  get_password_settings
  get_block_verifier_key_settings
  print_installation_settings
}










function start_processes()
{
  echo -ne "${COLOR_PRINT_YELLOW}Starting Processes${END_COLOR_PRINT}"
  screen -dmS MongoDB ${MONGODB_INSTALLATION_DIR}bin/mongod --dbpath /data/db/
  screen -dmS XCASH_Daemon ${XCASH_INSTALLATION_DIR}build/release/bin/xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind
  sleep 30s
  screen -dmS XCASH_Wallet ${XCASH_INSTALLATION_DIR}build/release/bin/xcash-wallet-rpc --wallet-file ${XCASH_WALLETS_INSTALLATION_DIR}XCASH_DPOPS_WALLET --password "${WALLET_PASSWORD}" --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon
  sleep 30s
  echo -ne "\r${COLOR_PRINT_GREEN}Starting Processes${END_COLOR_PRINT}"
  echo
}

function stop_processes()
{
  echo -ne "${COLOR_PRINT_YELLOW}Stoping Processes${END_COLOR_PRINT}"
  screen -XS "MongoDB" quit > /dev/null 2>&1
  screen -XS "XCASH_Daemon" quit > /dev/null 2>&1
  screen -XS "XCASH_Wallet" quit > /dev/null 2>&1
  sleep 30s
  echo -ne "\r${COLOR_PRINT_GREEN}Stoping Processes${END_COLOR_PRINT}"
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

function update_block_verifiers_sign_and_verify_messages_file()
{
BLOCK_VERIFIERS_SIGN_AND_VERIFY_MESSAGES_FILE="$(cat << EOF
#ifndef BLOCK_VERIFIERS_SIGN_AND_VERIFY_MESSAGES_H_
#define BLOCK_VERIFIERS_SIGN_AND_VERIFY_MESSAGES_H_
#define BLOCK_VERIFIERS_SECRET_KEY "${BLOCK_VERIFIER_SECRET_KEY}"
#endif
EOF
)"
}

function build_xcash_dpops_with_block_verifiers_key()
{
  echo -ne "${COLOR_PRINT_YELLOW}Building XCASH_DPOPS With Block Verifiers Key${END_COLOR_PRINT}"
  cd ${XCASH_DPOPS_INSTALLATION_DIR} || exit
  echo "a" > src/global_data/block_verifiers_sign_and_verify_messages.h
  echo "${BLOCK_VERIFIERS_SIGN_AND_VERIFY_MESSAGES_FILE}" > src/global_data/block_verifiers_sign_and_verify_messages.h
  make clean > /dev/null 2>&1
  make release -j "${CPU_THREADS}" > /dev/null 2>&1
  echo -ne "\r${COLOR_PRINT_GREEN}Building XCASH_DPOPS With Block Verifiers Key${END_COLOR_PRINT}"
  echo
}










function create_xcash_wallet()
{
  echo -ne "${COLOR_PRINT_YELLOW}Creating X-CASH Wallet (This might take a while)${END_COLOR_PRINT}"
  screen -dmS XCASH_Daemon ${XCASH_INSTALLATION_DIR}build/release/bin/xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind
  sleep 30s
  echo "exit" | ${XCASH_INSTALLATION_DIR}build/release/bin/xcash-wallet-cli --generate-new-wallet ${XCASH_WALLETS_INSTALLATION_DIR}XCASH_DPOPS_WALLET --password "${WALLET_PASSWORD}" --mnemonic-language English --restore-height 0 > /dev/null 2>&1
  screen -XS "XCASH_Daemon" quit > /dev/null 2>&1
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
  echo -ne "\r${COLOR_PRINT_GREEN}Importing X-CASH Wallet (This might take a while)${END_COLOR_PRINT}"
  echo
}

function get_xcash_wallet_data()
{
  echo -ne "${COLOR_PRINT_YELLOW}Getting X-CASH Wallet Data${END_COLOR_PRINT}"

  screen -dmS XCASH_Daemon ${XCASH_INSTALLATION_DIR}build/release/bin/xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind
  sleep 30s
  screen -dmS XCASH_RPC_Wallet ${XCASH_INSTALLATION_DIR}build/release/bin/xcash-wallet-rpc --wallet-file ${XCASH_WALLETS_INSTALLATION_DIR}XCASH_DPOPS_WALLET --password "${WALLET_PASSWORD}" --rpc-bind-port 18285 --confirm-external-bind --disable-rpc-login --trusted-daemon
  sleep 10s
  
   while
    data=$(curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"get_address"}' -H 'Content-Type: application/json') 
    sleep 10s
    [[ "$data" == "" ]]
  do true; done

  PUBLIC_ADDRESS=$(curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"get_address"}' -H 'Content-Type: application/json' | grep \"address\" | head -1 | sed s"|    \"address\": ||g" | sed s"|\"||g" | sed s"|,||g")
  SPEND_KEY=$(curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"query_key","params":{"key_type":"spend_key"}}' -H 'Content-Type: application/json' | grep \"key\" | sed s"|    \"key\": ||g" | sed s"|\"||g")
  VIEW_KEY=$(curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"query_key","params":{"key_type":"view_key"}}' -H 'Content-Type: application/json' | grep \"key\" | sed s"|    \"key\": ||g" | sed s"|\"||g")
  MNEMONIC_SEED=$(curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"query_key","params":{"key_type":"mnemonic"}}' -H 'Content-Type: application/json' | grep \"key\" | sed s"|    \"key\": ||g" | sed s"|\"||g")
  
  curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"stop_wallet"}' -H 'Content-Type: application/json' > /dev/null 2>&1
  sleep 10s

  screen -XS "XCASH_Daemon" quit > /dev/null 2>&1
  
  echo -ne "\r${COLOR_PRINT_GREEN}Getting X-CASH Wallet Data${END_COLOR_PRINT}"
  echo
}










function install()
{
  installation_settings

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

  update_block_verifiers_sign_and_verify_messages_file
  build_xcash_dpops_with_block_verifiers_key

  stop_processes

  get_xcash_wallet_data

  # Display Configuration data
  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}          Configuration Has Completed Successfully  ${END_COLOR_PRINT}"
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



install