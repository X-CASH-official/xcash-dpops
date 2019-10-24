#!/bin/bash

#########################################################
ROOT_DIR=/var/x-network
WALLET_PASSWD=`uuidgen`
DAEMON_USER=xcash
DPOPS_FEE=0
DPOPS_MINIMUM_AMOUNT=1000
LOGFILE=/var/log/x-cash-dpops-install.log
#########################################################


red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

reset 
echo
echo -e "${green}############################################################${reset}"
echo -e "${green}####### Welcome to X-Cash DPoPS auto-install script  #######${reset}"
echo -e "${green}############################################################${reset}"
echo
echo -e "${green}Installation will start in 10 sec, press Ctrl + C to cancel!${reset}"
sleep 10
echo -e "${green}Root directory:       $ROOT_DIR ${reset}"
echo -e "${green}Wallet password:      $WALLET_PASSWD ${reset}"
echo -e "${green}Daemon user:          $DAEMON_USER ${reset}"
echo -e "${green}DPoPS fee:            $DPOPS_FEE ${reset}"
echo -e "${green}DPoPS minimum amount: $DPOPS_MINIMUM_AMOUNT ${reset}"
echo -e "${green}Progress log file:    $LOGFILE ${reset}"
echo
> $LOGFILE

function check_user(){ 
    echo -ne "${green}Checking if running user is root..........................${reset}"
    if [ "$EUID" -ne 0 ]; then
        echo -e "${red}FAIL${reset}"
        echo
        echo -e "${red}############################################################${reset}"
        echo -e "${red}#######   !!!  PLEASE RUN THIS SCRIPT AS ROOT  !!!   #######${reset}"
        echo -e "${red}############################################################${reset}"
        echo
        exit
    fi

    echo -e "${green}OK${reset}"
}       


function check_ubuntu(){
    echo -ne "${green}Checking Ubuntu version...................................${reset}"
    
    command -v lsb_release > /dev/null 2>&1 ||
    {
        echo -e "${red}FAIL${reset}"
        echo 
        echo -e "${red}############################################################${reset}"
        echo -e "${red}#######   !!! CANNOT CHECK YOUR UBUNTU VERSION !!!   #######${reset}"
        echo -e "${red}############################################################${reset}"
        echo
        exit
    }

    UBUNTU_VERSION=`lsb_release -r | awk {'print $2'} | sed s"|\.||g"`
    if [ "$UBUNTU_VERSION" == "1804" ] || [ "$UBUNTU_VERSION" == "1604" ]; then
        echo -e "${green}OK${reset}"
    else
        echo -e "${red}FAIL${reset}"
        echo 
        echo -e "${red}############################################################${reset}"
        echo -e "${red}####### !!  YOUR UBUNTU VERSION IS NOT SUPPORTED !!  #######${reset}"
        echo -e "${red}############################################################${reset}"
        echo
        exit
    fi
}

function error_and_stop(){
    echo 
    echo -e "${red}############################################################${reset}"
    echo -e "${red}#######     !!! INSTALLATION PROCESS STOPPED !!!     #######${reset}"
    echo -e "${red}############################################################${reset}"
    echo
    exit
}

function system_update() {
    i=0
    tput sc
    while fuser /var/{lib/{dpkg,apt/lists},cache/apt/archives}/lock >/dev/null 2>&1; do
        case $(($i % 4)) in
            0 ) j="-" ;;
            1 ) j="\\" ;;
            2 ) j="|" ;;
            3 ) j="/" ;;
        esac
        tput rc
        echo && echo -en "\r${red}[$j] Waiting for other package manager to finish...${reset}" 
        sleep 0.25
        ((i=i+1))
    done
    echo && echo -ne "${green}Upgrading all installed packages, this can take a while...${reset}"
    apt update -y >> $LOGFILE 2>&1 && apt upgrade -y > $LOGFILE 2>&1
    echo -e "${green}OK${reset}"
}

function install_dependencies() {
    echo -ne "${green}Installing Node.js repository.............................${reset}"
    curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash - >> $LOGFILE 2>&1
    echo -e "${green}OK${reset}"

    echo -ne "${green}Installing all needed packets, this can take a while......${reset}"
    apt install -y \
        nodejs \
        unzip \
        build-essential \
        cmake \
        pkg-config \
        libssl-dev \
        git \
        gcc \
        g++ \
        make \
        libboost-all-dev \
        libzmq3-dev \
        libunbound-dev \
        libsodium-dev \
        libminiupnpc-dev \
        libunwind8-dev \
        liblzma-dev \
        libreadline6-dev \
        libldns-dev \
        libexpat1-dev \
        libgtest-dev \
        doxygen \
        graphviz \
        libpcsclite-dev >> $LOGFILE 2>&1
    echo -e "${green}OK${reset}"

}

function create_user(){
    echo -ne "${green}Creating daemon user..................................${reset}"

    getent passwd $DAEMON_USER > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${red}EXISTS${reset}"
    else
        useradd -M $DAEMON_USER
        echo -e "${green}....OK${reset}"
    fi
}

function create_dirs(){
    echo -ne "${green}Creating root dirs........................................${reset}"
    [ -d $ROOT_DIR ] || mkdir -p $ROOT_DIR

    [ -d $ROOT_DIR/build ] || mkdir -p $ROOT_DIR/build
    [ -d $ROOT_DIR/pids ] || mkdir -p $ROOT_DIR/pids
    [ -d $ROOT_DIR/logs ] || mkdir -p $ROOT_DIR/logs

    [ -d $ROOT_DIR/xcash ] || mkdir -p $ROOT_DIR/xcash
    [ -d $ROOT_DIR/xcash/bin ] || mkdir -p $ROOT_DIR/xcash/bin
    [ -d $ROOT_DIR/xcash/wallet ] || mkdir -p $ROOT_DIR/xcash/wallet
    [ -d $ROOT_DIR/xcash/blockchain ] || mkdir -p $ROOT_DIR/xcash/blockchain
    [ -d $ROOT_DIR/xcash/shared-ringdb ] || mkdir -p $ROOT_DIR/xcash/shared-ringdb

    [ -d $ROOT_DIR/mongodb ] || mkdir -p $ROOT_DIR/mongodb
    [ -d $ROOT_DIR/mongodb/bin ] || mkdir -p $ROOT_DIR/mongodb/bin
    [ -d $ROOT_DIR/mongodb/db ] || mkdir -p $ROOT_DIR/mongodb/db

    [ -d $ROOT_DIR/dpops ] || mkdir -p $ROOT_DIR/dpops
    [ -d $ROOT_DIR/dpops/shared_delegates_website ] || mkdir -p $ROOT_DIR/dpops/shared_delegates_website
    [ -d $ROOT_DIR/dpops/scripts ] || mkdir -p $ROOT_DIR/dpops/scripts

    chown -R $DAEMON_USER.$DAEMON_USER $ROOT_DIR &&
    echo -e "${green}OK${reset}"

}

install_mongodb(){
    echo -ne "${green}Downloading and installing MongoDB........................${reset}"

    MONGODB_SO_VERSION=`lsb_release -r | awk {'print $2'} | sed s"|\.||g"`
    systemctl list-units --full -all | grep -Fq "MongoDB.service"
    if [ $? -eq 0 ]; then
        systemctl stop MongoDB >> $LOGFILE 2>&1 &&
        systemctl disable MongoDB >> $LOGFILE 2>&1 &&
        rm -rf /etc/systemd/system/MongoDB.service >> $LOGFILE 2>&1 &&
        systemctl daemon-reload >> $LOGFILE 2>&1
    fi

    if [ "$(ls -A $ROOT_DIR/mongodb/db/)" ]; then
        rm -rf $ROOT_DIR/mongodb/db/*
    fi

    if [ "$(ls -A $ROOT_DIR/mongodb/bin/)" ]; then
        rm -rf $ROOT_DIR/mongodb/bin/*
    fi

    cd $ROOT_DIR/build &&
    [ -f mongodb-linux-x86_64-ubuntu$MONGODB_SO_VERSION-4.2.0.tgz ] && rm -rf mongodb-linux-x86_64-ubuntu$MONGODB_SO_VERSION-4.2.0.tgz 
    wget -q https://fastdl.mongodb.org/linux/mongodb-linux-x86_64-ubuntu$MONGODB_SO_VERSION-4.2.0.tgz &&

    [ -d mongodb-linux-x86_64-ubuntu$MONGODB_SO_VERSION-4.2.0 ] && rm -rf mongodb-linux-x86_64-ubuntu$MONGODB_SO_VERSION-4.2.0
    tar zxf mongodb-linux-x86_64-ubuntu$MONGODB_SO_VERSION-4.2.0.tgz && rm -rf mongodb-linux-x86_64-ubuntu$MONGODB_SO_VERSION-4.2.0.tgz &&
    mv mongodb-linux-x86_64-ubuntu$MONGODB_SO_VERSION-4.2.0/bin/* $ROOT_DIR/mongodb/bin/ && rm -rf mongodb-linux-x86_64-ubuntu$MONGODB_SO_VERSION-4.2.0

    chown -R $DAEMON_USER.$DAEMON_USER $ROOT_DIR &&
    echo -e "${green}OK${reset}"

}

function create_mongodb_unit_file(){
echo -ne "${green}Creating systemd unit file for MongoDB....................${reset}"
cat <<EOF>/etc/systemd/system/MongoDB.service
[Unit]
Description=X-Cash Database Daemon
After=network.target

[Service]
Type=simple
User=$DAEMON_USER
PIDFile=$ROOT_DIR/pids/MongoDB.pid
ExecStart=$ROOT_DIR/mongodb/bin/mongod --dbpath=$ROOT_DIR/mongodb/db/ --pidfilepath=$ROOT_DIR/pids/MongoDB.pid
KillMode=process
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
EOF
    
systemctl daemon-reload >> $LOGFILE 2>&1 &&
systemctl enable MongoDB >> $LOGFILE 2>&1 &&
systemctl start MongoDB >> $LOGFILE 2>&1
echo -e "${green}OK${reset}"

echo -ne "${green}Checking if MongoDB service is running....................${reset}"
    systemctl is-active --quiet MongoDB
    if [ $? -eq 0 ]; then
        echo -e "${green}OK${reset}"
    else
        error_and_stop
    fi
}


function install_xcash(){
    echo -ne "${green}Downloading and installing X-Cash Daemon..................${reset}"

    systemctl list-units --full -all | grep -Fq "XCASH_Daemon.service"
    if [ $? -eq 0 ]; then
        systemctl stop XCASH_Daemon >> $LOGFILE 2>&1 &&
        systemctl stop XCASH_Daemon_Block_Verifier >> $LOGFILE 2>&1
        systemctl disable XCASH_Daemon >> $LOGFILE 2>&1 &&
        rm -rf /etc/systemd/system/XCASH_Daemon.service >> $LOGFILE 2>&1 &&
        rm -rf /etc/systemd/system/XCASH_Daemon_Block_Verifier.service >> $LOGFILE 2>&1 &&
        systemctl daemon-reload >> $LOGFILE 2>&1
    fi

    if [ "$(ls -A $ROOT_DIR/xcash/bin/)" ]; then
        rm -rf $ROOT_DIR/xcash/bin/*
    fi

    cd $ROOT_DIR/build &&
    [ -d X-CASH ] && rm -rf X-CASH
    git clone https://github.com/X-CASH-official/X-CASH >> $LOGFILE 2>&1 && cd X-CASH &&
    make -j$(nproc) >> $LOGFILE 2>&1 &&
    mv build/release/bin/* $ROOT_DIR/xcash/bin/ &&
    cd $ROOT_DIR/build &&
    rm -rf X-CASH &&

    chown -R $DAEMON_USER.$DAEMON_USER $ROOT_DIR &&
    echo -e "${green}OK${reset}"    

}

function create_xcash_unit_file(){
echo -ne "${green}Creating systemd unit file for XCASH_Daemon...............${reset}"
cat <<EOF>/etc/systemd/system/XCASH_Daemon.service
[Unit]
Description=X-Cash Daemon
After=network.target

[Service]
Type=forking
User=$DAEMON_USER
LimitNOFILE=infinity
LimitNPROC=infinity
LimitCORE=infinity
PIDFile=$ROOT_DIR/pids/XCASH_Daemon.pid
ExecStart=$ROOT_DIR/xcash/bin/xcashd --data-dir $ROOT_DIR/xcash/blockchain --log-file $ROOT_DIR/logs/blockchain.log --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind --detach --pidfile $ROOT_DIR/pids/XCASH_Daemon.pid
RuntimeMaxSec=15d
Restart=always

[Install]
WantedBy=multi-user.target
EOF

cat <<EOF>/etc/systemd/system/XCASH_Daemon_Block_Verifier.service
[Unit]
Description=X-Cash Block Verifier Daemon
After=network.target

[Service]
Type=forking
User=$DAEMON_USER
LimitNOFILE=infinity
LimitNPROC=infinity
LimitCORE=infinity
PIDFile=$ROOT_DIR/pids/XCASH_Daemon.pid
ExecStart=$ROOT_DIR/xcash/bin/xcashd --data-dir $ROOT_DIR/xcash/blockchain --log-file $ROOT_DIR/logs/blockchain.log --block-verifier --rpc-bind-ip 0.0.0.0 --rpc-bind-port 18281 --restricted-rpc --confirm-external-bind --detach --pidfile $ROOT_DIR/pids/XCASH_Daemon.pid
RuntimeMaxSec=15d
Restart=always

[Install]
WantedBy=multi-user.target
EOF

systemctl daemon-reload >> $LOGFILE 2>&1 &&
systemctl enable XCASH_Daemon >> $LOGFILE 2>&1 &&
systemctl start XCASH_Daemon >> $LOGFILE 2>&1
echo -e "${green}OK${reset}"

echo -ne "${green}Checking if XCASH_Daemon service is running...............${reset}"
    systemctl is-active --quiet XCASH_Daemon
    if [ $? -eq 0 ]; then
        echo -e "${green}OK${reset}"
    else
        error_and_stop
        exit
    fi
}

function create_node_wallet(){
echo -ne "${green}Creating node's wallet....................................${reset}"

systemctl list-units --full -all | grep -Fq "XCASH_Wallet.service"
if [ $? -eq 0 ]; then
    systemctl stop XCASH_Wallet >> $LOGFILE 2>&1
fi

if [ "$(ls -A $ROOT_DIR/xcash/wallet/)" ]; then
    rm -rf $ROOT_DIR/xcash/wallet/*
fi

cat <<EOF>/etc/systemd/system/x-cash-temporal-rpc-wallet.service
[Unit]
Description=X-Cash RPC Wallet Daemon
After=network.target

[Service]
Type=simple
User=$DAEMON_USER
LimitNOFILE=infinity
LimitNPROC=infinity
LimitCORE=infinity
ExecStart=$ROOT_DIR/xcash/bin/xcash-wallet-rpc --log-file /dev/null --shared-ringdb-dir $ROOT_DIR/xcash/shared-ringdb --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon --wallet-dir $ROOT_DIR/xcash/wallet/
ExecStop=/bin/kill -9
KillMode=process
Restart=always

[Install]
WantedBy=multi-user.target
EOF

systemctl daemon-reload >> $LOGFILE 2>&1 &&
systemctl start x-cash-temporal-rpc-wallet >> $LOGFILE 2>&1 && sleep 5

systemctl is-active --quiet x-cash-temporal-rpc-wallet
if [ $? -eq 0 ]; then
    curl -s -X POST http://127.0.0.1:18285/json_rpc -d "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"create_wallet\",\"params\":{\"filename\":\"node-wallet\",\"password\":\"$WALLET_PASSWD\",\"language\":\"English\"}}" -H 'Content-Type: application/json' >> $LOGFILE 2>&1
    systemctl stop x-cash-temporal-rpc-wallet >> $LOGFILE 2>&1
    rm -rf /etc/systemd/system/x-cash-temporal-rpc-wallet.service
    systemctl daemon-reload >> $LOGFILE 2>&1
else
    error_and_stop
fi

echo -e "${green}OK${reset}"

}

function create_xcash_rpc_unit_file(){
echo -ne "${green}Creating systemd unit file for XCASH_Wallet...............${reset}"
cat <<EOF>/etc/systemd/system/XCASH_Wallet.service
[Unit]
Description=X-Cash RPC Wallet Daemon
After=network.target XCASH_Daemon.service

[Service]
Type=simple
User=$DAEMON_USER
LimitNOFILE=infinity
LimitNPROC=infinity
LimitCORE=infinity
ExecStart=$ROOT_DIR/xcash/bin/xcash-wallet-rpc --wallet-file $ROOT_DIR/xcash/wallet/node-wallet --password $WALLET_PASSWD --log-file $ROOT_DIR/logs/rpc.log --shared-ringdb-dir $ROOT_DIR/xcash/shared-ringdb --rpc-bind-port 18285 --confirm-external-bind --daemon-port 18281 --disable-rpc-login --trusted-daemon
Restart=always

[Install]
WantedBy=multi-user.target
EOF

systemctl daemon-reload >> $LOGFILE 2>&1
systemctl enable XCASH_Wallet >> $LOGFILE 2>&1

systemctl start XCASH_Wallet >> $LOGFILE 2>&1
systemctl is-active --quiet XCASH_Wallet
if [ $? -eq 0 ]; then
    echo -e "${green}OK${reset}"
else
    error_and_stop
fi

}

function install_mongo_c_driver(){
    echo -ne "${green}Downloading and installing Mongo C Driver.................${reset}"

    cd $ROOT_DIR/build &&
    [ -f mongo-c-driver-1.15.1.tar.gz ] && rm -rf mongo-c-driver-1.15.1.tar.gz
    wget -q https://github.com/mongodb/mongo-c-driver/releases/download/1.15.1/mongo-c-driver-1.15.1.tar.gz &&

    [ -d mongo-c-driver-1.15.1/ ] && rm -rf mongo-c-driver-1.15.1/
    tar zxf mongo-c-driver-1.15.1.tar.gz && rm -rf mongo-c-driver-1.15.1.tar.gz &&
    cd mongo-c-driver-1.15.1/ && mkdir cmake-build && cd cmake-build &&
    cmake -j -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF .. > $LOGFILE 2>&1 &&
    make -j$(nproc) > $LOGFILE 2>&1 &&
    make install > $LOGFILE 2>&1 &&
    cd $ROOT_DIR/build &&
    rm -rf mongo-c-driver-1.15.1/
    ldconfig

    chown -R $DAEMON_USER.$DAEMON_USER $ROOT_DIR &&
    echo -e "${green}OK${reset}"    

}

function install_dpops(){
    echo -ne "${green}Downloading and installing DPOPS..........................${reset}"

    systemctl list-units --full -all | grep -Fq "XCASH_DPOPS.service"
    if [ $? -eq 0 ]; then
        systemctl stop XCASH_DPOPS >> $LOGFILE 2>&1 &&
        systemctl disable XCASH_DPOPS >> $LOGFILE 2>&1 &&
        rm -rf /etc/systemd/system/XCASH_DPOPS.service >> $LOGFILE 2>&1 &&
        systemctl daemon-reload >> $LOGFILE 2>&1
    fi

    cd $ROOT_DIR/build &&
    [ -f $ROOT_DIR/dpops/XCASH_DPOPS ] && rm -rf $ROOT_DIR/dpops/XCASH_DPOPS
    [ -d XCASH_DPOPS ] && rm -rf XCASH_DPOPS
    git clone https://github.com/X-CASH-official/XCASH_DPOPS >> $LOGFILE 2>&1 && cd XCASH_DPOPS &&
    make -j$(nproc) >> $LOGFILE 2>&1 &&
    mv build/XCASH_DPOPS $ROOT_DIR/dpops/ &&
    cd $ROOT_DIR/build &&
    rm -rf XCASH_DPOPS &&

    chown -R $DAEMON_USER.$DAEMON_USER $ROOT_DIR &&
    echo -e "${green}OK${reset}"    


}

function create_dpops_unit_file(){
echo -ne "${green}Creating systemd unit file for XCASH_DPOPS................${reset}"

cat <<EOF>/etc/systemd/system/XCASH_DPOPS.service
[Unit]
Description=X-Cash DPoPS Shared Delegates
After=network.target XCASH_Wallet.service MongoDB.service

[Service]
Type=simple
LimitNOFILE=64000
User=$DAEMON_USER
WorkingDirectory=$ROOT_DIR/dpops/
ExecStart=$ROOT_DIR/dpops/XCASH_DPOPS --shared_delegates_website --fee $DPOPS_FEE --minimum_amount $DPOPS_MINIMUM_AMOUNT
KillMode=process
Restart=always

[Install]
WantedBy=multi-user.target
EOF

systemctl daemon-reload >> $LOGFILE 2>&1
systemctl enable XCASH_DPOPS >> $LOGFILE 2>&1

systemctl start XCASH_DPOPS >> $LOGFILE 2>&1
systemctl is-active --quiet XCASH_DPOPS
if [ $? -eq 0 ]; then
    echo -e "${green}OK${reset}"
else
    error_and_stop
fi

}

function install_dpops_shared_delegates_website(){
    echo -ne "${green}Downloading and installing DPOPS Delegates Website........${reset}"

    if [ "$(ls -A $ROOT_DIR/dpops/shared_delegates_website)" ]; then
        rm -rf $ROOT_DIR/dpops/shared_delegates_website/*
    fi

    cd $ROOT_DIR/build &&
    [ -d  XCASH_DPOPS_shared_delegates_website ] && rm -rf XCASH_DPOPS_shared_delegates_website
    git clone https://github.com/X-CASH-official/XCASH_DPOPS_shared_delegates_website >> $LOGFILE 2>&1 &&
    cd XCASH_DPOPS_shared_delegates_website &&
    export NG_CLI_ANALYTICS=false
    npm config set user 0 >> $LOGFILE 2>&1
    npm config set unsafe-perm true >> $LOGFILE 2>&1
    npm -g install npm >> $LOGFILE 2>&1
    npm -g install @angular/cli@latest >> $LOGFILE 2>&1
    npm -g install uglify-js >> $LOGFILE 2>&1
    npm update >> $LOGFILE 2>&1
    npm audit fix >> $LOGFILE 2>&1
    ng build --prod --aot >> $LOGFILE 2>&1 &&
    cd dist/ &&
    for f in *.js; do uglifyjs $f --compress --mangle --output "{$f}min" >> $LOGFILE 2>&1; rm -rf $f; mv "{$f}min" $f; done
    mv * $ROOT_DIR/dpops/shared_delegates_website
    cd $ROOT_DIR

    chown -R $DAEMON_USER.$DAEMON_USER $ROOT_DIR &&
    echo -e "${green}OK${reset}"    

}

function install_firewall(){
echo -ne "${green}Installing firewall rules script for DPOPS................${reset}"

cat <<EOF>$ROOT_DIR/dpops/scripts/firewall_script.sh
#!/bin/sh
# iptables script for server
# if you changed any default ports change them in the firewall as well
 
# ACCEPT all packets at the top so each packet runs through the firewall rules, then DROP all INPUT and FORWARD if they dont use any of the firewall settings
iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -P OUTPUT ACCEPT
# remove all existing IP tables
iptables -t nat -F
iptables -t mangle -F
iptables -t mangle -X
iptables -t mangle -F
iptables -t raw -F
iptables -t raw -X
iptables -F
iptables -X
 
# ip table prerouting data (this is where you want to block ddos attacks)
# Drop all invalid packets
iptables -t mangle -A PREROUTING -m conntrack --ctstate INVALID -j DROP
# Prevent syn flood
iptables -A INPUT -p tcp ! --syn -m state --state NEW -j DROP
iptables -t mangle -A PREROUTING -p tcp -m conntrack --ctstate NEW -m tcpmss ! --mss 536:65535 -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags FIN,SYN,RST,PSH,ACK,URG NONE -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags FIN,SYN FIN,SYN -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags SYN,RST SYN,RST -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags FIN,RST FIN,RST -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags FIN,ACK FIN -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags ACK,URG URG -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags ACK,FIN FIN -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags ACK,PSH PSH -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags ALL ALL -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags ALL NONE -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags ALL FIN,PSH,URG -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags ALL SYN,FIN,PSH,URG -j DROP
iptables -t mangle -A PREROUTING -p tcp --tcp-flags ALL SYN,RST,ACK,FIN,URG -j DROP
 
# filter data for INPUT, FORWARD, and OUTPUT
# Accept any packets coming or going on localhost
iptables -I INPUT -i lo -j ACCEPT
# keep already established connections running
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
 
# block ip spoofing. these are the ranges of local IP address.
iptables -A INPUT -s 45.76.169.83 -j DROP
iptables -A INPUT -s 10.0.0.0/8 -j DROP
iptables -A INPUT -s 169.254.0.0/16 -j DROP
iptables -A INPUT -s 172.16.0.0/12 -j DROP
iptables -A INPUT -s 127.0.0.0/8 -j DROP
iptables -A INPUT -s 192.168.0.0/24 -j DROP
iptables -A INPUT -s 224.0.0.0/4 -j DROP
iptables -A INPUT -d 224.0.0.0/4 -j DROP
iptables -A INPUT -s 240.0.0.0/5 -j DROP
iptables -A INPUT -d 240.0.0.0/5 -j DROP
iptables -A INPUT -s 0.0.0.0/8 -j DROP
iptables -A INPUT -d 0.0.0.0/8 -j DROP
iptables -A INPUT -d 239.255.255.0/24 -j DROP
iptables -A INPUT -d 255.255.255.255 -j DROP
 
# block all traffic from ip address (iptables -A INPUT -s ipaddress -j DROP)
#unblock them using iptables -D INPUT -s ipaddress -j DROP
 
# Block different attacks
# block one computer from opening too many connections (100 simultaneous connections) if this gives trouble with post remove this or increase the limit
iptables -t filter -I INPUT -p tcp --syn --dport 80 -m connlimit --connlimit-above 100 --connlimit-mask 32 -j DROP
iptables -t filter -I INPUT -p tcp --syn --dport 18283 -m connlimit --connlimit-above 100 --connlimit-mask 32 -j DROP
# block port scans
# this will lock the IP out for 1 day
iptables -A INPUT -m recent --name portscan --rcheck --seconds 86400 -j DROP
iptables -A FORWARD -m recent --name portscan --rcheck --seconds 86400 -j DROP
iptables -A INPUT -m recent --name portscan --remove
iptables -A FORWARD -m recent --name portscan --remove
iptables -A INPUT   -p tcp -m tcp -m multiport --destination-ports 21,25,110,135,139,143,445,1433,3306,3389 -m recent --name portscan --set -j DROP 
iptables -A FORWARD -p tcp -m tcp -m multiport --destination-ports 21,25,110,135,139,143,445,1433,3306,3389 -m recent --name portscan --set -j DROP
 
# Accept specific packets
# Accept ICMP
iptables -A INPUT -p icmp -j ACCEPT
 
# Accept HTTP
iptables -A INPUT -p tcp --dport 80 -j ACCEPT

 
# Accept XCASH
iptables -A INPUT -p tcp --dport 18280 -j ACCEPT
iptables -A INPUT -p tcp --dport 18281 -j ACCEPT
iptables -A INPUT -p tcp --dport 18283 -j ACCEPT
 
# Allow ssh (allow 10 login attempts in 1 hour from the same ip, if more than ban them for 1 hour)
iptables -A INPUT -p tcp -m tcp --dport 22 -m state --state NEW -m recent --set --name DEFAULT --rsource
iptables -A INPUT -p tcp -m tcp --dport 22 -m state --state NEW -m recent --update --seconds 3600 --hitcount 10 --name DEFAULT --rsource -j DROP
iptables -A INPUT -p tcp -m tcp --dport 22 -j ACCEPT
 
# Redirect HTTP to port 18283
iptables -A PREROUTING -t nat -p tcp --dport 80 -j REDIRECT --to-ports 18283
 
# DROP all INPUT and FORWARD packets if they have reached this point
iptables -A INPUT -j DROP
iptables -A FORWARD -j DROP
EOF

chown -R $DAEMON_USER.$DAEMON_USER $ROOT_DIR &&
chmod +x $ROOT_DIR/dpops/scripts/firewall_script.sh
echo -e "${green}OK${reset}"    

}

function create_firewall_unit_file(){
echo -ne "${green}Creating systemd unit file for XCASH_firewall.............${reset}"

cat <<EOF>/etc/systemd/system/XCASH_firewall.service
[Unit]
Description=X-Cash Firewall

[Service]
Type=oneshot
User=root
ExecStart=$ROOT_DIR/dpops/scripts/firewall_script.sh

[Install]
WantedBy=multi-user.target
EOF

systemctl daemon-reload >> $LOGFILE 2>&1
systemctl enable XCASH_firewall >> $LOGFILE 2>&1

systemctl start XCASH_firewall >> $LOGFILE 2>&1
if [ $? -eq 0 ]; then
    echo -e "${green}OK${reset}"
else
    error_and_stop
fi

}

function generate_info_scripts(){
echo -ne "${green}Creating x-cash-wallet-info script........................${reset}"
#!/bin/bash

cat <<\EOF>/usr/local/bin/x-cash-wallet-info
#!/bin/bash

if lsof -Pi :18285 -sTCP:LISTEN -t >/dev/null ; then
    PUBLIC_ADDRESS=`curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"get_address"}' -H 'Content-Type: application/json' | grep \"address\" | head -1 | sed s"|    \"address\": ||g" | sed s"|\"||g" | sed s"|,||g"`
    SPEND_KEY=`curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"query_key","params":{"key_type":"spend_key"}}' -H 'Content-Type: application/json' | grep \"key\" | sed s"|    \"key\": ||g" | sed s"|\"||g"`
    VIEW_KEY=`curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"query_key","params":{"key_type":"view_key"}}' -H 'Content-Type: application/json' | grep \"key\" | sed s"|    \"key\": ||g" | sed s"|\"||g"`
    MNEMONIC_SEED=`curl -s -X POST http://127.0.0.1:18285/json_rpc -d '{"jsonrpc":"2.0","id":"0","method":"query_key","params":{"key_type":"mnemonic"}}' -H 'Content-Type: application/json' | grep \"key\" | sed s"|    \"key\": ||g" | sed s"|\"||g"`

    echo "Public Address: $PUBLIC_ADDRESS"
    echo "Mnemonic Seed:  $MNEMONIC_SEED"
    echo "Spend Key:      $SPEND_KEY"
    echo "View Key:       $VIEW_KEY"
else
    echo "RPC Wallet is not running or it is in sync state...."
    echo "Check the XCASH_Wallet service status, if running try later"
fi    
EOF

chmod +x /usr/local/bin/x-cash-wallet-info
echo -e "${green}OK${reset}"    

}

function print_info(){
    echo
    echo -e "${green}############################################################${reset}"
    echo -e "${green}#######   !!!   SCRIPT FINISHED INSTALLATION   !!!   #######${reset}"
    echo -e "${green}############################################################${reset}"
    echo

    echo -e "${green}You can check the service status with the following commands${reset}"
    echo -e "${green}  - systemctl status MongoDB${reset}"
    echo -e "${green}  - systemctl status XCASH_Daemon${reset}"
    echo -e "${green}  - systemctl status XCASH_Daemon_Block_Verifier${reset}"
    echo -e "${green}  - systemctl status XCASH_Wallet${reset}"
    echo -e "${green}  - systemctl status XCASH_DPOPS${reset}"
    echo
    echo -e "${green}You can get your wallet info executing \"x-cash-wallet-info\"${reset}"
    echo
    echo -e "${green}  * Wait for the XCASH_Daemon to finish the blockchain sync before start XCASH_DPOPS${reset}"
    echo -e "${green}  * You can find all logs at $ROOT_DIR/logs${reset}"
    echo -e "${green}  * The XCASH_Daemon_Block_Verifier will start on need, you don't need to run it manually${reset}"

}

function clean_build_dir(){
    echo -ne "${green}Cleaning build directory..................................${reset}"
    rm -rf $ROOT_DIR/build/*
    echo -e "${green}OK${reset}"

}

# Base stuff
check_user
check_ubuntu
system_update
install_dependencies
create_user
create_dirs

# Install and create unit file for MongoDB.service
install_mongodb
create_mongodb_unit_file

# Install and create unit file for XCASH_Daemon.service
install_xcash
create_xcash_unit_file

# Install and create unit file for XCASH_Wallet.service
create_node_wallet
create_xcash_rpc_unit_file

# Install and create unit file for XCASH_DPOPS.service
install_mongo_c_driver
install_dpops
create_dpops_unit_file

# Install and configure DPOPS Delegates Website
install_dpops_shared_delegates_website

# Install and create unit file for XCASH_firewall.service
install_firewall
create_firewall_unit_file

# Generate informational scripts
generate_info_scripts

# Clean build dir
clean_build_dir

# Print info
print_info
