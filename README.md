# X-CASH Proof of stake

More details will be released soon!

## Installation

This program will only run on a Linux/Unix OS at this time. We recommend installing this on a Ubuntu VPS/Server (16.04 or 18.04) for the best compatibility.

You will also need to run an X-CASH Daemon and X-CASH RPC wallet on the server. You can either download the latest X-CASH [release](https://github.com/X-CASH-official/X-CASH/releases) or build from [source](https://github.com/X-CASH-official/X-CASH#compiling-x-cash-from-source)



## Compiling X-CASH Proof of stake



### Dependencies

The following table summarizes the tools and libraries required to build. 

| Dependencies                                 | Min. version  | Ubuntu package            |
| -------------------------------------------- | ------------- | ------------------------- |
| GCC                                          | 4.7.3         | `build-essential`         | 
| CMake                                        | 3.0.0         | `cmake`                   | 
| Git      		                       | any           | `git`                     |



### Cloning the repository

`$ git clone https://github.com/X-CASH-official/XCASH_proof_of_stake.git`



### Build instructions

X-CASH Proof of stake uses a Make file.

After cloning the repository, navigate to the folder  
`cd XCASH_proof_of_stake`

Then use the make file to build the binary file  
`make clean ; make`



## Setting up the xcashd and xcash-wallet-RPC

First you will need to run xcashd in the background. Navigate to the folder that contains the xcash binaries, then run  
`screen -dmS ./xcashd`

Next you need to run a xcash-wallet-rpc. This wallet file needs to include the public XCA address you registered on the voting website.

Our recommendation is you use an empty wallet (no X-CASH in it) for this address. This way you will only leave mining rewards that you receive to this address on the VPS, not the entire X-CASH that you have. We also recommend at least once a week you should move the mining reward X-CASH that this wallet collects to your personal wallet.

If your running a shared masternode you will need to leave most of the mining rewards on the VPS, since the [link to shared masternode github repo](link to shared masternode github repo) will automatically pay the people contributing to your node.



## Running X-CASH Proof of stake test
It is recomeneded to run the X-CASH Proof of stake test before you run the main program. The test will ensure that your system is compatbile, and that you have setup your system correctly.

To run the X-CASH Proof of stake test, Navigate to the folder that contains the binary, then run  
`./xcash_proof_of_stake --test`

The test will return the number of passed and failed test on the bottom of the console. The failed test need to be 0 before you run the node. If the output is not showing 0 for failed test, then you need to scroll through the testing output and find what test failed (It will be red instead of green). If this is a system compatibility test, then you will need to fix the system. If this is a core test that has failed, then you need to possibly rebuild, or contact us with your OS version, and we can look into it.



## Running X-CASH Proof of stake
Then you will need to run the xcash_proof_of_stake. Navigate to the folder that contains the binary, then run  
`./xcash_proof_of_stake`