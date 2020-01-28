# Save in /root/Installed-Programs/scripts/shutdown_script.sh
# Create a launcher on the desktop for it with the command /root/Installed-Programs/scripts/shutdown_script.sh and dont run it in a terminal
#!/bin/bash
 
curl -X POST http://127.0.0.1:18281/stop_daemon -H 'Content-Type: application/json'
sleep 20s
curl -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"stop_wallet"}' -H 'Content-Type: application/json'
sleep 20s
pkill -f mongod
pkill -f mongo
pkill -f xcash-wallet-rpc
pkill -f xcashd
pkill -f "/usr/share/code/code --user-data-dir /root/Installed-Programs/vscode/"
shutdown now