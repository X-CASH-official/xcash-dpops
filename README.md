# X-CASH Proof of stake

X-CASH Proof of stake is a variation of DPOS and DBFT. The key features of X-CASH proof of stake are:

* The top 100 delegates are elected as block verifiers
* Reserve proof based voting/staking system
* A minimum of 2 million X-CASH is needed to vote for a delegate

* The election process is every block, meaning a new vote will get counted for the next block
* No need to cancel a vote, as it will automatically get cancelled if you change your vote to a different delegate
* No lockup times, the coins always remain in your wallet and you can use them at any time, although moving them from your wallet will cancel your entire staking vote.
* No fees for voting, and you can revote or switch your vote as many times as you like.

* Using a variation of Delegated Byzantine Fault tolerance consensus
* DBFT allows for 33% of the elected block verifiers to stop working, and the system will still be able to produce a new block
* Using Verifiable Random Functions to select the next block producer in the system. This allows for a random, but provable way of selecting the next block producer.

## Installation

This program will only run on a Linux/Unix OS at this time. We recommend installing this on a Ubuntu VPS/Server (16.04 or 18.04) for the best compatibility.

Minimum System Requirements:
Operating System: Ubuntu 18.04 VPS/server or higher
CPU: At least 2 threads
RAM: 2GB
Hard drive: 50GB
Bandwidth Transfer: 500GB per month
Bandwidth Speed: At least 100 Mbps

Recommended System Requirements:
Operating System: Ubuntu 18.04 VPS/server or higher
CPU: At least 4 threads
RAM: 4GB
Hard drive: 100GB
Bandwidth Transfer: 2TB per month
Bandwidth Speed: At least 100 Mbps

You will also need to run an X-CASH Daemon and X-CASH RPC wallet on the server. You can either download the latest X-CASH [release](https://github.com/X-CASH-official/X-CASH/releases) or build from [source](https://github.com/X-CASH-official/X-CASH#compiling-x-cash-from-source)



## Compiling X-CASH Proof of stake from source



### Dependencies

The following table summarizes the tools and libraries required to build. 

| Dependencies                                 | Min. version  | Ubuntu package            |
| -------------------------------------------- | ------------- | ------------------------- |
| GCC                                          | 4.7.3         | `build-essential`         | 
| CMake                                        | 3.0.0         | `cmake`                   | 
| pkg-config                                   | any           | `pkg-config`              | 
| OpenSSL      		                       | any           | `libssl-dev`              |
| Git      		                       | any           | `git`                     |
| MongoDB                                      | 4.0.3         |  install from binaries    |
| MongoDB C Driver (includes BSON libary)      | 1.13.1        |  build from source        |



### Installing MongoDB from binaries

Visit [https://www.mongodb.com/download-center/community](https://www.mongodb.com/download-center/community)

Then choose your OS, and make sure the version is the current version and the package is server. Then click on All version binaries. Now find the current version to download. You do not want the debug symbols or the rc version, just the regular current version.

Once you have downloaded the file move the file to a location where you want to keep the binaries, then run this set of commands  
`tar -xf mongodb-linux-x86_64-*.tgz && rm mongodb-linux-x86_64-*.tgz && sudo mkdir -p /data/db && sudo chmod 770 /data/db && sudo chown $USER /data/db`



### Building the MongoDB C driver from source

Visit the offical websites installation instructions at [http://mongoc.org/libmongoc/current/installing.html](http://mongoc.org/libmongoc/current/installing.html)
You will need to follow the instructions for [Building from a release tarball](http://mongoc.org/libmongoc/current/installing.html#building-from-a-release-tarball) or [Building from git](http://mongoc.org/libmongoc/current/installing.html#building-from-git) since you need the header files, not just the library files.

After you have built the MongoDB C driver from source, you will need to run  
`sudo ldconfig`



### Adding MongoDB to your PATH
You will probably want to add MongoDB to your path so you can run MonogDB by typing `mongod` at any terminal.

To add MongoDB to your PATH (replace "MongoDB_folder" with the location of the bin folder in the folder you installed MongoDB in  
`echo -e '\nexport PATH=MongoDB_folder:$PATH' >> ~/.profile && source ~/.profile`



### Cloning the repository

`$ git clone https://github.com/X-CASH-official/XCASH_proof_of_stake.git`



### Build instructions

X-CASH Proof of stake uses a Make file.

After cloning the repository, navigate to the folder  
`cd XCASH_proof_of_stake`

Then use the make file to build the binary file  
`make clean ; make`



## Running MongoDB
To run MongoDB you will need to navigate to the folder you downloaded the binaries to, and in the bin folder run mongod by running  
`./mongod`

If you have already added MongoDB to your path, you can just type in any terminal
`mongod`



## Setting up the xcashd and xcash-wallet-RPC

First you will need to run xcashd in the background. Navigate to the folder that contains the xcash binaries, then run  
`./xcashd`

Next you need to run a xcash-wallet-rpc. Depending on if this is the consensus node or the consensus backup node, you will need to the run the wallet that contains the public address in the [Proof of stake](https://github.com/X-CASH-official/Proof_of_stake) for the CONSENSUS_NODE_PUBLIC_ADDRESS or CONSENSUS_BACKUP_NODE_PUBLIC_ADDRESS

To run the rpc wallet you can run  
`./xcash-wallet-rpc --wallet-file NAME_OF_WALLET_FILE --password WALLET_FILE_PASSWORD --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon`

Just replace NAME_OF_WALLET_FILE with the name of your wallet file and WALLET_FILE_PASSWORD with the password of that wallet file. Make sure to use port 18285 as this is the port that is used in the program.

We suggest you use the screen command to run the program in the background, this way you can still maintenance the server. To do this run  
`screen -dmS Daemon ./xcashd`

You can also run the RPC wallet this way as well  
`screen -dmS RPC-Wallet ./xcash-wallet-rpc --wallet-file NAME_OF_WALLET_FILE --password WALLET_FILE_PASSWORD --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon`

To bring the screen from the bacground process to the active process run  
`screen -x NAME_OF_BACKGROUNDS_SCREEN`

Where NAME_OF_BACKGROUNDS_SCREEN would be Daemon or RPC-Wallet in the above examples.

To exit a screen if it is the active process, you can press Control + C. To exit a screen that is a background process you can run  
`screen -XS NAME_OF_BACKGROUNDS_SCREEN quit`



## Running X-CASH Proof of stake test
It is recomeneded to run the X-CASH Proof of stake test before you run the main program. The test will ensure that your system is compatbile, and that you have setup your system correctly.

To run the X-CASH Proof of stake test, Navigate to the folder that contains the binary, then run  
`./xcash_proof_of_stake --test`

The test will return the number of passed and failed test on the bottom of the console. The failed test need to be 0 before you run the node. If the output is not showing 0 for failed test, then you need to scroll through the testing output and find what test failed (It will be red instead of green). If this is a system compatibility test, then you will need to fix the system. If this is a core test that has failed, then you need to possibly rebuild, or contact us with your OS version, and we can look into it.



## Running X-CASH Proof of stake

Then you will need to run the xcash_proof_of_stake_consensus_node. Navigate to the folder that contains the binary, then run  
`./xcash_proof_of_stake`

We suggest you use the screen command to run the program in the background, this way you can still maintenance the server. To do this run  
`screen -dmS xcash_proof_of_stake ./xcash_proof_of_stake`

To bring the screen from the bacground process to the active process run  
`screen -x NAME_OF_BACKGROUNDS_SCREEN`

Where NAME_OF_BACKGROUNDS_SCREEN would be xcash_proof_of_stake_consensus_node in the above example.

To exit a screen if it is the active process, you can press Control + C. To exit a screen that is a background process you can run  
`screen -XS NAME_OF_BACKGROUNDS_SCREEN quit`