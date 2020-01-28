#!/bin/bash

# Processes
MONGODB_PROCESS=""
XCASH_DAEMON_PROCESS=""
XCASH_WALLET_PROCESS=""
XCASH_DPOPS_PROCESS=""

count=0
XCASH_DAEMON_RESTART=21600

# make sure they have all of the processes started before running the script
data=$(ps -eaf)
if [[ ! $data =~ "SCREEN -dmS MongoDB" ]]; then
   echo "Please start MongoDB before running the autorestart script"
fi
if [[ ! $data =~ "SCREEN -dmS XCASH_Daemon" ]]; then
    echo "Please start XCASH_Dameon before running the autorestart script"
fi
if [[ ! $data =~ "SCREEN -dmS XCASH_Wallet" ]]; then
    echo "Please start XCASH_Wallet before running the autorestart script"
fi
if [[ ! $data =~ "SCREEN -dmS XCASH_DPOPS" ]]; then
    echo "Please start XCASH_DPOPS before running the autorestart script"
fi

# Update the processes
MONGODB_PROCESS=$(ps -eo args | grep MongoDB | head -n -1)
XCASH_DAEMON_PROCESS=$(ps -eo args | grep XCASH_Daemon | head -n -1)
XCASH_WALLET_PROCESS=$(ps -eo args | grep XCASH_Wallet | head -n -1)
XCASH_DPOPS_PROCESS=$(ps -eo args | grep XCASH_DPOPS | head -n -1)

# Restart a process if it fails
while true; do
data=$(ps -eaf)
if [[ ! $data =~ "/root/x-network/mongodb" ]]; then
   eval "screen${MONGODB_PROCESS:6}"
fi
if [[ ! $data =~ "/root/x-network/X-CASH/build/release/bin/xcashd" ]]; then
   eval "screen${XCASH_DAEMON_PROCESS:6}"
fi
if [[ ! $data =~ " /root/x-network/X-CASH/build/release/bin/xcash-wallet-rpc" ]]; then
   eval "screen${XCASH_WALLET_PROCESS:6}"
fi
if [[ ! $data =~ "/root/x-network/XCASH_DPOPS/build/XCASH_DPOPS" ]]; then
   eval "screen${XCASH_DPOPS_PROCESS:6}"
fi
if [[ $count -eq $XCASH_DAEMON_RESTART ]]; then
  screen -XS "XCASH_Daemon" quit
  eval "screen${XCASH_DAEMON_PROCESS:6}"
  count=0
fi
sleep 60
done