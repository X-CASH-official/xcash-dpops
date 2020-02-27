#!/bin/bash
 
# Set the script to exit if any command fails
set -e
 
# Configuration get_install_settings
XCASH_DIR="/root/Installed-Programs/X-CASH/build/release/bin/"
XCASH_BLOCKCHAIN_DIR="/root/Installed-Programs/XCASH_BLOCKCHAIN/"
XCASH_WALLETS_DIR="/root/Installed-Programs/xcash_wallets/"
MONGODB_DIR="/root/Installed-Programs/mongodb-linux-x86_64-ubuntu1804-4.2.1/bin/"
VSCODE_DATA_DIR="/root/Installed-Programs/vscode/"
IP_ADDRESS=$(hostname -I | awk '{print $1}')
MAIN_NODES_IP_ADDRESS="192.168.1.201"

# start the internet
dhclient enp0s3 -v
sudo netplan apply

# sync the time
timedatectl set-ntp true
systemctl restart systemd-timesyncd

xfce4-terminal -e "bash -c \"${MONGODB_DIR}mongod; bash\"" -T "mongod" &
sleep 10s
xfce4-terminal -e "bash -c \"${MONGODB_DIR}mongo; bash\"" -T "mongo" &
sleep 10s
if [ "$IP_ADDRESS" == $MAIN_NODES_IP_ADDRESS ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
else
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --allow-local-ip --add-exclusive-node ${MAIN_NODES_IP_ADDRESS} --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
fi
sleep 20s
xfce4-terminal -e "bash -c \"${XCASH_DIR}xcash-wallet-rpc --wallet-file ${XCASH_WALLETS_DIR}wallet1 --password password --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon; bash\"" -T "xcash-wallet-rpc" &
cp -a "$HOME"/.vscode/extensions/ms* "${VSCODE_DATA_DIR}"
sleep 10s
code --user-data-dir "${VSCODE_DATA_DIR}"
