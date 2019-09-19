# X-CASH Delegated Proof Of Privacy Stake (DPOPS)
 
X-CASH DPOPS is a variation of DPOS and DBFT. The key features of X-CASH DPOPS are:
 
* The top 100 delegates are elected as block verifiers.
* Reserve proof based voting/staking system, meaning the XCASH always stays in your wallet.
* No need to keep your wallet or computer online if your just staking towards a shared delegate.
* No need to keep your XCASH directly on the server if running a solo node, as you can use an empty wallet on the server and vote for yourself.
* A minimum of 2 million XCASH is needed to vote for a delegate.
 
* The election process is every block, meaning a new vote will get counted for the next block.
* No need to cancel a vote, as it will automatically get cancelled if you change your vote to a different delegate.
* No lockup times, the xcash always remains in your wallet and you can use them at any time, although moving them from your wallet will cancel your entire staking vote.
* No fees for voting, and you can revote or switch your vote as many times as you like.
 
* Using a variation of Delegated Byzantine Fault tolerance consensus where 67% consensus must be reached for a new block to be added to the network.
* DBFT allows for up to 33% of the elected block verifiers to stop working, and the system will still be able to produce a new block.
* Using Verifiable Random Functions to select the next block producer in the system. This allows for a random, but verifiable way of selecting the next block producer.
* Blocks can be verified in the XCASH Daemon with a detailed explanation of the reserve bytes in the block.
 
* Using a decentralized database system, to hold all of the voting data and reserve bytes data.
* The block format is to only store a hash of the contents of the reserve bytes data in the block, and store the actual reserve bytes data in the decentralized datbase, to reduce the extra size of the blockchain, while keeping all of the rounds data.
 
This program allows one to run a DPOPS node, a shared delegates website, and a delegates website.

**If you plan on running a shared delegates website or a delegates website, you will need to run the website on the same system as the DPOPS node**

**By running a DPOPS node (solo or shared) you will need the computer to be online and running at all times**
 
## System Requirements
 
XCASH DPOPS will only run on a Linux/Unix OS at this time. We recommend installing this on a Ubuntu VPS/dedicated server (18.04) for the best compatibility.
 
**Minimum System Requirements:**  
Operating System: Ubuntu 18.04 (or higher)  
CPU: 4 threads  
RAM: 8GB  
Hard drive: 50GB  
Bandwidth Transfer: 500GB per month  
Bandwidth Speed: 30 Mbps
 
**Recommended System Requirements:**  
Operating System: Ubuntu 18.04 (or higher)  
CPU: 8 threads  
RAM: 16GB  
Hard drive: 100GB  
Bandwidth Transfer: 2TB per month  
Bandwidth Speed: 100 Mbps
 
 
## Dependencies
 
The following table summarizes the tools and libraries required to run XCASH DPOPS
 
| Dependencies                                 | Min. version  | Ubuntu package            |
| -------------------------------------------- | ------------- | ------------------------- |
| GCC                                          | 4.7.3         | `build-essential`         |
| CMake                                        | 3.0.0         | `cmake`                   |
| pkg-config                                   | any           | `pkg-config`              |
| OpenSSL                                      | any           | `libssl-dev`              |
| Git                                          | any           | `git`                     |
| MongoDB                                      | 4.0.3         |  install from binaries    |
| MongoDB C Driver (includes BSON libary)      | 1.13.1        |  build from source        |
| XCASH                                        | latest version         |  [download the latest release](https://github.com/X-CASH-official/X-CASH/releases) or [build from source](https://github.com/X-CASH-official/X-CASH#compiling-x-cash-from-source)       |
 
 
## Recommendations For the XCASH Wallet
It is recommended if you are going to run a XCASH DPOPS node, to not keep all of your XCASH on the server. The recommended way is to create an empty wallet and leave that on the server, to collect the block rewards, and then use your main wallet to vote for the new wallet that you created on the server.



## Installation Process

### Installation Path
It is recommend to install the XCASH_DPOPS folder, MongoDB and MongoDB C Driver in the home directory (`/home/$USER/`) or root directory (`/root/`) in a `Installed-Programs` folder
 
### Install System Packages
Make sure the systems packages list is up to date  
`sudo apt update`
 
Install the packages  
`sudo apt install build-essential cmake pkg-config libssl-dev git`
 
(Optionally) Install the packages for XCASH if you plan to [build XCASH from source](https://github.com/X-CASH-official/X-CASH#compiling-x-cash-from-source)
 
 
 
### Installing MongoDB From Binaries
 
Visit [https://www.mongodb.com/download-center/community](https://www.mongodb.com/download-center/community)
 
Then choose your OS, and make sure the version is the current version and the package is server. Then click on All version binaries. Now find the current version to download. You do not want the debug symbols or the rc version, just the regular current version.
 
Once you have downloaded the file move the file to a location where you want to keep the binaries, then run this set of commands 

**If you want to install MongoDB on a different hard drive then the hard drive your OS is installed on, make sure to change the path of the `/data/db`**
``` 
tar -xf mongodb-linux-x86_64-*.tgz
rm mongodb-linux-x86_64-*.tgz
sudo mkdir -p /data/db
sudo chmod 770 /data/db
sudo chown $USER /data/db
```
 
 
 
### Building the MongoDB C Driver From Source
 
Visit the offical websites installation instructions at [http://mongoc.org/libmongoc/current/installing.html](http://mongoc.org/libmongoc/current/installing.html)
You will need to follow the instructions for [Building from a release tarball](http://mongoc.org/libmongoc/current/installing.html#building-from-a-release-tarball) or [Building from git](http://mongoc.org/libmongoc/current/installing.html#building-from-git) since you need the header files, not just the library files.
 
After you have built the MongoDB C driver from source, you will need to run  
`sudo ldconfig`
 
 
 
### Cloning the Repository
```
cd ~Installed-Programs 
git clone https://github.com/X-CASH-official/XCASH_DPOPS.git
```
 
 
 
### Build Instructions
 
X-CASH Proof of stake uses a Make file.
 
After cloning the repository, navigate to the folder  
`cd ~/Installed-Programs/XCASH_proof_of_stake`
 
Then use the make file to build the binary file  
`make clean ; make release`
 
 
## How To Setup the Firewall
 
We will need to setup a firewall for our DPOPS node. The goal of settings up the firewall is to block any DDOS attacks. We will use IPtables for the firewall
 
The firewall is configured for a solo node setup. To configure the firewall for a shared delegates website or delegates website:
 
Open the firewall script  
`nano ~/Installed-Programs/XCASH_DPOPS/scripts/firewall/firewall_script.sh`
 
Uncomment these 3 lines (by removing the `#`) if running a shared delegates website or delegates website  
`# iptables -t filter -I INPUT -p tcp --syn --dport 80 -m connlimit --connlimit-above 100 --connlimit-mask 32 -j DROP`
 
`# iptables -A INPUT -p tcp --dport 80 -j ACCEPT`
 
`# iptables -A PREROUTING -t nat -p tcp --dport 80 -j REDIRECT --to-ports 18283`
 
If you want to run the shared delegates website or delegates website using HTTPS, you will need to install a webserver like nginx and configure it.
 
Now we need to run the firewall script and activate it  
```
chmod +x ~/Installed-Programs/XCASH_DPOPS/scripts/firewall/firewall_script.sh
~/Installed-Programs/XCASH_DPOPS/scripts/firewall/firewall_script.sh
iptables-save > /etc/network/iptables.up.rules
iptables-apply -t 60
```
 
You should then open another connection to the server to make sure it worked and did not lock you out. Then press y to confirm the changes for the firewall.
 
 
 
 
## How To Setup and Install the Systemd Files

Edit the below systemd files to your paths

Copy all of the service files in the systemd folder to `/lib/systemd/system/`  
`cp -a ~/Installed-Programs/XCASH_DPOPS/scripts/systemd/* /lib/systemd/system/`

Reload systemd  
`systemctl daemon-reload`

Create a systemd PID folder  
`mkdir ~/Installed-Programs/systemdpid/`

Create a mongod pid file and a xcashd pid file
```
touch ~/Installed-Programs/systemdpid/mongod.pid
touch ~/Installed-Programs/systemdpid/xcash_daemon.id
```


### MongoDB
This is the systemd file for MongoDB
```
[Unit]
Description=MongoDB Database Server
After=network.target
 
[Service]
Type=forking
User=root
PIDFile=/root/Installed-Programs/systemdpid/mongod.pid
ExecStart=/root/Installed-Programs/mongodb-linux-x86_64-ubuntu1804-4.2.0/bin/mongod
Restart=always
 
LimitFSIZE=infinity
LimitCPU=infinity
LimitAS=infinity
LimitNOFILE=64000
LimitNPROC=64000
LimitMEMLOCK=infinity
TasksMax=infinity
TasksAccounting=false
 
[Install]
WantedBy=multi-user.target
```

You will need to change the **User** to the user of the system

You will need to change the **PIDFile** to the full path of the `mongod.pid` file

You will need to change the **ExecStart** to the full path of the `mongod` file

Reload systemd after you have made any changes to the systemd service files  
`systemctl daemon-reload`


### XCASH Daemon
This is the systemd file for XCASH Daemon
```
[Unit]
Description=XCASH Daemon systemd file
 
[Service]
Type=forking
User=root
PIDFile=/root/Installed-Programs/systemdpid/xcash_daemon.pid
ExecStart=/root/Installed-Programs/X-CASH/build/release/bin/xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind --detach --pidfile /root/Installed-Programs/systemdpid/xcash_daemon.pid
RuntimeMaxSec=15d
Restart=always
 
[Install]
WantedBy=multi-user.target
```

Make sure to leave the RuntimeMaxSec in the systemd service file, as the XCASH Daemon usually needs to restart after a while to prevent it from not synchronizing

You will need to change the **User** to the user of the system

You will need to change the **PIDFile** to the full path of the `xcash_daemon.pid` file

You will need to change the **ExecStart** to the full path of the `xcashd` file

Reload systemd after you have made any changes to the systemd service files  
`systemctl daemon-reload`



### XCASH Daemon Block Verifier
This is the systemd file for XCASH Daemon when you are a block verifier  
**Only run the XCASH Daemon service file, as the XCASH DPOPS program will determine if your a block verifier and start the correct systemd service file**
```
[Unit]
Description=XCASH Daemon Block Verifier systemd file
 
[Service]
Type=forking
User=root
PIDFile=/root/Installed-Programs/systemdpid/xcash_daemon.pid
ExecStart=/root/Installed-Programs/X-CASH/build/release/bin/xcashd --block_verifier --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind --detach --pidfile /root/Installed-Programs/systemdpid/xcash_daemon.pid
RuntimeMaxSec=15d
Restart=always
 
[Install]
WantedBy=multi-user.target
```

Make sure to leave the RuntimeMaxSec in the systemd service file, as the XCASH Daemon usually needs to restart after a while to prevent it from not synchronizing

You will need to change the **User** to the user of the system

You will need to change the **PIDFile** to the full path of the `xcash_daemon.pid` file

You will need to change the **ExecStart** to the full path of the `xcashd` file

Reload systemd after you have made any changes to the systemd service files  
`systemctl daemon-reload`



### XCASH Wallet
This is the systemd file for XCASH Wallet
```
[Unit]
Description=XCASH Wallet
 
[Service]
Type=simple
User=root
ExecStart=/root/X-CASH/build/release/bin/xcash-wallet-rpc --wallet-file WALLET_FILE_NAME --password PASSWORD --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon
Restart=always
 
[Install]
WantedBy=multi-user.target

```

You will need to change the **User** to the user of the system

You will need to change the **WALLET_FILE_NAME** with the name of your wallet file, and the **PASSWORD** with the password of your wallet file.

Reload systemd after you have made any changes to the systemd service files  
`systemctl daemon-reload`


### XCASH DPOPS
This is the systemd file for XCASH DPOPS
```
[Unit]
Description=XCASH DPOPS
 
[Service]
Type=simple
LimitNOFILE=64000
User=root
WorkingDirectory=/root/Installed-Programs/XCASH_DPOPS/
ExecStart=/root/Installed-Programs/XCASH_DPOPS/XCASH_DPOPS
Restart=always
 
[Install]
WantedBy=multi-user.target
```

The LimitNOFILE will allow the XCASH DPOPS program to utilize up to 64000 concurrent file descriptors instead of the default 4096 for a linux process. The actual XCASH DPOPS program is limited to only accept up to 1000 concurrent connections due to that DPOPS usage and shared delegates website or delegates website usage will not be that much at the start. This will help with DDOS at this time and the limits in the XCASH DPOPS program will be updated.

You will need to change the **User** to the user of the system

You will need to change the **ExecStart** to the full path of the `XCASH_DPOPS` file and add any startup flags if running a shared delegates website or a delegates website

To run the XCASH DPOPS node and the shared delegates website, add the flag `--shared_delegates_website`

To run the XCASH DPOPS node and the delegates website, add the flag `--delegates_website`

Reload systemd after you have made any changes to the systemd service files  
`systemctl daemon-reload`
 
 
 
## How To Run Each Component
To start a systemd service  
`systemctl start name_of_service_file_without.service`

To stop a systemd service  
`systemctl stop name_of_service_file_without.service`

To restart a systemd service  
`systemctl restart name_of_service_file_without.service`

To check the status of a systemd service  
`systemctl status name_of_service_file_without.service`

For example

To start XCASH DPOPS service  
`systemctl start XCASH_DPOPS`

To stop XCASH DPOPS service  
`systemctl stop XCASH_DPOPS`

To restart XCASH DPOPS service  
`systemctl restart XCASH_DPOPS`

To check the status of XCASH DPOPS service  
`systemctl status XCASH_DPOPS`



## How To View Logs For Each Component
To view the logs for any service file. you can run  
`journalctl --unit=name_of_service_file_without.service`

To view only the last 100 lines of the log file, you can run  
`journalctl --unit=name_of_service_file_without.service -n 100 --output cat`

To view live logging of XCASH DPOPS, you can run  
`journalctl --unit=XCASH_DPOPS --follow -n 100 --output cat`
 
 
 
## Running X-CASH Proof of stake test
It is recomeneded to run the X-CASH Proof of stake test before you run the main program. The test will ensure that your system is compatbile, and that you have setup your system correctly.
 
To run the X-CASH Proof of stake test, make sure to have already started the XCASH Daemon, XCASH Wallet and MongoDB systemd services, and to have stopped the XCASH DPOPS systemd service if it was already running.  

Navigate to the folder that contains the binary, then run  
`./XCASH_DPOPS --test`
 
The test will return the number of passed and failed test on the bottom of the console. The failed test need to be 0 before you run the node. If the output is not showing 0 for failed test, then you need to scroll through the testing output and find what test failed (It will be red instead of green). If this is a system compatibility test, then you will need to fix the system. If this is a core test that has failed, then you need to possibly rebuild, or contact us with your OS version, and we can look into it.



## How to Register a Delegate
Make sure to stop the XCASH Wallet service if it is running  
`systemctl stop XCASH_Wallet`

Open the wallet file in the `xcash-wallet-cli`

Once the wallet is fully synchronized run the following:  
`delegate_register delegate_name delegate_IP_address`

Replace delegate_name with the name that you want to name your delegate

Replace delegate_IP_address with your VPS/dedicated servers IP Address or a domain name (View the setup below)



## How to Vote For a Delegate
Make sure to stop the XCASH Wallet service if it is running  
`systemctl stop XCASH_Wallet`

Open the wallet file in the `xcash-wallet-cli`

Once the wallet is fully synchronized run the following:  
`vote delegates_public_address`

Replace delegates_public_address with the delegates public address




## How to Update a Delegates Information
Make sure to stop the XCASH Wallet service if it is running  
`systemctl stop XCASH_Wallet`

Open the wallet file in the `xcash-wallet-cli`

Once the wallet is fully synchronized run the following:  
`delegate_update item value`

Replace item with the item you want to update. The list of valid items are:  
```
IP_address
about
website
team
pool_mode
fee_structure
server_settings
```
Replace value with the updated information



## How to Remove a Delegate
Make sure to stop the XCASH Wallet service if it is running  
`systemctl stop XCASH_Wallet`

Open the wallet file in the `xcash-wallet-cli`

Once the wallet is fully synchronized run the following:  
`delegate_remove`