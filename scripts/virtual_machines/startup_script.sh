#!/bin/bash
 
# Set the script to exit if any command fails
set -e
 
# Configuration get_install_settings
XCASH_DIR="/root/xcash-official/X-CASH/build/release/bin/"
XCASH_BLOCKCHAIN_DIR="/root/xcash-official/XCASH_BLOCKCHAIN/"
XCASH_WALLETS_DIR="/root/xcash-official/xcash-wallets/"
MONGODB_DIR="/root/xcash-official/mongodb-linux-x86_64-ubuntu1804-4.2.1/bin/"
VSCODE_DATA_DIR="/root/xcash-official/vscode/"
IP_ADDRESS=$(hostname -I | awk '{print $1}')
MAIN_NODES_IP_ADDRESS="192.168.1.201"

TEST_WALLET_1="XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP"
TEST_WALLET_2="XCA1VSDHKCc4Qhvqb3fquebSYxfMeyGteQeAYtDSpaTcgquBY1bkKWtQ42tZG2w7Ak7GyqnaiTgWL4bMHE9Lwd2A3g2Recxz7B"
TEST_WALLET_3="XCA1f8ngVg6fW5pJ49TC3DK4axYDMu5teUKUf7aP5rLCRvsL1ZCnf2LjAFtSYF6xfVWygSMMvn1hCEeupgGTX5n82GDZvcapbj"
TEST_WALLET_4="XCA1skiymYUHN5Vjg5kXhriGi25ZDKpgdLMZks3DKCwy9sxzyqY7uEr6hxRPnAvkYwLoT6peBi4aVT1g4t4vgtFj96eE1JF1L4"
TEST_WALLET_5="XCA1c8vbHxUiFfgnp7P5pPaAWEJac1W8vjiNHSLRB1k7G6XrWQkWXy85RfefFiCzB8V43jopp5AwmcezSoUVbXcp8Z4Eki8Gmt"
TEST_WALLET_6="XCA1jh9Nw2XbshVEYsSrnEX5q6WWzHkMaEgRd2TSkrcGckvqoqxoQZejnv31SZPgVjL9uVRvX4NYsPbCLjMdT4m58kmgCnN63n"
TEST_WALLET_7="XCA1ZWK9xxe9EtCHG1H5gW72yLndfRdveHC7SSLqqPU9U9G5RZ5M4LC3xiuLepRMzcTzwhJvwS9MVCDFHby3LtNk3VKz9RDbGU"
TEST_WALLET_8="XCA1UU85yYx5EmQ52g84TDDVkiVKSiwn6JAC1xSHNd2UCxPBiei9yHcDpnwwVDvfEeXm9RYGuD63h8nx3zjD7PSU47QHJn3fFk"
TEST_WALLET_9="XCA1vNNSR2M5QjD7NK6VGtdCrLLAazwt1Qc9qxrRBPwoWCz9GraMCsu4wH6v6W5Td1DoAbyA47AgpEigt6UoXE4H1wS7aiasjf"
TEST_WALLET_10="XCA1ogpCudEVS1B72pFWfKQQ7gMXyePgjhc5PLaQuHmmgjPZAfeT2jLPdv7SW6qCBu42aXR3Tp1zWeJTeUXWXJG74t26j3awwn"
TEST_WALLET_11="XCA1oPZcuxH1WeyMntcv1Mb28Nzc73UvMbck3VyBedN57gV1c9ikU6tBALSjCwigMs1XCDtTYND7vfELU31PQEBh6zs5MLxYek"
TEST_WALLET_12="XCA1S1T6abaY3xy2K8bTdi3ZnpdtJmNAEWCB6SLxfFCTbRjn9ukRMGyaCgBa8BFf5B9HM4dyyEAhmL94ne7RqYQo6w3PuA4omv"
TEST_WALLET_13="XCA1ThLqpF3eXjZ5pJPmRtPifbCthUwmp2dpMPjJyveQ6U9FFsm1RJQecTJ41n5ezv2NmkNB9gwN4RHbMPKdNpquA68g1bSrRS"
TEST_WALLET_14="XCA1pvUcSBDUkijSLa9vaHbDudrEcT88QdLBLzt4n9UqPb1wWxtnh9u8VPzCZMg2TXh8TdR1zphWi3qxooKDG82k8YkFwpB3BD"
TEST_WALLET_15="XCA1dpsUrcgZvnZYLjXUbcbxSSJqhNWyphoqURumeeKoE8BXQeKqep8QQSk2fd65at2yspnT3z7KVRTU3MFmVS8u7K89wrKVxu"
TEST_WALLET_16="XCA1XUVpDPKXKH5T6s9bX3CnSS46pidyTQAzjmkNyf9H4d3ajARrQFr1CQM4La8n8ggzf6GDKVDtua3CufEqcDJz1NLSvp4Ung"
TEST_WALLET_17="XCA1V28tnHEETmewJvoJB57URT7xzBowJ9JJwj29xxGme21DQgXhUWTCx2XvxJR2EtecZxYRiHdTtGnzZVqBKqyt9K4MTkZuyo"
TEST_WALLET_18="XCA1VXWDkYm2u56cbe6b3VajEe4qFLGhVgxeNcgnA3ht1gWvrnZTYr8PJtpkWzagR7c76dDdeyFgoeKZj55W78bp4T8L8n6wLW"
TEST_WALLET_19="XCA1qbzqCoX9WGsqjYgKMGH2eFjkvBRmsb2HKiumrKbjMVDLhukUShsgjFbT6osQBA3TaL9hD7wr8dW2Zq5QzRyU1mnQAQwTpJ"
TEST_WALLET_20="XCA1u5AWxhjCtRKzZiyUUmdHLnQFQdFcZGtuFxx9JW21hWJnzgf31AV2g58gUsN3aKSju1iL9RdzbHJqvUKjVYBh67e74Zc38v"

TEST_WALLET_SECRET_KEY_1="dc348f17eba0c7e43236b59e41c3e3c88a232cbfbb44c9dedb69100906d95a613f7d2dcabdbf351bf8aef4202711a4332b2a46c18dc92b1502eeacfa74c49163"
TEST_WALLET_SECRET_KEY_2="7affb8c08d0d8650507c132c1ddf90ab615b3395ed9531691d7ae6dda3be1c9cb1962712c6bd9294ac426196b2433aad0a39a67485b233f766f888ddb6b94359"
TEST_WALLET_SECRET_KEY_3="62fb423d711de76cf533c130ce2ad59f867a6893a14d2ccf155d8abd4db0c2aea1e3d6f49dee438f447412b59f1625a04e3f15897edf6313dce1a19e2ef8343f"
TEST_WALLET_SECRET_KEY_4="41beafa5e361b1f737201deb10d8114bab5b2a2b1f7bcf5fef4eab3e9bbfc2dc59f4608dd31cf0811feeb5ee15030d6de8e0ef2fea78d2fa39c5620ae71f15b5"
TEST_WALLET_SECRET_KEY_5="2a6d0e69fa01c55bc7723b2dc998d0920c2108c6e751f57f25202047aebd7ad8029af930d9b5930e3f56168c42fd874cb386f618b5681dd8aa3640f506ef55ce"
TEST_WALLET_SECRET_KEY_6="5bd622a988b6a080a7b04cacb30570ce296bfc3b13aa7e334f18c38c661ffec1cc136628a8eff7301be1f553f9e5e784c7b87b14a981dafde5b5e25afecd2436"
TEST_WALLET_SECRET_KEY_7="bf93308c0f94350f1a19d45fe0f1dc798d07663307e5b6f37d291d5cbb76bd7ad97aec73d77c44ed27315d7a32b8e620438e3e7a17dc670b8ce97501bbc5d94b"
TEST_WALLET_SECRET_KEY_8="2c5138cb616ffe228773095af7aa85eadcb268290aa921d93a4050332f9b2ad90b0b8bc13a792e9ef6ae16e7d177f4f3f9d253c6a7874515ec7e355ed2c4f76e"
TEST_WALLET_SECRET_KEY_9="511f27fcfaa66cedba32b9c0e4a9dfde6e16037cf44fe3b12b09c02fd868bd2975b5564334355355027b3ff18d74b57e54e351e85ae90fb9d646fb471b5f5c35"
TEST_WALLET_SECRET_KEY_10="30658d02a511fb798291e0cb99413086a4158c4422f70c9e2357bf69a8d0e05732f97e5e95a0d0047c47d3bb341d38605dd84c7ea6e18ff823ea4d3c9d4e44c3"
TEST_WALLET_SECRET_KEY_11="4d76fd51502fc31070e91b3ab6e54eaece620bfe165ff0356b404e1e5617f923cab0ac887926d4a01d9cc62ad14f804add8270c288f8ca3c9bfd3cb9b2c046bc"
TEST_WALLET_SECRET_KEY_12="5d9d07ad01ce4f72d27228e3bdb97088ee1b9060b862cb219a732b3c7758aad3391ff49ca3df0c36d82c6cdc73e3bdfb998a556bf67eb78981764f5c5a630753"
TEST_WALLET_SECRET_KEY_13="7420705afa7385b60d113c2fb3dbf4e0ba365225792bb35869350a382122d795f7593740aa53df6e0d0a6e9bec1da94ecfba5b05c1fc40820b30e8e8c80d1242"
TEST_WALLET_SECRET_KEY_14="71cc61515538d7da05744f6c4f51c9ca318f3db97219d015157bf609fb43806ca38779062167f496f4edb944096ea45967d85b373bcf8aa1e06431c7f8d5b0b2"
TEST_WALLET_SECRET_KEY_15="adc155e5088bd5da2ba0c192a97442022ce69bb54637b3317caa6301e3fe38904c705dcc4cf579b11a76e9d2b16c726d415d86c715d0908893d96c5e2716bf25"
TEST_WALLET_SECRET_KEY_16="2254d0f8704640c5285b1fca82523815e11922949ba9bc9355bf9d5b058aaea512fc1eec85dfb2aeaf39e8379022d67f34e68aaff3de1ec18dd02330c42a1e11"
TEST_WALLET_SECRET_KEY_17="f111e34dc598bd19aaa270962a2830aefdacc1e665378e96f2ec271f0cc4a97c01838d3f43927e91c2ec2053ad8df92af8466b4f3a6e649d532c258939de1490"
TEST_WALLET_SECRET_KEY_18="9fa0bb3688d3c5529ef33abea151829a42ff4c9a769c96a303333ef311fd6daf6f765c5976d3ff2cf0d93727cb199d7b404502f90301ee0a4c3a7fc83da84724"
TEST_WALLET_SECRET_KEY_19="4412a9fa1633102dc31c9f3b305da941fc2aab6c6e79f903cf546ab1a114a6e4fdf5f9397e9a3494db33dc61127860083db794d1f32f0359665d46388d355af8"
TEST_WALLET_SECRET_KEY_20="b18d7b404e6fb08dbe3d67f32478ee48269a0e22c0cac5c6a8e2eff1f6b590a7738f0533a60105f47b15e46866cf3ecd5cfd265d2ec5f2a7c8900cc3bba391ec"

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

# test IP address anti DDOS
#if [ "$IP_ADDRESS" == $MAIN_NODES_IP_ADDRESS ]; then
#  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
#else
#  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
#fi

# test public address anti DDOS
if [ "$IP_ADDRESS" == $MAIN_NODES_IP_ADDRESS ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_1} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_1} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.202" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_2} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_2} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.203" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_3} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_3} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.204" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_4} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_4} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.205" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_5} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_5} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.206" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_6} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_6} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.207" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_7} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_7} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.208" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_8} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_8} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.209" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_9} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_9} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.210" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_10} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_10} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.211" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_11} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_11} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.212" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_12} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_12} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.213" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_13} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_13} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.214" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_14} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_14} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.215" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_15} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_15} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.216" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_16} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_16} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.217" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_17} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_17} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.218" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_18} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_18} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.219" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_19} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_19} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
elif [ "$IP_ADDRESS" == "192.168.1.220" ]; then
  xfce4-terminal -e "bash -c \"${XCASH_DIR}xcashd --xcash_dpops_delegates_public_address ${TEST_WALLET_20} --xcash_dpops_delegates_secret_key ${TEST_WALLET_SECRET_KEY_20} --allow-local-ip --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --data-dir ${XCASH_BLOCKCHAIN_DIR}; bash\"" -T "xcashd" &
fi

sleep 20s
xfce4-terminal -e "bash -c \"${XCASH_DIR}xcash-wallet-rpc --wallet-file ${XCASH_WALLETS_DIR}wallet1 --password password --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon; bash\"" -T "xcash-wallet-rpc" &
cp -a "$HOME"/.vscode/extensions/ms* "${VSCODE_DATA_DIR}"
sleep 10s
code --user-data-dir "${VSCODE_DATA_DIR}"
