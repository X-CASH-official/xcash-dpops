# Save in /root/Installed-Programs/scripts/restart_script.sh
# Create a launcher on the desktop for it with the command /root/Installed-Programs/scripts/restart_script.sh and dont run it in a terminal
#!/bin/bash
 
# Set the script to exit if any command fails
set -e
 
# Configuration get_install_settings
XCASH_DIR="/root/Installed-Programs/X-CASH/build/release/bin/"
XCASH_BLOCKCHAIN_DIR="/root/Installed-Programs/XCASH_BLOCKCHAIN/"
XCASH_WALLETS_DIR="/root/Installed-Programs/xcash_wallets/"
MONGODB_DIR="/root/Installed-Programs/mongodb-linux-x86_64-ubuntu1804-4.2.1/bin/"
IP_ADDRESS=$(hostname -I | awk '{print $1}')
MAIN_NODES_IP_ADDRESS="192.168.1.201"
XCASH_DPOPS_BLOCK_HEIGHT=521850

# reset the blockchain height
data=$(curl -s -X POST http://127.0.0.1:18281/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"get_block_count"}' -H 'Content-Type: application/json')
data="${data:66:6}"
data=$(($data-$XCASH_DPOPS_BLOCK_HEIGHT))
if [ ! $data -le 0 ]; then
  ${XCASH_DIR}xcash-blockchain-import --data-dir ${XCASH_BLOCKCHAIN_DIR} --pop-blocks ${data}
fi

# reset the database
(echo "use XCASH_PROOF_OF_STAKE"; echo "db.reserve_bytes_1.drop()"; echo "exit";) | "${MONGODB_DIR}"mongo
(echo "use XCASH_PROOF_OF_STAKE"; echo "db.reserve_bytes_2.drop()"; echo "exit";) | "${MONGODB_DIR}"mongo
(echo "use XCASH_PROOF_OF_STAKE"; echo "db.reserve_bytes_3.drop()"; echo "exit";) | "${MONGODB_DIR}"mongo
(echo "use XCASH_PROOF_OF_STAKE"; echo "db.reserve_bytes_4.drop()"; echo "exit";) | "${MONGODB_DIR}"mongo
(echo "use XCASH_PROOF_OF_STAKE"; echo "db.reserve_bytes_5.drop()"; echo "exit";) | "${MONGODB_DIR}"mongo

# reset xcashd and xcash-wallet-rpc
curl -X POST http://127.0.0.1:18281/stop_daemon -H 'Content-Type: application/json'
sleep 20s
curl -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"stop_wallet"}' -H 'Content-Type: application/json'
sleep 20s
pkill -f xcashd
pkill -f xcash-wallet-rpc
if [ $IP_ADDRESS == $MAIN_NODES_IP_ADDRESS ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
else
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --allow-local-ip --add-exclusive-node ${MAIN_NODES_IP_ADDRESS} --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
fi
sleep 20s
xfce4-terminal -e "bash -c \"${XCASH_DIR}xcash-wallet-rpc --wallet-file ${XCASH_WALLETS_DIR}wallet1 --password password --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon; bash\"" -T "xcash-wallet-rpc" &