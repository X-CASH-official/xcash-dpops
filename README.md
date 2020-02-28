# X-CASH Delegated Proof Of Privacy Stake (DPOPS)

⚠ Alpha: The DPOPS integration is still a work in progress 🏗 
 
X-CASH DPOPS is a variation of DPOS and DBFT that uses verifiable random functions for the selection of the next block producer. The key features of X-CASH DPOPS are:
 
* The top 100 delegates are elected as block verifiers.
* Reserve proof based voting/staking system, meaning the XCASH always stays in your wallet.
* No need to keep your wallet or computer online if your just staking towards a shared delegate.
* No need to keep your XCASH directly on the server if running a solo node, as you can use an empty wallet on the server and vote for yourself.
* A minimum of 2 million XCASH is needed to vote for a delegate.
* A minimum of 20 million XCASH is needed to register as a delegate and not be auto removed for inactivity at UTC 00:00.
 
* The election process is every block, meaning a new vote will get counted for the next block.
* No need to cancel a vote, as it will automatically get cancelled if you change your vote to a different delegate.
* No lockup times, the xcash always remains in your wallet and you can use them at any time, although moving them from your wallet will cancel your entire staking vote.
* No fees for voting, and you can revote or switch your vote as many times as you like.
 
* Using a variation of Delegated Byzantine Fault tolerance consensus where 67% consensus must be reached for a new block to be added to the network.
* DBFT allows for up to 33% of the elected block verifiers to stop working, and the system will still be able to produce a new block.
* Using Verifiable Random Functions to select the next block producer in the system. This allows for a random, but verifiable way of selecting the next block producer.
* Blocks can be verified in the XCASH Daemon with a detailed explanation of the reserve bytes in the block.
 
* Using a decentralized database system, to hold all of the voting data and reserve bytes data.
* The block format is to only store a hash of the contents of the reserve bytes data in the block, and store the actual reserve bytes data in the decentralized database, to reduce the extra size of the blockchain, while keeping all of the rounds data.
 
This program allows one to run a DPOPS node, a shared delegates website, and a delegates website.

**If you plan on running a [shared delegates website](https://github.com/X-CASH-official/XCASH_DPOPS_shared_delegates_website) or a [delegates website](https://github.com/X-CASH-official/XCASH_DPOPS_delegates_website), you will need to run the website on the same system as the DPOPS node**

**By running a DPOPS node (solo or shared) you will need the computer to be online and running at all times**


## Table of Contents  
[System Requirements](#system-requirements)  
[Synchronizing the Time](#synchronizing-the-time)  
[Dependencies](#dependencies)  
[Recommendations For the XCASH Wallet](#recommendations-for-the-xcash-wallet)  
[XCASH_DPOPS Parameters](#xcash_dpops-parameters)  
[Installation Options](#installation-options)  
[Installation Process](#installation-process)  
* [Installation Path](#installation-path)  
* [Install System Packages](#install-system-packages) 
* [Installing MongoDB From Binaries](#installing-mongodb-from-binaries)  
* [Building the MongoDB C Driver From Source](#building-the-mongodb-c-driver-from-source)  
* [Cloning the Repository](#cloning-the-repository)  
* [Build Instructions](#build-instructions)  

[How To Setup and Install the Systemd Files](#how-to-setup-and-install-the-systemd-files)  
* [MongoDB](#mongodb)  
* [XCASH Daemon](#xcash-daemon)  
* [XCASH Wallet](#xcash-wallet)
* [XCASH DPOPS](#xcash-dpops)
* [Firewall](#firewall)  

[How To Setup the Firewall](#how-to-setup-the-firewall)  
[How To Run Each Component](#how-to-run-each-component)  
[How To View Logs For Each Component](#how-to-view-logs-for-each-component)  
[Running X-CASH Proof of stake test](#running-x-cash-proof-of-stake-test)  
[How to Use the XCASH DPOPS Wallet Commands](#how-to-use-the-xcash-dpops-wallet-commands)  
*  [How to Register a Delegate](#how-to-register-a-delegate)  
*  [How to Vote For a Delegate](#how-to-vote-for-a-delegate)  
*  [How to Update a Delegates Information](#how-to-update-a-delegates-information)  
*  [How to Remove a Delegate](#how-to-remove-a-delegate)  

### Appendix
[How to Setup a Domain Name Instead of an IP Address](#how-to-setup-a-domain-name-instead-of-an-ip-address)  
[Auto Installation Process](#auto-installation-process)  
*  [Install](#install)  
*  [Update](#update)  
*  [Remove](#remove)
  
[How to Setup an LXC container](#how-to-setup-an-lxc-container)
*  [Install LXC](#install-lxc)    
*  [Create LXC Container](#create-lxc-container)  
*  [Configure the Container](#configure-the-container)  
*  [Install XCASH DPOPS in the Container](#install-xcash_dpops-in-the-container) 
 
[How to Setup A Private Test](#how-to-setup-a-private-test)  
[How to Setup A Private Test Using LXC Containers](#how-to-setup-a-private-test-using-lxc-containers)  
[How to Debug the Code on a Server](#how-to-debug-the-code-on-a-server)
*  [Allow X11 Forwarding on the server](#allow-x11-forwarding-on-the-server)  
*  [Install GDB and vscode](#install-gdb-and-vscode)
*  [Run vscode on the Server and Forward the GUI to a Dekstop](#run-vscode-on-the-server-and-forward-the-gui-to-a-dekstop)
*  [Change vscode Keyboard Shortcuts](#change-vscode-keyboard-shortcuts)

 
## System Requirements
 
XCASH DPOPS will only run on a Linux/Unix OS at this time. We recommend installing this on a Ubuntu VPS/dedicated server (18.04) for the best compatibility.
 
**Minimum System Requirements:**  
Operating System: Ubuntu 18.04 (or higher)  
CPU: 4 threads, with at least 2.0GHZ or more for each thread  
RAM: 8GB  
Hard drive: 50GB (with 18 GB of space needed per year)  
Bandwidth Transfer: 500GB per month  
Bandwidth Speed: 50 Mbps
 
**Recommended System Requirements:**  
Operating System: Ubuntu 18.04 (or higher)  
CPU: 8 threads, with at least 2.0GHZ or more for each thread  
RAM: 16GB  
Hard drive: 100GB (with 18 GB of space needed per year)  
Bandwidth Transfer: 2TB per month  
Bandwidth Speed: 500 Mbps


## Synchronizing the Time
XCASH_DPOPS uses the system time to be able to know what part of the round it is and what data to send or receive. It is important that your system time is accurate otherwise the program will most likely not work. All XCASH_DPOPS delegates must be synced within 2 seconds of each other. A check is included when you start the program to make sure that your system time is within two seconds of the main network data node, but it is important to check your server manually to see if your time is synced.

Note you can use any timezone you want, the timezones do not affect the synchronization.

To check if your system time is synced properly run  
`timedatectl`

Make sure that `System clock synchronized` is `yes`

If it is `no` then run  
```
timedatectl set-ntp true
systemctl restart systemd-timesyncd
```

Then run  
`timedatectl`

Make sure that `System clock synchronized` is `yes`

 
 
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



## XCASH_DPOPS Parameters
```
Parameters
--parameters - Show a list of all valid parameters
All parameters are optional, execept for --block_verifiers_secret_key. **The --block_verifiers_secret_key must be the first parameter**

--test - Run the test to make sure the program is compatible with your system. This can take around 24 hours. Pass this parameter last.

--quick_test - Run the quick test to make sure the program is compatible with your system. This can take around 10 minutes. Pass this parameter last.

--optimizations_test - Run the optimizations test only. This will check if your computer is fast enough for XCASH_DPOPS. This can take around 2 minutes. Pass this parameter last.

--block_verifiers_secret_key - The block verifiers secret key. This must be the first parameter

--debug - Show all incoming and outgoing messages from the server

--log_file "log_file" - Write all output to a log file with no colors

--log_file_color "log_file" - Write all output to a log file with colors

--total_threads "total_threads" - The total threads to use, the maximum is 100 total threads.
If this parameter is not specified, the default is the number of threads the CPU has.

--XCASH_DPOPS_delegates_IP_address "delegates IP address" - The delegates IP address (IPV4 only) to create the server on.
If this parameter is not specified, the default is 0.0.0.0 (all available networking interfaces).

--xcash_wallet_port "xcash_wallet_port" - The xcash wallet port.
If this parameter is not specified, the default port is 18285.

--database_name - The name of the database.
If this parameter is not specified, the default name is XCASH_PROOF_OF_STAKE.

--shared_delegates_database_name - The name of the shared delegates database.
If this parameter is not specified, the default name is XCASH_PROOF_OF_STAKE_DELEGATES.

--delegates_website - Run the delegates website

--shared_delegates_website --fee "fee" --minimum_amount "minimum_amount" - Run the shared delegates website, with a fee of "fee" and a minimum amount of "minimum_amount"
The fee in a percentage (1 would equal 1 percent. You can use up to 6 decimal places.)
The minimum for a public_address to receive a payment (10000 etc. The minimum amount should be in regular units, not atomic units.)
The voter inactivity count is optional. This is the number of consecutive days where a voter would have registered in the database, but is not actively participating towards the shared delegate. If this number of consecutive days is reached, the voter will be removed from the database and all funds that were left over would not sent out, since they were below the MINIMUM AMOUNT. If this parameter is not specified, the default amount is 30 consecutive days.
The shared delegate payment time hour is optional. This is the hour that the shared delegate will send the daily payments. If this parameter is not specified, the default amount is a random hour
The shared delegate payment time minute is optional. This is the minute that the shared delegate will send the daily payments. If this parameter is not specified, the default amount is a random minute.

--generate_key - Generate a public and private key for signing and verifying messages between block verifiers

--synchronize_database_from_network_data_node - Synchronize the database from a network data node.

--synchronize_database_from_specific_delegate DELEGATES_IP_ADDRESS - Synchronize the database from a specific node, and does not check the majority. It will also exit the program once done syncing.
Note: This method of syncing is likely to be the most error prone, since it skips the majority checks, but will also likely be the fastest.
It should only be used in specific circumstances.

--disable_synchronizing_databases_and_starting_timers - Disables synchronizing the databases and starting the timers. Used for testing.

--registration_mode - Runs the registration mode only

--start_time "day" "hour" "minute" - Starts the current block height timer at a specific time. The day is the day of the month (1-31), the hour is the hour in UTC (0-23) the minute is the minute (0-59)

--test_data_add - Add test data to the databases

--test_data_remove - Remove test data from the databases
```


## Installation Options
XCASH_DPOPS provides a few different installation options. It is recommended to at least read over how to manually install XCASH_DPOPS, and then choose one of the installation options below:

[Manual Installation Process](#installation-process)  
[Automatic Installation Process](#auto-installation-process)  
[Docker Automatic Installation Process](#docker-automatic-installation-process)


## Installation Process

### Installation Path
It is recommend to install the XCASH_DPOPS folder, MongoDB and MongoDB C Driver in the home directory (`/home/$USER/`) or root directory (`/root/`) in a `x-network` folder

Create the `x-network` folder  
`mkdir ~/x-network/`

Create a `xcash_wallets` folder  
`mkdir ~/x-network/xcash_wallets`

Create a `logs` folder  
`mkdir ~/x-network/logs`



 
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

Then add MongoDB to your path  
`echo -e '\nexport PATH=MongoDB_folder:$PATH' >> ~/.profile && source ~/.profile
 
 
 
### Building the MongoDB C Driver From Source
 
Visit the official websites installation instructions at [http://mongoc.org/libmongoc/current/installing.html](http://mongoc.org/libmongoc/current/installing.html)
You will need to follow the instructions for [Building from a release tarball](http://mongoc.org/libmongoc/current/installing.html#building-from-a-release-tarball) or [Building from git](http://mongoc.org/libmongoc/current/installing.html#building-from-git) since you need the header files, not just the library files.
 
After you have built the MongoDB C driver from source, you will need to run  
`sudo ldconfig`
 
 
 
### Cloning the Repository
```
cd ~/x-network 
git clone https://github.com/X-CASH-official/XCASH_DPOPS.git
``` 
 
 
### Build Instructions
 
XCASH_DPOPS uses a Make file.
 
After cloning the repository, navigate to the folder  
`cd ~/x-network/XCASH_DPOPS`
 
Then use the make file to build the binary file  
`make clean ; make release -j $(nproc)`
 
 
 
## How To Setup and Install the Systemd Files

Edit the below systemd files to your paths

Copy all of the service files in the systemd folder to `/lib/systemd/system/`  
`cp -a ~/x-network/XCASH_DPOPS/scripts/systemd/* /lib/systemd/system/`

Reload systemd  
`systemctl daemon-reload`

Create a systemd PID folder  
`mkdir ~/x-network/systemdpid/`

Create a logs folder  
`mkdir ~/x-network/logs/`

Create a mongod pid file and a xcashd pid file
```
touch ~/x-network/systemdpid/mongod.pid
touch ~/x-network/systemdpid/xcash_daemon.pid
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
Type=oneshot
RemainAfterExit=yes
PIDFile=/root/x-network/systemdpid/mongod.pid
ExecStart=/root/x-network/mongodb-linux-x86_64-ubuntu1804-4.2.0/bin/mongod --fork --syslog

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
PIDFile=/root/x-network/systemdpid/xcash_daemon.pid
ExecStart=/root/x-network/X-CASH/build/release/bin/xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind --log-file /root/x-network/logs/XCASH_Daemon_log.txt --max-log-file-size 0 --detach --pidfile /root/x-network/systemdpid/xcash_daemon.pid
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
ExecStart=/root/x-network/X-CASH/build/release/bin/xcash-wallet-rpc --wallet-file /root/x-network/xcash_wallets/WALLET_FILE_NAME --password PASSWORD --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon
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
WorkingDirectory=/root/x-network/XCASH_DPOPS/build/
ExecStart=/root/x-network/XCASH_DPOPS/build/XCASH_DPOPS --block_verifiers_secret_key BLOCK_VERIFIERS_SECRET_KEY
Restart=always
 
[Install]
WantedBy=multi-user.target
```

The LimitNOFILE will allow the XCASH DPOPS program to utilize up to 64000 concurrent file descriptors instead of the default 4096 for a linux process. The actual XCASH DPOPS program is limited to only accept up to 1000 concurrent connections due to that DPOPS usage and shared delegates website or delegates website usage will not be that much at the start. This will help with DDOS at this time and the limits in the XCASH DPOPS program will be updated.

You will need to change the **User** to the user of the system

You will need to change the **ExecStart** to the full path of the `XCASH_DPOPS` file and add any startup flags if running a shared delegates website or a delegates website

You will need to change the **BLOCK_VERIFIERS_SECRET_KEY** to your block verifiers secret key. Make sure this is the first parameter

For a full list of XCASH_DPOPS parameters please read the [XCASH_DPOPS Parameters](#xcash_dpops-parameters) section. This section will explain how to change any of the provided parameters in detail. Since the shared delegate option is probably the most used parameter, this parameter will be explained below.

```
--shared_delegates_website --fee "fee" --minimum_amount "minimum_amount" - Run the shared delegates website, with a fee of "fee" and a minimum amount of "minimum_amount"
The fee in a percentage (1 would equal 1 percent. You can use up to 6 decimal places.)
The minimum for a public_address to receive a payment (10000 etc. The minimum amount should be in regular units, not atomic units.)
```

Reload systemd after you have made any changes to the systemd service files  
`systemctl daemon-reload`



### Firewall
This is the systemd file for firewall
```
[Unit]
Description=firewall
 
[Service]
Type=oneshot
RemainAfterExit=yes
User=root
ExecStart=/root/x-network/XCASH_DPOPS/scripts/firewall/firewall_script.sh
 
[Install]
WantedBy=multi-user.target
```

You will need to change the **User** to the user of the system

You will need to change the **ExecStart** to the full path of the `firewall_script.sh` file

Reload systemd after you have made any changes to the systemd service files  
`systemctl daemon-reload`



## How To Setup the Firewall
 
We will need to setup a firewall for our DPOPS node. The goal of settings up the firewall is to block any DDOS attacks. We will use IPtables for the firewall
 
The firewall is configured for a solo node setup. To configure the firewall for a shared delegates website or delegates website:
 
Open the firewall script  
`nano ~/x-network/XCASH_DPOPS/scripts/firewall/firewall_script.sh`
 
Uncomment these 3 lines (by removing the `#`) if running a shared delegates website or delegates website  
`# iptables -t filter -I INPUT -p tcp --syn --dport 80 -m connlimit --connlimit-above 100 --connlimit-mask 32 -j DROP`
 
`# iptables -A INPUT -p tcp --dport 80 -j ACCEPT`
 
`# iptables -A PREROUTING -t nat -p tcp --dport 80 -j REDIRECT --to-ports 18283`
 
If you want to run the shared delegates website or delegates website using HTTPS, you will need to install a webserver like nginx and configure it.
 
Now we need to run the firewall script and activate it  
```
chmod +x ~/x-network/XCASH_DPOPS/scripts/firewall/firewall_script.sh
~/x-network/XCASH_DPOPS/scripts/firewall/firewall_script.sh
iptables-save > /etc/network/iptables.up.rules
iptables-apply -t 60
```
 
You should then open another connection to the server to make sure it worked and did not lock you out. Then press y to confirm the changes for the firewall.

Now we need to enable the firewall systemd service file to run this script after a restart  
`systemctl enable firewall`
 
 
 
 
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

To view logs for the XCASH_Daemon systemd service  
`tail -f -n 100 ~/x-network/logs/XCASH_Daemon_log.txt`
 
 
 
## Running X-CASH Proof of stake test
It is recomeneded to run the X-CASH Proof of stake test before you run the main program. The test will ensure that your system is compatbile, and that you have setup your system correctly.

There are 3 different testing modes that can be run: 
--test = This will run all of the test. This will take around 24 hours.
--quick_test = This will run all of the test, execpt for the longest test. This is the recomended test to run. This will take around 1 hour.
--optimizations_test = This will only run the optimizations test. This will check if your computer is quick enough to run XCASH_DPOPS. This test will take around 10 minutes.
 
To run the X-CASH Proof of stake test, make sure to have already started the XCASH Daemon, XCASH Wallet and MongoDB systemd services, and to have stopped the XCASH DPOPS systemd service if it was already running. 

Navigate to the folder that contains the binary  
Rebuild the binary in debug mode  
`make clean ; make debug -j $(nproc)`

Then run the binary  
`./XCASH_DPOPS --block_verifiers_secret_key BLOCK_VERIFIER_SECRET_KEY --test`

Replace BLOCK_VERIFIER_SECRET_KEY with your block_verifiers_secret_key 
 
The test will return the number of passed and failed test on the bottom of the console. The failed test need to be 0 before you run the node. If the output is not showing 0 for failed test, then you need to scroll through the testing output and find what test failed (It will be red instead of green). If this is a system compatibility test, then you will need to fix the system. If this is a core test that has failed, then you need to possibly rebuild, or contact us with your OS version, and we can look into it.



## How to Use the XCASH DPOPS Wallet Commands



### How to Register a Delegate
Make sure to stop the XCASH_DPOPS service, if it is running  
`systemctl stop XCASH_DPOPS`

Generate a public and secret key pair for signing and verifying the block verifier messages, and for signing and verifying the blocks reserve bytes  
`~/x-network/XCASH_DPOPS/build/XCASH_DPOPS --generate_key`

Put the secret key in the `BLOCK_VERIFIERS_SECRET_KEY` in 
```
~/x-network/XCASH_DPOPS/src/global_data/block_verifiers_sign_and_verify_messages.h
```

Rebuild XCASH_DPOPS  
`make clean ; make debug -j $(nproc)`

Make sure that when updating the repository the `block_verifiers_sign_and_verify_messages.h` does not update  
```
git update-index --skip-worktree ~/x-network/XCASH_DPOPS/src/global_data/block_verifiers_sign_and_verify_messages.h
```

You can always undo this change by running 
```  
git update-index --no-skip-worktree ~/x-network/XCASH_DPOPS/src/global_data/block_verifiers_sign_and_verify_messages.h
```

Make sure to stop the XCASH Wallet service, if it is running  
`systemctl stop XCASH_Wallet`

Open the wallet file in the `xcash-wallet-cli`

Once the wallet is fully synchronized run the following:  
`delegate_register delegate_name delegate_IP_address block_verifier_messages_public_key`

Replace `delegate_name` with the name that you want to name your delegate

Replace `delegate_IP_address` with your VPS/dedicated servers IP Address or a domain name (View the setup below)

Replace `block_verifier_messages_public_key` with public key that you got from the `XCASH_DPOPS --generate_key`



### How to Vote For a Delegate
Make sure to stop the XCASH Wallet service if it is running  
`systemctl stop XCASH_Wallet`

Open the wallet file in the `xcash-wallet-cli`

Once the wallet is fully synchronized run the following:  
`vote delegates_public_address | delegates_name`

Replace `delegates_public_address | delegates_name` with the delegates public address or delegates name




### How to Update a Delegates Information
Make sure to stop the XCASH Wallet service if it is running  
`systemctl stop XCASH_Wallet`

Open the wallet file in the `xcash-wallet-cli`

Once the wallet is fully synchronized run the following:  
`delegate_update item value`

Replace `item` with the item you want to update. The list of valid items are:  

| Name                                         | Type          | Maximum Length or Range       |
| -------------------------------------------- | ------------- | -------------------------     |
| IP_address                                   | String        | maximum of 255 characters     |
| about                                        | String        | maximum of 1024 characters    |
| website                                      | String        | maximum of 255 characters     |
| team                                         | String        | maximum of 255 characters     |
| pool_mode                                    | Boolean       | true or false                 |
| fee_structure                                | Number        | 0-100 up to 6 decimal places  |
| server_settings                              | String        |  maximum of 255 characters    |

Replace value with the updated information



### How to Remove a Delegate
Make sure to stop the XCASH Wallet service if it is running  
`systemctl stop XCASH_Wallet`

Open the wallet file in the `xcash-wallet-cli`

Once the wallet is fully synchronized run the following:  
`delegate_remove`


# Appendix



## How to Setup a Domain Name Instead of an IP Address
The XCASH DPOPS system needs a IP Address when registering a delegate to be able to let other delegates know where to send messages to. One can instead setup a domain name (**without the www**) and register this instead of an IP address. The possible benefits of using a domain name over an IP address could be:

* One can change IP's from their domain page if they change servers instead of having to update that info in the DPOPS database.
* It would probably be more recognizable if there was a problem, since in the XCASH_DPOPS logs and the XCASH_Daemon logs it will print the source and destination of messages.

To Setup a domain instead of an IP address, go to the domain registrar you have purchased the domain name from. Add an A record to the domain. Each domain registrar is going to be a little different, so you will want to check if they have an official article on how to add an A record.

Now you need to setup the reverse DNS as well. Go to the hosting dashboard of the place where you are renting the server. Not all hosting companies let you change the reverse DNS, so you might not be able to change the reverse DNS. Navigate to the server your are renting. At this point their should be something that says modify the reverse DNS or something similar. Change it to the domain name you used in the first step.

At this point you can now register the domain name (**without the www.**) to the XCASH DPOPS system.



## Auto Installation Process
The XCASH DPOPS auto installer can be run to first setup the system, and then later in an update mode to update any needed x-network repositories or newer third party dependencies. It can also be run in a remove mode that will uninstall everything.

### Install
To Install, run the autoinstaller script, and choose Install at the menu.  
`scripts/autoinstaller/autoinstaller.sh`

Since the XCASH_DPOPS repository will probably not be downloaded yet onto your system, you can run directly from the github by running  
```
bash -c "$(curl -sSL https://raw.githubusercontent.com/X-CASH-official/XCASH_DPOPS/master/scripts/autoinstaller/autoinstaller.sh)"
```

You can then press enter for the default settings, or input custom settings. The autoinstaller will then install everything automatically for you.

The autoinstaller has the following customizable options:
```
Installation Directory
MongoDB Installation Directory
X-CASH Blockchain Installation Directory
Shared Delegate or Solo Delegate
Shared Delegate Fee
Shared Delegate Minimum Payment Amount
Create a New Wallet or Import an Existing Wallet
Generate a New Password or Use an Existing Password:
```


### Update

It is recommend to run the autoinstaller in update mode. The autoinstaller will update you automatically to the latest versions of:  
```
System Packages that are related X-CASH or XCASH_DPOPS
X-CASH
XCASH_DPOPS
XCASH_DPOPS_shared_delegates_website
MongoDB
Mongo C Driver
NodeJS
NPM
```


### Remove

The autoinstaller can also remove XCASH_DPOPS. It will remove the following items:
```
System Packages that are related X-CASH or XCASH_DPOPS
X-CASH
XCASH_DPOPS
XCASH_DPOPS_shared_delegates_website
MongoDB
Mongo C Driver
NodeJS
NPM
```



## How to Setup an LXC Container
You can also run XCASH DPOPS inside of a container, using LXC containers. This will allow you to keep the main system clean and separated from the XCASH_DPOPS system.

It is recommended to install the firewall on your system (not inside the LXC container)  
Follow the [How To Setup the Firewall](#how-to-setup-the-firewall)  part of the readme first.


### Install LXC  
We will use LXD to manage LXC containers as this is a more user friendly tool for LXC containers. LXD might already be installed on some Ubuntu servers, but it is recommend to uninstall this version of LXD and install LXD using a snap. This is because the LXD that comes installed on some servers will not be up to date and can only be updated to minor releases.

First check if LXD is already installed on your server  
`lxd --version`

If it does output a version, then uninstall it  
`sudo apt remove -y --purge lxd lxd-client`

Then install LXD  
`sudo snap install lxd`

Then run the configuration for LXD and press enter to get the default selection to the configuration questions  
`lxd init`

When it ask for the size of the new loop device, dont use the default answer, instead specify 80-90% of your disk space, this way your containers wont have an artifical low disk space limit and can use most of your disk space.


### Create LXC Container

Now create a new container  
`lxc init --profile default ubuntu:18.04 container`

This will download the Ubuntu 18.04 LXC image if it is not already downloaded.

To start the container  
`lxc start container`

To stop the container  
`lxc stop container`

To delete the container (Note the container must be stopped first)  
`lxc delete container`

To open a terminal inside the container  
`lxc exec container -- bash`

If you already have the blockchain locally installed, and need to add it to the container you can run  
`lxc file push -r /root/.X-CASH/ container/root/`

To exit the terminal  
`exit`

To list all running containers  
`lxc list`


### Configure the Container

Now you need to setup port forwarding on the container so other block verifiers can access XCASH_DPOPS inside the container. You will need to forward TCP ports 18280, 18281 and 18283 for the public IP of the server to the containers private IP address.

First get the containers private IP address (the IPV4 address)  
`lxc list`

Then forward the ports to the containers private IP address  
```
lxc config device add container container_XCASH_DPOPS_18280 proxy listen=tcp:PUBLIC_IP_OF_SERVER:18280 connect=tcp:PRIVATE_IP_OF_CONTAINER:18280 proxy_protocol=true
lxc config device add container container_XCASH_DPOPS_18281 proxy listen=tcp:PUBLIC_IP_OF_SERVER:18281 connect=tcp:PRIVATE_IP_OF_CONTAINER:18281 proxy_protocol=true
lxc config device add container container_XCASH_DPOPS_18283 proxy listen=tcp:PUBLIC_IP_OF_SERVER:18283 connect=tcp:PRIVATE_IP_OF_CONTAINER:18283 proxy_protocol=true
```

### Install XCASH_DPOPS in the Container

LXC allows for systemd to run inside the containers. Refer to the [Auto Installation Process](#auto-installation-process) to proceed with using the autoinstaller to install XCASH_DPOPS inside the LXC container.



## How to Setup a Private Test
This will allow you to run your own private test setup of the DPOPS consensus for debugging purposes.

First, you will need at a minimum 5 servers or virtual machines. You can use any amount greater than or equal to 5, but its mandatory to use 5 to properly test the DBFT part of the consensus. It is recommended to use the autoinstaller to setup these 5 machines

Once you have the 5 machines setup, you will need a fully synced copy of the blockchain on each server.

Now you need to make the following code adjustments to work with your setup and to make a testnet copy of the official blockchain, this way you can mine blocks on it with your machines.

**X-CASH**

**src/p2p/net_node.h**  
Remove all seed nodes and add 2 of your servers IP addresses to `const std::vector<std::string> m_seed_nodes_list`

**src/p2p/net_node.inl**  
Remove all seed nodes and add 2 of your servers IP addresses 

**src/cryptonote_config.h**  
Change the block height to the current block height of your blockchain
`#define HF_BLOCK_HEIGHT_PROOF_OF_STAKE 449850 // The first block of the X-CASH proof of stake`

Change these to the total amount of servers you will use and how much percentage for verifying a block you will want
```
// XCASH DPOPS
#define BLOCK_VERIFIERS_AMOUNT 100 // The amount of block verifiers in a round
#define BLOCK_VERIFIERS_VALID_AMOUNT 67 // The amount of block verifiers that need to vote true for the part of the round to be valid.
#define BLOCK_VERIFIERS_VALID_AMOUNT_PERCENTAGE 0.67 // The amount of block verifiers in a percentage that need to vote true for the part of the round to be valid.
```

Change these define macros to your setup. Change the NETWORK_DATA_NODES_AMOUNT to the amount of seed nodes in the previous step. Remove or add NETWORK_DATA_NODE_PUBLIC_ADDRESS_N depending on how many seed nodes you have, and list there public address and IP address

```
// Network data nodes
#define NETWORK_DATA_NODES_AMOUNT 5 // The amount of network data nodes
#define NETWORK_DATA_NODE_PUBLIC_ADDRESS_1 "XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12"
#define NETWORK_DATA_NODE_PUBLIC_ADDRESS_2 "XCA1dNsv9cGc5kPMrgpdTkGttM17uR2JvCpmraGschxYSEt3MK4NRmmgyc13CTYWBGDNefdem5MFsG384DuUpGKc3ShZa4R56e"
#define NETWORK_DATA_NODE_PUBLIC_ADDRESS_3 "XCA1rU5hFV98QvysF3ByeZSPkt9wPyUxkSErBZADJjsHPMKnmCxKFH2H6aLy3oFbYaGkkYGCJcLF1ERWT5uQweEu8yZodwCtHc"
#define NETWORK_DATA_NODE_PUBLIC_ADDRESS_4 "XCA1kk9q8H7JNe9aWXLYRpG2oqFQxLD7vTy8s3pPZprBVnLRQNAurnabEHsQCSAUyxC8nForSa2C39qAhtFt4f845ZSz2Xz5Mr"
#define NETWORK_DATA_NODE_PUBLIC_ADDRESS_5 "XCA1diBcGjRhBEdDkphu5oUTTvDHiSGjmZ7unCwBFgdpMiCQoF1BpMWP2E96iFWoWoD41npDcRUo51ih45We29Hd5XZsikzt71"
#define NETWORK_DATA_NODE_IP_ADDRESS_1 "delegates.xcash.foundation"
#define NETWORK_DATA_NODE_IP_ADDRESS_2 "europe1.xcash.foundation"
#define NETWORK_DATA_NODE_IP_ADDRESS_3 "europe2.xcash.foundation"
#define NETWORK_DATA_NODE_IP_ADDRESS_4 "europe3.xcash.foundation"
#define NETWORK_DATA_NODE_IP_ADDRESS_5 "asia1.xcash.foundation"
#define INITIALIZE_NETWORK_DATA_NODES_LIST \
network_data_nodes_list.network_data_nodes_public_address[0] = NETWORK_DATA_NODE_PUBLIC_ADDRESS_1; \
network_data_nodes_list.network_data_nodes_IP_address[0] = NETWORK_DATA_NODE_IP_ADDRESS_1; \
network_data_nodes_list.network_data_nodes_public_address[1] = NETWORK_DATA_NODE_PUBLIC_ADDRESS_2; \
network_data_nodes_list.network_data_nodes_IP_address[1] = NETWORK_DATA_NODE_IP_ADDRESS_2; \
network_data_nodes_list.network_data_nodes_public_address[2] = NETWORK_DATA_NODE_PUBLIC_ADDRESS_3; \
network_data_nodes_list.network_data_nodes_IP_address[2] = NETWORK_DATA_NODE_IP_ADDRESS_3; \
network_data_nodes_list.network_data_nodes_public_address[3] = NETWORK_DATA_NODE_PUBLIC_ADDRESS_4; \
network_data_nodes_list.network_data_nodes_IP_address[3] = NETWORK_DATA_NODE_IP_ADDRESS_4; \
network_data_nodes_list.network_data_nodes_public_address[4] = NETWORK_DATA_NODE_PUBLIC_ADDRESS_5; \
network_data_nodes_list.network_data_nodes_IP_address[4] = NETWORK_DATA_NODE_IP_ADDRESS_5;
```

Under `namespace config` change a few bytes in the `boost::uuids::uuid const NETWORK_ID`

**XCASH_DPOPS**

Change the block height to the current block height of your blockchain
`#define XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT 449850 // The start block height for X-CASH proof of stake`

Change these to the total amount of servers you will use and how much percentage for verifying a block you will want
```
#define BLOCK_VERIFIERS_TOTAL_AMOUNT 100 // The total amount of block verifiers
#define BLOCK_VERIFIERS_AMOUNT 100 // The amount of block verifiers in a round
#define BLOCK_VERIFIERS_VALID_AMOUNT 67 // The amount of block verifiers that need to vote true for the part of the round to be valid
```

Change these define macros to your setup. Change the NETWORK_DATA_NODES_AMOUNT to the amount of seed nodes in the previous step. Remove or add NETWORK_DATA_NODE_PUBLIC_ADDRESS_N depending on how many seed nodes you have, and list there public address and IP address

```
// Network data nodes
#define NETWORK_DATA_NODES_AMOUNT 5 // The amount of network data nodes
#define NETWORK_DATA_NODE_1_PUBLIC_ADDRESS "XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12"
#define NETWORK_DATA_NODE_1_IP_ADDRESS "delegates.xcash.foundation"
#define NETWORK_DATA_NODE_2_PUBLIC_ADDRESS "XCA1dNsv9cGc5kPMrgpdTkGttM17uR2JvCpmraGschxYSEt3MK4NRmmgyc13CTYWBGDNefdem5MFsG384DuUpGKc3ShZa4R56e"
#define NETWORK_DATA_NODE_2_IP_ADDRESS "europe1.xcash.foundation"
#define NETWORK_DATA_NODE_3_PUBLIC_ADDRESS "XCA1rU5hFV98QvysF3ByeZSPkt9wPyUxkSErBZADJjsHPMKnmCxKFH2H6aLy3oFbYaGkkYGCJcLF1ERWT5uQweEu8yZodwCtHc"
#define NETWORK_DATA_NODE_3_IP_ADDRESS "europe2.xcash.foundation"
#define NETWORK_DATA_NODE_4_PUBLIC_ADDRESS "XCA1kk9q8H7JNe9aWXLYRpG2oqFQxLD7vTy8s3pPZprBVnLRQNAurnabEHsQCSAUyxC8nForSa2C39qAhtFt4f845ZSz2Xz5Mr"
#define NETWORK_DATA_NODE_4_IP_ADDRESS "europe3.xcash.foundation"
#define NETWORK_DATA_NODE_5_PUBLIC_ADDRESS "XCA1diBcGjRhBEdDkphu5oUTTvDHiSGjmZ7unCwBFgdpMiCQoF1BpMWP2E96iFWoWoD41npDcRUo51ih45We29Hd5XZsikzt71"
#define NETWORK_DATA_NODE_5_IP_ADDRESS "asia1.xcash.foundation"

#define DATABASE_COLLECTION_DELEGATES_DATA_1 "{\"public_address\":\"XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12\",\"total_vote_count\":\"0\",\"IP_address\":\"delegates.xcash.foundation\",\"delegate_name\":\"delegates_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"delegates.xcash.foundation\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System = Ubuntu 18.04 CPU = 32 threads (Intel 2xE5 - 2660 - 2.2GHz RAM = 256GB DDR3 Hard drive = 2x240 GB SSD + 4x2TB SATA Bandwidth Transfer = Unlimited Bandwidth Speed = 500 Mbps upload and 500 Mbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_DELEGATES_DATA_2 "{\"public_address\":\"XCA1dNsv9cGc5kPMrgpdTkGttM17uR2JvCpmraGschxYSEt3MK4NRmmgyc13CTYWBGDNefdem5MFsG384DuUpGKc3ShZa4R56e\",\"total_vote_count\":\"0\",\"IP_address\":\"europe1.xcash.foundation\",\"delegate_name\":\"europe1_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System = Ubuntu 18.04 CPU = 8 threads (Intel D-1521 - 2.40GHz RAM = 32GB DDR3 Hard drive = 2x HDD SATA 2 TB\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_DELEGATES_DATA_3 "{\"public_address\":\"XCA1rU5hFV98QvysF3ByeZSPkt9wPyUxkSErBZADJjsHPMKnmCxKFH2H6aLy3oFbYaGkkYGCJcLF1ERWT5uQweEu8yZodwCtHc\",\"total_vote_count\":\"0\",\"IP_address\":\"europe2.xcash.foundation\",\"delegate_name\":\"europe2_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System = Ubuntu 18.04 CPU = 8 threads (Intel i7-6700 - 3.40GHz RAM = 64GB DDR4 Hard drive = 2x NVMe SSD 512GB Bandwidth Transfer = Unlimited Bandwidth Speed = 1 Gbps upload and 1 Gbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_DELEGATES_DATA_4 "{\"public_address\":\"XCA1kk9q8H7JNe9aWXLYRpG2oqFQxLD7vTy8s3pPZprBVnLRQNAurnabEHsQCSAUyxC8nForSa2C39qAhtFt4f845ZSz2Xz5Mr\",\"total_vote_count\":\"0\",\"IP_address\":\"europe3.xcash.foundation\",\"delegate_name\":\"europe3_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System = Ubuntu 18.04 CPU = 8 threads (Intel i7-6700 - 3.40GHz RAM = 64GB DDR4 Hard drive = 2x NVMe SSD 512GB Bandwidth Transfer = Unlimited Bandwidth Speed = 1 Gbps upload and 1 Gbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_DELEGATES_DATA_5 "{\"public_address\":\"XCA1diBcGjRhBEdDkphu5oUTTvDHiSGjmZ7unCwBFgdpMiCQoF1BpMWP2E96iFWoWoD41npDcRUo51ih45We29Hd5XZsikzt71\",\"total_vote_count\":\"0\",\"IP_address\":\"asia1.xcash.foundation\",\"delegate_name\":\"asia1_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System = Ubuntu 18.04 CPU = 8 threads (Intel E3-1245 v5 - 3.50GHz RAM = 32GB DDR3 Hard drive = 2x HDD SATA 2 TB\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_DELEGATES_DATA_6 "{\"public_address\":\"XCA1kkdrRQ9SC7wfJ3F329giei31r13wKEKiD1ZMYf6nBiU8KE6a6LDJTZobfJ58o8A3vtGrAkzfHDd815kac7F17iPhK9zYGG\",\"total_vote_count\":\"0\",\"IP_address\":\"officialdelegate.xcash.foundation\",\"delegate_name\":\"officialdelegate_xcash_foundation\",\"about\":\"Official X-Network Shared Delegate\",\"website\":\"officialdelegate.xcash.foundation\",\"team\":\"X-Network Team\",\"pool_mode\":\"true\",\"fee_structure\":\"1\",\"server_settings\":\"Operating System = Ubuntu 18.04 CPU = 8 threads (Intel E3-1246 v3 - 3.50GHz RAM = 32GB DDR3 Hard drive = 2x HDD SATA 2 TB Bandwidth Transfer = Unlimited Bandwidth Speed = 1 Gbps upload and 1 Gbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_STATISTICS_DATA "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"delegates_xcash_foundation\",\"most_total_rounds\":\"0\",\"best_block_verifier_online_percentage_delegate_name\":\"delegates_xcash_foundation\",\"best_block_verifier_online_percentage\":\"0\",\"most_block_producer_total_rounds_delegate_name\":\"delegates_xcash_foundation\",\"most_block_producer_total_rounds\":\"0\"}"

#define INITIALIZE_DATABASE_DATA \
color_print("Initializing database data","yellow"); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1,0); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_2,0); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_3,0); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_4,0); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_5,0); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_6,0); \
insert_document_into_collection_json(DATABASE_NAME,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0); \
sleep(10);

#define INITIALIZE_NETWORK_DATA_NODES \
memcpy(network_data_nodes_list.network_data_nodes_public_address[0],NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH); \
memcpy(network_data_nodes_list.network_data_nodes_IP_address[0],NETWORK_DATA_NODE_1_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_1_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \
memcpy(network_data_nodes_list.network_data_nodes_public_address[1],NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH); \
memcpy(network_data_nodes_list.network_data_nodes_IP_address[1],NETWORK_DATA_NODE_2_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_2_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \
memcpy(network_data_nodes_list.network_data_nodes_public_address[2],NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH); \
memcpy(network_data_nodes_list.network_data_nodes_IP_address[2],NETWORK_DATA_NODE_3_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_3_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \
memcpy(network_data_nodes_list.network_data_nodes_public_address[3],NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH); \
memcpy(network_data_nodes_list.network_data_nodes_IP_address[3],NETWORK_DATA_NODE_4_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_4_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \
memcpy(network_data_nodes_list.network_data_nodes_public_address[4],NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH); \
memcpy(network_data_nodes_list.network_data_nodes_IP_address[4],NETWORK_DATA_NODE_5_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_5_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \

#define CHECK_IF_BLOCK_VERIFIERS_IS_NETWORK_DATA_NODE \
if (memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0) \
{ \
  network_data_node_settings = 1; \
}
```

Now you can disable the reserve proofs checker since there is no penalties in a test setup and this will also allow you to not have to register, or vote for the delegates. Comment out the following lines of code

**src/XCASH_DPOPS.c**

```
  // start the check_reserve_proofs_timer_thread
  if (pthread_create(&thread_id[1], NULL, &check_reserve_proofs_timer_thread, NULL) != 0 && pthread_detach(thread_id[1]) != 0)
  {
    MAIN_ERROR("Could not start the check_reserve_proofs_timer_thread");
  }

  color_print("Started the check reserve proofs timer thread","green");
```

You will also need to change the number for the INITIALIZE_DATABASE_DATA, view the comments above this line in the code to see what number to change it to, but 6 is for 5 nodes.

At this point you can run the test setup. Make sure to run the main server (NETWORK_DATA_NODE_1_PUBLIC_ADDRESS) first, then run all of the other network data nodes. Once those are done syncing run all other servers. 

Start all of the servers right after 0 seconds of a divisible by 5 minute (0,5,10 etc) this way the block creation process will start at the next divisible by 5 minute and will give your nodes enough time to sync and start up (although this usually only takes a minute)

Also make sure you run xcashd with the `--block-verifier` flag

If your computer system does not have internet access, make sure to run the xcashd on the NETWORK_DATA_NODE_1 system with `./xcashd --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --confirm-external-bind --restricted-rpc --block-verifier` andd run the other systems with `./xcashd --allow-local-ip --add-exclusive-node IP_OF_MAIN_SERVER --block-verifier`



## How to Setup a Private Test Using LXC Containers

You can also use 5 LXC containers to setup a private test. You can share the host's folder for X-CASH and XCASH_DPOPS this way you only need to edit one code for it to refresh on all containers. You cant share the actual blockchain though so you will need each container to have its own copy of the blockchain.

**Note this will only work on a Linux host that is using a wired connection**

First make the changes above to the code at [How to Setup A Private Test](#how-to-setup-a-private-test)  

Next folow the steps at [Install LXC](#install-lxc) to install LXD

Then create a gui profile for LXD  
`lxc profile create gui`

Then create a file called GUI and pass this file to LXD. You can try either GUI as the first one is for older versions and the second one is for the latest version of LXD

Credit of this profile goes to [https://blog.simos.info/wp-content/uploads/2018/06/lxdguiprofile.txt](https://blog.simos.info/wp-content/uploads/2018/06/lxdguiprofile.txt) located in this tutorial [https://blog.simos.info/how-to-easily-run-graphics-accelerated-gui-apps-in-lxd-containers-on-your-ubuntu-desktop/](https://blog.simos.info/how-to-easily-run-graphics-accelerated-gui-apps-in-lxd-containers-on-your-ubuntu-desktop/)

This is the latest GUI profile. Again credit goes to [https://blog.simos.info/running-x11-software-in-lxd-containers/](https://blog.simos.info/running-x11-software-in-lxd-containers/)

Once you have copied the profiles content to the GUI file, remove the packages part of the profile. We will not be using the cloud-init to install the packages, since we cant know when the packages will be done, and we will be installing them using `lxc exec` instead.

Then cat the file to the gui profile  
`cat GUI | lxc profile edit gui`

Then initialize 5 containers, with the necessary packages to run X-CASH and XCASH_DPOPS and to be able to run GUI apps like vscode  

`lxc lanch --profile default --profile gui ubuntu:18.04 container1`

This will create and start the container, and will also download the ubuntu 18.04 image if it is not already downloaded on your system. Wait about 20 seconds for LXD to assign the container an IP address (you can check using `lxc list` and make sure it shows an internal IP address assigned to it) then run the following commands

```
lxc exec container1 -- apt update -y
lxc exec container1 -- apt install -y x11-apps mesa-utils pulseaudio
lxc exec container1 -- bash -c "wget -q https://packages.microsoft.com/keys/microsoft.asc -O- | sudo apt-key add -"
lxc exec container1 -- add-apt-repository -y "deb [arch=amd64] https://packages.microsoft.com/repos/vscode stable main"
lxc exec container1 -- apt update -y
lxc exec container1 -- apt install -y code
lxc exec container1 -- sudo --login --user ubuntu code --install-extension ms-vscode.cpptools
lxc exec container1 -- sudo --login --user ubuntu apt install -y gdb
lxc exec container1 -- sudo --login --user ubuntu bash
```

After this you should have a terminal open inside the container. You now need install XCASH_DPOPS using the autoinstaller in each container.

Then exit out of the container by typing `exit`

Now you will need to add 5 internal IP addresses to your host and then route all of them to each containers internal IP address.

You need to make sure that your not adding IP addresses that are already being used on the network, so make sure to check that in the router.

Once you have 5 IP addresses that are not currently being used, you can temporarily assign them to the host machine using
`sudo ip addr add IP_ADDRESS/32 dev NETWORK_INTERFACE`

You can get the network interface by running `ip addr` and look for the interface that says `<BROADCAST,MULTICAST,UP,LOWER_UP>`

**Note this will only add these IP addresses to the host for as long as the machine is running. If you reboot or shutdown you will need to run the ip addr command again on the next startup**

Now you will need to route these IP addresses to each containers internal IP address  
```
lxc config device add container container_XCASH_DPOPS_18280 proxy listen=tcp:PUBLIC_IP_OF_SERVER:18280 connect=tcp:PRIVATE_IP_OF_CONTAINER:18280 proxy_protocol=true
lxc config device add container container_XCASH_DPOPS_18281 proxy listen=tcp:PUBLIC_IP_OF_SERVER:18281 connect=tcp:PRIVATE_IP_OF_CONTAINER:18281 proxy_protocol=true
lxc config device add container container_XCASH_DPOPS_18283 proxy listen=tcp:PUBLIC_IP_OF_SERVER:18283 connect=tcp:PRIVATE_IP_OF_CONTAINER:18283 proxy_protocol=true
```

On older versions of LXD you might need to leave out the `proxy_protocol=true`

Next you need to add the host's X-CASH and XCASH_DPOPS directories to the containers  
```
lxc config device add container1 container1_XCASH disk source=/PATH_TO_HOST_X-CASH path=/root/x-network/X-CASH
lxc config device add container1 container1_XCASH_DPOPS disk source=/PATH_TO_HOST_XCASH_DPOPS path=/root/x-network/XCASH_DPOPS
```

Now you need to add the blockchain to each container from the host  
`lxc file push -r /$HOME/.X-CASH/ container/root/`

At this point you should be able to login to each container  
`lxc exec container1 -- sudo --login --user ubuntu bash`

Then start the mongodb, xcashd and xcash-wallet-rpc and then type `code` to start vscode




## How to Debug the Code on a Server
If your looking to participate in the bug bounty, having the ability to debug the code while it is running on the server will be essential. In this part, we will explain how to use visual studio code and gdb to visually debug the program on your desktop.

**Note this works best on a Linux host machine, although there are ways to get it to work with MacOS and Windows**

#### Note It is strongly recommended to run this on a Linux host inside of a virtual machine. This is due to the fact that X11 forwarding can allow someone on the server to run X11 programs on your machine, or to collect data on your currently running programs.


### Allow X11 Forwarding on the server
First, we need to make sure that SSH is setup to allow X11 forwarding. Open the SSH config file on the server  
`nano /etc/ssh/ssh_config`

Add the following line at the bottom of the file  
`ForwardX11 yes`

Restart the ssh service  
`systemctl restart sshd.service`



### Install GDB and vscode
Install gdb on the server  
`sudo apt install gdb`

Install X11 dependencies  
`sudo apt install xauth libx11-xcb1 libasound2 x11-apps libice6 libsm6 libxaw7 libxft2 libxmu6 libxpm4 libxt6 x11-apps xbitmaps libxtst6`

Install vscode on the server
```
wget -q https://packages.microsoft.com/keys/microsoft.asc -O- | sudo apt-key add -
add-apt-repository -y "deb [arch=amd64] https://packages.microsoft.com/repos/vscode stable main"
apt update -y
apt install -y code
```

[Install the C++/C extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
`code --install-extension ms-vscode.cpptools`

### Run vscode on the Server and Forward the GUI to a Dekstop

Add `-XC -c aes128-gcm@openssh.com` to the front of any ssh command that you would normally use to connect to the server.

Optionally try `-X -c aes128-gcm@openssh.com` to run without compression. On fast Internet connections no compression might be faster.

Once connected type `code` to start vscode. If root you might have to run `code --user-data-dir PATH_TO_FOLDER` to get it to run. Make sure its an empty folder only used for vscode

It might take 10-30 seconds for the vscode window to appear on your dekstop.

You can now use vscode on your dekstop but it will have the file system of the server. It will be a little laggy though, so its probably not good for writing new functions, but for quick file edits (since you have the files panel) and for debugging it should work fine.

### Change vscode Keyboard Shortcuts
Optionally change the vscode keyboard shortcuts to the following

Open the keyboard shortcuts  
`File=>Prederences=>Keyboard Shortcuts

At the search bar at the top type `saveall`. For the `File: Save All` edit it to `Control+S` and ignore the warning about there already being that keyboard shortcut

At the search bar at the top type `start debugging`. For the `Debug: Start Debugging` edit it to `Control+Shift+D` and ignore the warning about there already being that keyboard shortcut

This will now allow you to save all files with `Control+S`

To rebuild, and start the debugger use `Control+Shift+D`

If the keyboard shortcuts are not changed, then to rebuild and start the debugger it is `F5` and `Control+Shift+D` to switch to the debugger panel

The vscode files have already been written for you and are included in the `.vscode` directory
