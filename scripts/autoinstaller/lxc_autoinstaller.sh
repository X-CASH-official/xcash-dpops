#!/bin/bash

# Color print variables
COLOR_PRINT_RED="\033[1;31m"
COLOR_PRINT_GREEN="\033[1;32m"
COLOR_PRINT_YELLOW="\033[1;33m"
END_COLOR_PRINT="\033[0m"

USERNAME=$(whoami)
XCASH_LXC_AUTOINSTALLER_MENU_OPTION=1
XCASH_HOST_DATA_BASE_DIR=${HOME}/xcash-shared-node/
XCASH_AUTOINSTALLER_NAME="autoinstaller.sh"
XCASH_AUTOINSTALLER_INSTALL_OPTION_NUMBER=1
XCASH_AUTOINSTALLER_CONFIGURATOR_OPTION_NUMBER=15
XCASH_AUTOINSTALLER_REGISTER_DELEGATE_OPTION_NUMBER=10
XCASH_AUTOINSTALLER_FIREWALL_OPTION_NUMBER=16
XCASH_AUTOINSTALLER_FIREWALL_SHARED_OPTION_NUMBER=17
XCASH_MAINTENANCE_SCRIPT_NAME="xcash-maintenance"
XCASH_DPOPS_SRV="xcash-dpops"
XCASH_AUTOINSTALLER_SCRIPT_SCRATCH_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/autoinstaller/autoinstaller.sh"
XCASH_MAINTENANCE_SCRIPT_SCRATCH_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/management/xcash-maintenance"
XCASH_AUTOINSTALLER_HOST_FIREWALL_URL="https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/autoinstaller/autoinstaller.sh"

# If Opacity is "NO" will be used the direct download link. If Opacity is "YES" will be used the Opacity CLI download tool with the possibility to download a given file handle (asked to the user from stdin)
DOWNLOAD_USE_OPACITY_DOWNLOADER="YES"
DOWNLOAD_TOOL_URL_OPACITY_CLI="https://github.com/n3me5is-git/opacity-cly-py.git"
DIRECT_DOWNLOAD_URL=""

XCASH_LXC_IMAGE_NAME="xcash-shared-node-image"
XCASH_LXC_CONTAINER_NAME="xcash-shared-node"
XCASH_LXC_PROFILE_RUN_NAME="xcash-node"
XCASH_LXC_PROFILE_SCRATCH_INIT_NAME="xcash-node-scratch-init"

VPS_MAIN_ADAPTER=$(ip route | grep default | sed -e "s/^.*dev.//" -e "s/.proto.*//")
VPS_EXTERNAL_IP=$(ip addr show $VPS_MAIN_ADAPTER | grep "inet\b" | awk '{print $2}' | cut -d/ -f1)
LXC_BRIDGE_IP_RANGE="10.12.242.1/24"
LXC_BRIDGE_IPV6_RANGE="fd42:a89:1b3f:e728::1/64"
LXC_BRIDGE_DEV_NAME="lxdbr0"
LXC_DEFAULT_ZFSPOOL_SIZE="15GB"
LXC_XCASH_CONTAINER_IP="10.12.242.10"
# Set this to "UTC" or your timezone (for example "Europe/Rome")
LXC_CONTAINER_TIMEZONE="UTC"

LXD_INIT_CONFIG="$(cat << EOF
config: {}
networks:
- config:
    ipv4.address: ${LXC_BRIDGE_IP_RANGE}
    ipv4.nat: "true"
    ipv6.address: ${LXC_BRIDGE_IPV6_RANGE}
    ipv6.nat: "true"
  description: ""
  name: ${LXC_BRIDGE_DEV_NAME}
  type: bridge
storage_pools:
- config:
    size: ${LXC_DEFAULT_ZFSPOOL_SIZE}
    source: /var/snap/lxd/common/lxd/disks/default.img
    zfs.pool_name: default
  description: ""
  name: default
  driver: zfs
profiles:
- config: {}
  description: Default LXD profile
  devices:
    eth0:
      name: eth0
      network: ${LXC_BRIDGE_DEV_NAME}
      type: nic
    root:
      path: /
      pool: default
      type: disk
  name: default
EOF
)"


LXC_XCASH_PROFILE_RUN="$(cat << EOF
description: Xcash-shared-node-profile-node-run
config: 
  # Set IDMAP to enable writing on bind mounts (disk devices)
  raw.idmap: |
    uid 1000 1000
    gid 1000 1000
  # Boot autostart configuration
  boot.autostart: true
  # Set ENV variables
  # User variable, used by X-cash autoinstaller
  environment.USER: root
# Configure devices: network, bind-mounts and port forwarding (proxy)
devices:
  # Bridge network (if static IP is used, must be on the subnet - see ifconfig)
  eth0:
    name: eth0
    network: ${LXC_BRIDGE_DEV_NAME}   # Bridge host device 
    type: nic
    ipv4.address: ${LXC_XCASH_CONTAINER_IP}    # Gateway is XX.XX.XX.1
  # Root device for container (same of default profile - do not modify)
  root:
    path: /
    pool: default
    type: disk
  # START bind-mount definition
  # Daemon and RPC-Wallet Logs
  xcash-daemon-and-wallet-logs:
    source: ${XCASH_HOST_DATA_BASE_DIR}logs/xcash-daemon-and-wallet-logs/
    path: /root/xcash-official/logs/
    required: true
    readonly: false
    type: disk  
  # Syslog Folder (DPOPS daemon logs are in syslog)
  xcash-dpops-logs:
    source: ${XCASH_HOST_DATA_BASE_DIR}logs/xcash-node-syslogs/
    path: var/log/syslog-container/
    required: true
    readonly: false
    type: disk  
  # Wallets folder
  xcash-wallets:
    source: ${XCASH_HOST_DATA_BASE_DIR}xcash-wallets/
    path: /root/xcash-official/xcash-wallets/
    required: true
    readonly: false
    type: disk  
  # Service files 
  xcash-services-mongodb:
    source: ${XCASH_HOST_DATA_BASE_DIR}xcash-services/mongodb.service
    path: /lib/systemd/system/mongodb.service
    required: true
    readonly: false
    type: disk 
  xcash-services-daemon:
    source: ${XCASH_HOST_DATA_BASE_DIR}xcash-services/xcash-daemon.service
    path: /lib/systemd/system/xcash-daemon.service
    required: true
    readonly: false
    type: disk 
  xcash-services-rpc-wallet:
    source: ${XCASH_HOST_DATA_BASE_DIR}xcash-services/xcash-rpc-wallet.service
    path: /lib/systemd/system/xcash-rpc-wallet.service
    required: true
    readonly: false
    type: disk 
  xcash-services-dpops:
    source: ${XCASH_HOST_DATA_BASE_DIR}xcash-services/xcash-dpops.service
    path: /lib/systemd/system/xcash-dpops.service
    required: true
    readonly: false
    type: disk 
  # Blockchain data (blocks and peers)
  xcash-blockchain-data:
    source: ${XCASH_HOST_DATA_BASE_DIR}xcash-blockchain/
    path: /root/.X-CASH/
    required: true
    readonly: false
    type: disk  
  # MongoDB database data (DPOPS)
  xcash-dpops-dbdata:
    source: ${XCASH_HOST_DATA_BASE_DIR}xcash-dbdata/
    path: /data/db
    required: true
    readonly: false
    type: disk  
  # END bind-mount definition
  # START Port forwarding (proxy or nat-iptables)
  # Daemon Ports (P2P port and RPC port). 
  # P2P port needs incoming connections preserving client IP
  xcash-proxy-18280:
    listen: tcp:${VPS_EXTERNAL_IP}:18280
    connect: tcp:${LXC_XCASH_CONTAINER_IP}:18280
    type: proxy
    nat: true
  xcash-proxy-18281:
    listen: tcp:${VPS_EXTERNAL_IP}:18281
    connect: tcp:${LXC_XCASH_CONTAINER_IP}:18281
    type: proxy
    nat: true
  # DPOPS port (and shared delegate website dashboard) 
  xcash-proxy-18283:
    listen: tcp:0.0.0.0:18283
    connect: tcp:${LXC_XCASH_CONTAINER_IP}:18283
    type: proxy
  # END Port forwarding
EOF
)"


LXC_XCASH_PROFILE_SCRATCH_INIT="$(cat << EOF
description: Xcash-shared-node-profile-scratch-init
config: 
  # User data is used by cloud-init when available ("cloud" images)
  user.user-data: |
    #cloud-config
    #START of cloud-init configuration
    package_upgrade: true
    # Install the following packages (if not already installed)
    packages:
      - locales
      - tzdata
      - wget
      - curl
      - nano
      - moreutils
      - byobu
      - htop
      - sudo
      - lsb-release
      - dbus
      - systemd
      - systemd-sysv
      - cron
      - rsyslog
      - python2.7-minimal
      - iproute2
    # Set the locale
    locale: en_US.UTF-8
    # Set the timezone (eg. Europe/Rome or UTC)
    timezone: ${LXC_CONTAINER_TIMEZONE}
    # Creating container configuration script (run when created first time)
    write_files:
      # Basic configuration script
      - content: |
          #!/bin/bash
          # Clean apt-get temp files and cache
          apt-get clean
          rm -rf /var/lib/apt/lists/* /tmp/*
          
          # fix mesg error when sourcing .profile
          sed -i '/mesg n/d' /root/.profile
          
          # Disable imklog for rsyslog (kernel logging)
          sed -i 's/^\(module(load="imklog"\)/#\1/' /etc/rsyslog.conf
          # Change the output folder for syslog to var/log/syslog-container/
          sed -i "s/\/var\/log\/syslog/\/var\/log\/syslog-container\/syslog/g" /etc/rsyslog.d/50-default.conf
          # Disable auth logs
          sed -i "s/auth,authpriv.\*/#auth,authpriv.\*/g" /etc/rsyslog.d/50-default.conf
          
          # Remove unused things of systemd
          cd /lib/systemd/system/sysinit.target.wants/ \
          && ls | grep -v systemd-tmpfiles-setup | xargs rm -f $1
          
          rm -f /lib/systemd/system/multi-user.target.wants/* \
          /etc/systemd/system/*.wants/* \
          /lib/systemd/system/local-fs.target.wants/* \
          /lib/systemd/system/sockets.target.wants/*udev* \
          /lib/systemd/system/sockets.target.wants/*initctl* \
          /lib/systemd/system/basic.target.wants/* \
          /lib/systemd/system/anaconda.target.wants/* \
          /lib/systemd/system/plymouth* \
          /lib/systemd/system/systemd-update-utmp*
          
          find /etc/systemd/system \
          /lib/systemd/system \
          -path '*.wants/*' \
          -not -name '*journald*' \
          -not -name '*systemd-tmpfiles*' \
          -not -name '*systemd-user-sessions*' \
          -exec rm \{} \;
          
          # Don't start any optional services except for the few we need.
          systemctl set-default multi-user.target
          systemctl mask sys-fs-fuse-connections.mount
          
          # Enable needed systems at boot (default) and start them
          # hostname resolution (www.website.com)
          systemctl enable systemd-resolved
          systemctl start systemd-resolved
          ln -fs /run/systemd/resolve/resolv.conf /etc/resolv.conf
          systemctl restart systemd-resolved
          # Cron service
          systemctl enable cron
          systemctl start cron
          # Logging service
          systemctl stop rsyslog
          systemctl enable rsyslog
          systemctl start rsyslog
          
        path: /scripts/init_config.sh
        permissions: 0777
        
      # X-cash DPOPS installer configuration script for LXC setup
      - content: |
          #!/bin/bash
          # X-cash DPOPS installer configuration script for LXC setup
          # Set owner for bind mounted folder and files
          chown root:root /root/xcash-official/{logs,xcash-wallets}
          chown root:root /root/xcash-official/logs/xcash-node-syslogs
          chown root:root /root/.X-CASH
          chown root:root /data/db
          chown root:root /lib/systemd/system/{mongodb.service,xcash-daemon.service,xcash-dpops.service,xcash-rpc-wallet.service}
          
          # Move to working directory
          cd /root
          
          # Download LXC autoinstaller from N3me5is git
          wget ${XCASH_AUTOINSTALLER_SCRIPT_SCRATCH_URL} && chmod +x autoinstaller.sh
          # Download Maintenance script from N3me5is git
          wget ${XCASH_MAINTENANCE_SCRIPT_SCRATCH_URL} && chmod +x xcash-maintenance
          
          # Set EU seed instead of US seed (faster in europe). Comment if you are in the US
          sed -i "s/usseed1.x-cash.org/euseed1.x-cash.org/g" autoinstaller.sh
          
        path: /scripts/xcash_installer_config.sh
        permissions: 0777    
    # Run the following commands before starting the instance
    runcmd:
      # Run initial configuration script
      - [ bash, -c, "cd /scripts && ./init_config.sh" ]
      # Run x-cash installer connfigurator script (for LXC setup)
      - [ bash, -c, "cd /scripts && ./xcash_installer_config.sh" ]
    final_message: END of cloud-init configuration
  # END of cloud-init configuration
EOF
)"



function get_password()
{
  # ask for the root password if not root so it wont ask again when installing
  if [ "$EUID" -ne 0 ]; then
    sudo echo
  fi
}

function check_ubuntu_version()
{   
    command -v lsb_release > /dev/null 2>&1 ||
    {
        echo -e "${COLOR_PRINT_RED}FAIL${END_COLOR_PRINT}"
        echo 
        echo -e "${COLOR_PRINT_RED}############################################################${END_COLOR_PRINT}"
        echo -e "${COLOR_PRINT_RED}           !!! CANNOT CHECK YOUR UBUNTU VERSION !!!${END_COLOR_PRINT}"
        echo -e "${COLOR_PRINT_RED}############################################################${END_COLOR_PRINT}"
        echo
        exit
    }

    UBUNTU_VERSION=$(lsb_release -r | awk '{print $2}' | sed s"|\.||g")
    if [ "$UBUNTU_VERSION" -lt 1804 ]; then
      echo -e "${COLOR_PRINT_RED}FAIL${END_COLOR_PRINT}"
      echo 
      echo -e "${COLOR_PRINT_RED}############################################################${END_COLOR_PRINT}"
      echo -e "${COLOR_PRINT_RED}          !!!  YOUR UBUNTU VERSION IS NOT SUPPORTED !!!${END_COLOR_PRINT}"
      echo -e "${COLOR_PRINT_RED}############################################################${END_COLOR_PRINT}"
      echo
      exit
    fi
}


function configure_init_lxc()
{
    # Check if default pool exists in the system but not in lxc storage list 
    if sudo zpool list | grep -q "default "; then
      # Default pool present on system
      if ! sudo lxc storage list | grep -q "default "; then
        # Default pool not in LXD storage
        echo
        echo -e "${COLOR_PRINT_RED}WARNING: A default ZFS pool is already present in the system, but it's not present in the LXD database (probably you uninstalled LXD in the past without purging the pool). If you continue with the configuration the old default storage pool will be removed (with all the old images/containers inside it, if any). If you are looking for a 'disaster recovery' procedure please search on google.${END_COLOR_PRINT}"
        echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
        read -r data
        echo -ne "\r"
        echo
        sudo zpool destroy default
        sleep 1
      fi
    else
      # Default pool not present on system
      if sudo lxc storage list | grep -q "default "; then
        # Default pool present LXD storage
        echo
        echo -e "${COLOR_PRINT_RED}ERROR: there is a default ZFS pool in the LXD database, but there is no default pool in your system (verified with zpool tool). Something is wrong with your installation. You need to reboot your system and then purge LXD with 'sudo snap remove --purge lxd' and then retry the installation.${END_COLOR_PRINT}"
        echo
        exit
      fi
    fi
    # Initialize LXD config and create storage pool, network adapter, etc
    sudo lxd init --preseed <<< "$LXD_INIT_CONFIG"
    # Configure default editor for LXD
    if ! cat ~/.profile | grep -q "export EDITOR=nano"; then
      echo 'export EDITOR=nano' >> ~/.profile
    fi
    source ~/.profile
    # Enable lxc command execution from current user
    sudo usermod --append --groups lxd $USERNAME
    # Enable write permission to bind mounts
    SUBUID=$(printf "lxd:$(id -u):1\nroot:$(id -u):1\n")
    SUBGID=$(printf "lxd:$(id -g):1\nroot:$(id -g):1\n")
    if ! cat /etc/subuid | grep -q "${SUBUID}"; then
      echo "${SUBUID}" | sudo tee -a /etc/subuid
    fi
    if ! cat /etc/subgid | grep -q "${SUBGID}"; then
      echo "${SUBGID}" | sudo tee -a /etc/subgid
    fi
    sudo systemctl reload snap.lxd.daemon
    echo -e "${COLOR_PRINT_GREEN}Configuration of LXC completed${END_COLOR_PRINT}"
    echo
    echo -e "${COLOR_PRINT_RED}Please logout/login again or reboot the system!${END_COLOR_PRINT}"
}


function install_update_lxc()
{
  # Ask for password 
  get_password
  sleep 1
  # Install zfsutils (useful to check storage pool with 'sudo zpool list' and 'sudo zfs list')
  sudo apt install -y zfsutils-linux
  # Install byobu (a must have)
  sudo apt install -y byobu
  sleep 1
  echo
  echo -e "${COLOR_PRINT_GREEN}Checking if LXC (snap version) is installed${END_COLOR_PRINT}"
  lxc_which=$(which lxc)
  if [ "$lxc_which" == "/snap/bin/lxc" ]; then
    # Found snap version, force update it
    sudo snap refresh lxd
    echo
    echo -e "${COLOR_PRINT_GREEN}LXC snap version found and updated!${END_COLOR_PRINT}"
    echo -ne "${COLOR_PRINT_YELLOW}Do you want to configure LXD? Do this only if it's the first time you run the script and LXD was already installed. The configuration will change the LXC bridge IP and create the default storage pool file (/var/snap/lxd/common/lxd/disks/default.img) if does not exists. If you are running also other LXD containers please change the LXC_BRIDGE_DEV_NAME and LXC_XCASH_CONTAINER_IP variables in the lxc_autoinstaller.sh script accordingly to your setup. Continue with the default LXD configuration? (enter Y for YES, leave empty for default: NO): ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    if [ "$data" == "Y" ] || [  "$data" == "y" ]; then
      echo
      echo -ne "${COLOR_PRINT_GREEN}Installing and configuring LXC snap version${END_COLOR_PRINT}"
      echo
      configure_init_lxc
    fi
  elif [ "$lxc_which" == "" ]; then
    # Nothing installed
    echo
    echo -ne "${COLOR_PRINT_GREEN}Installing and configuring LXC snap version${END_COLOR_PRINT}"
    echo
    sudo snap install lxd
    configure_init_lxc
  else
    # Installed with ubuntu package, so remove and install with SNAP
    echo
    echo -ne "${COLOR_PRINT_GREEN}Installing and configuring LXC snap version${END_COLOR_PRINT}"
    echo
    sudo apt remove -y --purge lxd lxd-client
    sudo apt autoremove -y
    sudo snap install lxd
    configure_init_lxc
  fi
  echo
  echo -e "${COLOR_PRINT_GREEN}Use of 'byobu' (just installed) is STRONGLY SUGGESTED when connecting trough SSH to maintain the session active also in case of disconnection. Just type 'byobu' to create/reattach the session. If you want to automatically reconnect to the session after ssh login type 'byobu-enable'. You can manage multiple terminal tabs, quick guide: F2 for new tab, F3 and F4 move between tabs, CTRL+D close a tab (when nothin in execution), SHIFT+F12 enable/disable F keys, SHIFT+F6 detach the session (it remains in background).${END_COLOR_PRINT}"
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}       LXC Install/Update Has Completed Successfully  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  # Enable instant use of lxc command from the local user
  sudo su $USERNAME
}


function download_update_xcash_image()
{
  echo
  echo -e "${COLOR_PRINT_RED}WARNING: The old X-Cash node image (if already existent) will be deleted!${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_RED}WARNING: The current X-Cash node container (if exists) will be re-created!${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  cwd=$(pwd)
  # Ask the user if wants to use Opacity o direct download from url (wget)
  echo -ne "${COLOR_PRINT_YELLOW}Do you want to download the image using an Opacity Handle or with a direct link (using wget)? (leave empty for: Opacity Hanlde - Type N for: direct link): ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  if [ "$data" == "" ]; then
    DOWNLOAD_USE_OPACITY_DOWNLOADER="YES"
  else
    DOWNLOAD_USE_OPACITY_DOWNLOADER="NO"
    # Ask the url
    if [ ! "$DIRECT_DOWNLOAD_URL" == "" ]; then
      echo -ne "${COLOR_PRINT_YELLOW}Use the default download URL (${DIRECT_DOWNLOAD_URL})? Leave empty and press enter to YES or type N and press enter to privide a custom URL: ${END_COLOR_PRINT}"
      read -r data
      echo -ne "\r"
      echo
      if [ ! "$data" == "" ]; then
        echo -ne "${COLOR_PRINT_YELLOW}Enter the X-Cash Image direct download URL (must be a tar.gz file downloadable with wget): ${END_COLOR_PRINT}"
        read -r DIRECT_DOWNLOAD_URL
        echo -ne "\r"
        echo
      fi
    else
      echo -ne "${COLOR_PRINT_YELLOW}Enter the X-Cash Image direct download URL (must be a tar.gz file downloadable with wget): ${END_COLOR_PRINT}"
      read -r DIRECT_DOWNLOAD_URL
      echo -ne "\r"
      echo
    fi
  fi
  # Process the choice 
  if [ "$DOWNLOAD_USE_OPACITY_DOWNLOADER" == "YES" ]; then
    # Install Opacity CLI tool, remove if already existent (with already downloaded file inside)
    if [ -d opacity-cly-py ]; then
      rm -rf opacity-cly-py 2&> /dev/null || true
    fi
    git clone ${DOWNLOAD_TOOL_URL_OPACITY_CLI} && cd opacity-cly-py && chmod +x *.sh && ./install.sh
    echo
    # Ask for Update Handle
    echo -ne "${COLOR_PRINT_YELLOW}Enter the X-Cash Image Download Handle (Opacity): ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    echo
    # Download file
    download_string="download ${data} \".\""
    echo -e "${COLOR_PRINT_GREEN}Starting download with Opacity tool${END_COLOR_PRINT}"
    (echo; echo ${download_string}; echo "exit") | ./opacity-cli.sh    
  else
    rm -f *.tar.gz 2&> /dev/null || true
    echo -e "${COLOR_PRINT_GREEN}Starting download with wget${END_COLOR_PRINT}"
    wget ${DIRECT_DOWNLOAD_URL}
  fi
  source_file=$(ls "${XCASH_LXC_IMAGE_NAME}_"* 2> /dev/null)
  echo ${source_file}
  if [ "$source_file" == "" ]; then
    # File not exists, so an error occurred
    echo -e "${COLOR_PRINT_RED}File does not exists! Something went wrong during download! Try again! Cleaning and exiting.${END_COLOR_PRINT}"
    # Clean
    rm -f *.tar.gz 2&> /dev/null || true
    cd ${cwd}
    rm -f *.tar.gz 2&> /dev/null || true
    rm -rf opacity-cly-py 2&> /dev/null || true
    exit
  fi
  # Rename file
  if [ ! "$source_file" == "${XCASH_LXC_IMAGE_NAME}.tar.gz" ]; then
    mv "$source_file" "${XCASH_LXC_IMAGE_NAME}.tar.gz"
  fi
  # Remove old image file
  if lxc image list | grep -q "${XCASH_LXC_IMAGE_NAME} "; then
    lxc image delete ${XCASH_LXC_IMAGE_NAME}
  fi
  # Import new image
  echo -e "${COLOR_PRINT_GREEN}Importing new X-Cash node Image${END_COLOR_PRINT}"
  lxc image import "${XCASH_LXC_IMAGE_NAME}.tar.gz" --alias=${XCASH_LXC_IMAGE_NAME}
  # Clean downloaded file and tool
  echo -e "${COLOR_PRINT_GREEN}Removing temp files and download tool${END_COLOR_PRINT}"
  rm -f *.tar.gz 2&> /dev/null || true
  cd ${cwd}
  rm -f *.tar.gz 2&> /dev/null || true
  rm -rf opacity-cly-py 2&> /dev/null || true
  # Re-Create the container
  if lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
    # Container exists
    if lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Running"; then
      # Stop container
      echo -ne "${COLOR_PRINT_GREEN}There is an X-Cash Container already running. Stopping it.${END_COLOR_PRINT}"
      stop_xcash_container
      echo -ne "\r${COLOR_PRINT_GREEN}There is an X-Cash Container already running. Stopping it.${END_COLOR_PRINT}"
      echo
      echo -e "${COLOR_PRINT_GREEN}Re-creating the container using the new X-Cash base image...${END_COLOR_PRINT}"
      lxc init ${XCASH_LXC_IMAGE_NAME} ${XCASH_LXC_CONTAINER_NAME} -p ${XCASH_LXC_PROFILE_RUN_NAME}
      echo -ne "${COLOR_PRINT_GREEN}Container Created. Before it was in running state, so start it${END_COLOR_PRINT}"
      start_xcash_container
      echo -ne "\r${COLOR_PRINT_GREEN}Container Created. Before it was in running state, so start it${END_COLOR_PRINT}"
      echo
      echo -ne "\r${COLOR_PRINT_GREEN}X-Cash container started${END_COLOR_PRINT}"
      echo
    elif lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Stopped"; then
      echo -ne "\r${COLOR_PRINT_GREEN}The container already exists but it's stopped${END_COLOR_PRINT}"
      echo
      echo -e "${COLOR_PRINT_GREEN}Re-creating the container using the new X-Cash base image...${END_COLOR_PRINT}"
      lxc init ${XCASH_LXC_IMAGE_NAME} ${XCASH_LXC_CONTAINER_NAME} -p ${XCASH_LXC_PROFILE_RUN_NAME}
      echo -ne "${COLOR_PRINT_GREEN}Container Created. Container kept in stopping state, please start it or configure it${END_COLOR_PRINT}"
    fi
    echo
    echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
    echo -e "${COLOR_PRINT_GREEN}   Image and Container Update Has Completed Successfully  ${END_COLOR_PRINT}"
    echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
    echo
  else
    # Container not exists, tell the user to run configuration
    echo -e "${COLOR_PRINT_GREEN}X-Cash container not exists yet. Please run X-Cash First Configuration to create and configure it (if needed)${END_COLOR_PRINT}"
    echo
    echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
    echo -e "${COLOR_PRINT_GREEN}          Image Update Has Completed Successfully  ${END_COLOR_PRINT}"
    echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
    echo
  fi

}



function xcash_image_first_configuration()
{
  # Ask for password 
  get_password
  # Make sure time sync is enabled in host 
  sudo timedatectl set-ntp true
  sudo systemctl enable systemd-timesyncd
  sudo systemctl restart systemd-timesyncd
  # Create folders and files for Xcash. If directory structure and files already exists they won't be overwritten
  echo
  echo -e "${COLOR_PRINT_GREEN}Creating directories and files for bind mounts (if not exist)${END_COLOR_PRINT}"
  mkdir -p ${XCASH_HOST_DATA_BASE_DIR}{logs,xcash-wallets,xcash-services,xcash-blockchain,xcash-dbdata}
  mkdir -p ${XCASH_HOST_DATA_BASE_DIR}logs/{xcash-node-syslogs,xcash-daemon-and-wallet-logs}
  sudo touch ${XCASH_HOST_DATA_BASE_DIR}xcash-services/{mongodb.service,xcash-daemon.service,xcash-dpops.service,xcash-rpc-wallet.service}
  # Set chown for bind mounts
  echo -e "${COLOR_PRINT_GREEN}Setting permissions${END_COLOR_PRINT}"
  set_chown_bind_mounts
  # Create/Update profiles
  if lxc profile list | grep -q "${XCASH_LXC_PROFILE_RUN_NAME} "; then
    # Update profile
    echo -e "${COLOR_PRINT_GREEN}Updating X-cash node LXC profile${END_COLOR_PRINT}"
    lxc profile edit ${XCASH_LXC_PROFILE_RUN_NAME} <<< ${LXC_XCASH_PROFILE_RUN}
  else
    # Create and update profile
    echo -e "${COLOR_PRINT_GREEN}Creating and updating X-cash node LXC profile${END_COLOR_PRINT}"
    lxc profile create ${XCASH_LXC_PROFILE_RUN_NAME}
    lxc profile edit ${XCASH_LXC_PROFILE_RUN_NAME} <<< ${LXC_XCASH_PROFILE_RUN}
  fi
  # Check if container already exists and the status
  if lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
    # Container exists
    if lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Running"; then
      # Container running so launch first configuration
      echo -e "${COLOR_PRINT_GREEN}X-Cash node container already running, launching configurator${END_COLOR_PRINT}"
      launch_xcash_configurator
      # Ask if user wants to install the firewall script host side
      install_firewall_host
    elif lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Stopped"; then
      echo -ne "${COLOR_PRINT_YELLOW}Starting X-Cash node container and launching configurator${END_COLOR_PRINT}"
      start_xcash_container
      sleep 1
      echo -ne "\r${COLOR_PRINT_GREEN}Starting X-Cash node container and launching configurator${END_COLOR_PRINT}"
      echo
      launch_xcash_configurator
      # Ask if user wants to install the firewall script host side
      install_firewall_host
    fi
  else
    # Container not exists, check if image exists
    echo -e "${COLOR_PRINT_GREEN}X-Cash container not exists yet. Creating the container using the X-Cash base image...${END_COLOR_PRINT}"
    if lxc image list | grep -q "${XCASH_LXC_IMAGE_NAME} "; then
      # Image exists, so we can create the container from the x-cash base image and start it
      lxc launch ${XCASH_LXC_IMAGE_NAME} ${XCASH_LXC_CONTAINER_NAME} -p ${XCASH_LXC_PROFILE_RUN_NAME}
      echo -e "${COLOR_PRINT_GREEN}Container Created. Launching the configurator${END_COLOR_PRINT}"
      sleep 2
      launch_xcash_configurator
      # Ask if user wants to install the firewall script host side
      install_firewall_host
    else
      #Image does not exist
      echo
      echo -e "${COLOR_PRINT_RED}Error: Image not exists! Please download X-Cash image or create one from scratch. Exiting.${END_COLOR_PRINT}"
      exit
    fi
  fi
  echo
  # Ask the user if he wants to register the delegate and run the corresponding option of the autoinstaller
  echo
  echo -ne "${COLOR_PRINT_YELLOW}Do you want to register a new delegate or update existing delegate info? (leave empty for default: NO): ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  if [ ! "$data" == "" ]; then
    launch_xcash_register_delegate
  fi
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}   X-Cash Node Configuration Has Completed Successfully  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
}


function container_running_warning_continue()
{
  if lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
    # Container exists
    if lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Running"; then
      echo -e "${COLOR_PRINT_RED}The Container is running! If you continue it will be stopped. Are you sure you want to continue?${END_COLOR_PRINT}"
      echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
      read -r data
      echo -ne "\r"
      echo
      stop_xcash_container
    fi
  fi
}


function purge_xcash()
{
  echo
  echo -e "${COLOR_PRINT_RED}WARNING: Pay attention when removing things! Operation can not be reverted${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_RED}All operations require inserting Y for proceed. Leave empty to skip.${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  echo -ne "${COLOR_PRINT_YELLOW}Do you want to delete the archived X-Cash Image (NOT the container)? (enter Y for YES, leave empty for default: NO): ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  if [ "$data" == "Y" ] || [  "$data" == "y" ]; then
    echo
    if lxc image list | grep -q "${XCASH_LXC_IMAGE_NAME} "; then
      echo -e "${COLOR_PRINT_RED}The Image ${XCASH_LXC_IMAGE_NAME} will be deleted. The current container executing X-Cash Node (if exists) will be left untouched${END_COLOR_PRINT}"
      echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
      read -r data
      echo -ne "\r"
      echo
      lxc image delete ${XCASH_LXC_IMAGE_NAME}
      echo -e "${COLOR_PRINT_GREEN}Image deleted successfully!${END_COLOR_PRINT}"
    else
      #Image does not exist
      echo -e "${COLOR_PRINT_GREEN}Image does not exist! Already deleted or never downloaded${END_COLOR_PRINT}"
    fi
  fi
  echo
  echo -ne "${COLOR_PRINT_YELLOW}Do you want to remove X-Cash Container? (enter Y for YES, leave empty for default: NO): ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  if [ "$data" == "Y" ] || [  "$data" == "y" ]; then
    echo
    # Check if container already exists and the status
    if lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
      # Container exists
      echo -e "${COLOR_PRINT_RED}The Container ${XCASH_LXC_CONTAINER_NAME} will be deleted. Your personal data (wallet, service files, delegate key) and blockchain/DB data will be left untouched${END_COLOR_PRINT}"
      echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
      read -r data
      echo -ne "\r"
      echo
      if lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Running"; then
        echo -e "${COLOR_PRINT_RED}The Container is running! Are you sure you want to continue?${END_COLOR_PRINT}"
        echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
        read -r data
        echo -ne "\r"
        echo
        stop_xcash_container
      fi
      lxc delete -f ${XCASH_LXC_CONTAINER_NAME}
      echo -e "${COLOR_PRINT_GREEN}Container deleted successfully!${END_COLOR_PRINT}" 
    else
      echo -e "${COLOR_PRINT_GREEN}Container does not exist! Already deleted or never created${END_COLOR_PRINT}"
    fi
  fi
  echo
  echo -ne "${COLOR_PRINT_YELLOW}Do you want to completely remove all X-Cash node data? (enter Y for YES, leave empty for default: NO): ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  if [ "$data" == "Y" ] || [  "$data" == "y" ]; then
    echo
    echo -e "${COLOR_PRINT_RED}All the X-Cash Data stored on host bind-mounted folders will be deleted. This will delete also your wallet, the service files (containing passwords/keys), logs, DPOPS mongo database, blockchain blocks and so on. Make sure to have a backup if needed${END_COLOR_PRINT}"
    echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    echo
    container_running_warning_continue
    sudo rm -rf ${XCASH_HOST_DATA_BASE_DIR}
    echo -e "${COLOR_PRINT_GREEN}Host Data deleted successfully!${END_COLOR_PRINT}" 
  else
    # Give the user the possibility to delete only some things
    echo
    echo -ne "${COLOR_PRINT_YELLOW}Do you want to remove only P2P peers data (X-CASH_p2pstate.bin file)? (enter Y for YES, leave empty for default: NO): ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    if [ "$data" == "Y" ] || [  "$data" == "y" ]; then
      echo
      container_running_warning_continue
      sudo rm -f ${XCASH_HOST_DATA_BASE_DIR}xcash-blockchain/X-CASH_p2pstate.bin
      echo -e "${COLOR_PRINT_GREEN}P2P peers data deleted successfully!${END_COLOR_PRINT}" 
    fi
    echo
    echo -ne "${COLOR_PRINT_YELLOW}Do you want to remove only Blockchain Data (lmdb folder)? (enter Y for YES, leave empty for default: NO): ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    if [ "$data" == "Y" ] || [  "$data" == "y" ]; then
      echo
      container_running_warning_continue
      sudo rm -rf ${XCASH_HOST_DATA_BASE_DIR}xcash-blockchain/lmdb/
      echo -e "${COLOR_PRINT_GREEN}Blockchain data deleted successfully!${END_COLOR_PRINT}" 
    fi
    echo
    echo -ne "${COLOR_PRINT_YELLOW}Do you want to remove only Wallets? (enter Y for YES, leave empty for default: NO): ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    if [ "$data" == "Y" ] || [  "$data" == "y" ]; then
      echo
      echo -e "${COLOR_PRINT_RED}Make sure to have a backup of your wallets information (seeds, passwords, ...)${END_COLOR_PRINT}"
      echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
      read -r data
      echo -ne "\r"
      echo
      container_running_warning_continue
      sudo rm -f ${XCASH_HOST_DATA_BASE_DIR}xcash-wallets/*
      echo -e "${COLOR_PRINT_GREEN}Wallets deleted successfully!${END_COLOR_PRINT}" 
    fi
    echo
    echo -ne "${COLOR_PRINT_YELLOW}Do you want to remove only the service files (they contain wallet password and validator private key)? (enter Y for YES, leave empty for default: NO): ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    if [ "$data" == "Y" ] || [  "$data" == "y" ]; then
      echo
      echo -e "${COLOR_PRINT_RED}Make sure to have a backup of your wallet password and validator private key${END_COLOR_PRINT}"
      echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
      read -r data
      echo -ne "\r"
      echo
      container_running_warning_continue
      sudo rm -f ${XCASH_HOST_DATA_BASE_DIR}xcash-services/*
      echo -e "${COLOR_PRINT_GREEN}Service files deleted successfully!${END_COLOR_PRINT}" 
    fi
  fi
  echo
  echo
  echo -ne "${COLOR_PRINT_YELLOW}Do you want to uninstall and purge LXD? (enter Y for YES, leave empty for default: NO): ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  if [ "$data" == "Y" ] || [  "$data" == "y" ]; then
    if lxc image list | grep -q "${XCASH_LXC_IMAGE_NAME} " || lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
      # Container and/or image exist
      echo
      echo -e "${COLOR_PRINT_RED}Error: you can not uninstall LXD if you still have the X-Cash Container and/or the X-Cash Image in your system. Please purge Image and Container first. Skipping...${END_COLOR_PRINT}"
    else
      echo
      echo -e "${COLOR_PRINT_RED}LXD (snap version) will be removed (purged). The LXD default volume (/var/snap/lxd/common/lxd/disks/default.img) WILL BE REMOVED TOO, with all the images/containers/configurations inside (not only X-Cash stuff). If you don't want to do this, please uninstall LXD manually with all its consequences...${END_COLOR_PRINT}"
      echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
      read -r data
      echo -ne "\r"
      echo
      sudo snap remove --purge lxd
      # Make sure zpool tool is installed
      sudo apt install -y zfsutils-linux
      sudo zpool destroy default
      echo -e "${COLOR_PRINT_GREEN}LXD snap package uninstalled successfully! Default storage pool destroyed!${END_COLOR_PRINT}" 
    fi
  fi
  echo
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}       X-Cash Node Purge Has Completed Successfully  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
}



function install_firewall_host()
{
  # Ask for password 
  get_password
  # Ask which firewall 
  while
    echo -ne "${COLOR_PRINT_YELLOW}HOST FIREWALL: Do you want to install Firewall [1] or Shared Delegates Firewall [2] on the host? Default exit without installing the firewall [0]. Enter your choice: ${END_COLOR_PRINT}"
    read -r firewall_choice
    if [ "${firewall_choice}" == "" ]; then
      firewall_choice="0"
    fi
    echo -ne "\r"
    echo
    [ ! ${firewall_choice^^} == "1" ] && [ ! ${firewall_choice^^} == "2" ] && [ ! ${firewall_choice^^} == "0" ]
  do true; done
  if [ ${firewall_choice^^} == "0" ]; then
    # Do nothing
    sleep 0.2
  elif [ ${firewall_choice^^} == "1" ]; then
    # Install Solo Firewall with autoinstaller
    wget ${XCASH_AUTOINSTALLER_HOST_FIREWALL_URL} && chmod +x ${XCASH_AUTOINSTALLER_NAME}
    sleep 0.1
    (echo "${XCASH_AUTOINSTALLER_FIREWALL_OPTION_NUMBER}"; cat) | ./${XCASH_AUTOINSTALLER_NAME}
    rm ${XCASH_AUTOINSTALLER_NAME}
    sudo systemctl reload snap.lxd.daemon
    echo
  elif [ ${firewall_choice^^} == "2" ]; then
    # Install Delegate Firewall with autoinstaller
    wget ${XCASH_AUTOINSTALLER_HOST_FIREWALL_URL} && chmod +x ${XCASH_AUTOINSTALLER_NAME}
    sleep 0.1
    (echo "${XCASH_AUTOINSTALLER_FIREWALL_SHARED_OPTION_NUMBER}"; cat) | ./${XCASH_AUTOINSTALLER_NAME}
    rm ${XCASH_AUTOINSTALLER_NAME}
    sudo systemctl reload snap.lxd.daemon
    echo
  fi
}



function xcash_container_create_scratch()
{
  # Ask for password 
  get_password
  # Make sure time sync is enabled in host 
  sudo timedatectl set-ntp true
  sudo systemctl enable systemd-timesyncd
  sudo systemctl restart systemd-timesyncd
  # Check if container already exists and the status
  if lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
    # Container exists, ask the user if replace it
    echo
    echo -e "${COLOR_PRINT_RED}WARNING: X-Cash container already exists. If you continue the current container${END_COLOR_PRINT}"
    echo -e "${COLOR_PRINT_RED}will be stopped and deleted. Are you sure you want to continue?${END_COLOR_PRINT}"
    echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    echo
    if lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Running"; then
      # Container running so stop it first
      stop_xcash_container
    fi
    sleep 2
    lxc delete -f ${XCASH_LXC_CONTAINER_NAME}
  fi
  echo
  echo -e "${COLOR_PRINT_RED}WARNING: Please remove Host data directory (if already exists) ${XCASH_HOST_DATA_BASE_DIR} ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_RED}before continue (make backup if needed), so a clean installation can be made.${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_RED}You can do it with the Purge Option or simply with  sudo rm -R -f ${XCASH_HOST_DATA_BASE_DIR}${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  # Create folders and files for Xcash. If directory structure and files already exists they won't be overwritten
  echo
  echo -e "${COLOR_PRINT_GREEN}Creating directories and files for bind mounts (if not exist)${END_COLOR_PRINT}"
  mkdir -p ${XCASH_HOST_DATA_BASE_DIR}{logs,xcash-wallets,xcash-services,xcash-blockchain,xcash-dbdata}
  mkdir -p ${XCASH_HOST_DATA_BASE_DIR}logs/{xcash-node-syslogs,xcash-daemon-and-wallet-logs}
  sudo touch ${XCASH_HOST_DATA_BASE_DIR}xcash-services/{mongodb.service,xcash-daemon.service,xcash-dpops.service,xcash-rpc-wallet.service}
  # Set chown for bind mounts
  echo -e "${COLOR_PRINT_GREEN}Setting permissions${END_COLOR_PRINT}"
  set_chown_bind_mounts
  # Create/Update profiles
  if lxc profile list | grep -q "${XCASH_LXC_PROFILE_RUN_NAME} "; then
    # Update profile
    echo -e "${COLOR_PRINT_GREEN}Updating X-cash node LXC profile${END_COLOR_PRINT}"
    lxc profile edit ${XCASH_LXC_PROFILE_RUN_NAME} <<< ${LXC_XCASH_PROFILE_RUN}
  else
    # Create and update profile
    echo -e "${COLOR_PRINT_GREEN}Creating and updating X-cash node LXC profile${END_COLOR_PRINT}"
    lxc profile create ${XCASH_LXC_PROFILE_RUN_NAME}
    lxc profile edit ${XCASH_LXC_PROFILE_RUN_NAME} <<< ${LXC_XCASH_PROFILE_RUN}
  fi
  if lxc profile list | grep -q "${XCASH_LXC_PROFILE_SCRATCH_INIT_NAME} "; then
    # Update profile
    echo -e "${COLOR_PRINT_GREEN}Updating X-cash node scratch init LXC profile${END_COLOR_PRINT}"
    lxc profile edit ${XCASH_LXC_PROFILE_SCRATCH_INIT_NAME} <<< ${LXC_XCASH_PROFILE_SCRATCH_INIT}
  else
    # Create and update profile
    echo -e "${COLOR_PRINT_GREEN}Creating and updating X-cash node scratch init LXC profile${END_COLOR_PRINT}"
    lxc profile create ${XCASH_LXC_PROFILE_SCRATCH_INIT_NAME}
    lxc profile edit ${XCASH_LXC_PROFILE_SCRATCH_INIT_NAME} <<< ${LXC_XCASH_PROFILE_SCRATCH_INIT}
  fi
  # Create the new container using linux 18.04 as base image and creating the container with cloud-init
  echo -e "${COLOR_PRINT_GREEN}Create the X-Cash node empty container${END_COLOR_PRINT}"
  lxc launch ubuntu:18.04 ${XCASH_LXC_CONTAINER_NAME} -p ${XCASH_LXC_PROFILE_SCRATCH_INIT_NAME} -p ${XCASH_LXC_PROFILE_RUN_NAME}
  sleep 1
  # Wait cloud-init initialization 
  echo -ne "${COLOR_PRINT_YELLOW}Waiting cloud-init completion${END_COLOR_PRINT}"
  while 
    cloud_init_outcome=$(lxc exec ${XCASH_LXC_CONTAINER_NAME} -- cloud-init status)
    if [ "$cloud_init_outcome" == "Error: not found" ]; then
      echo
      echo -e "${COLOR_PRINT_RED}An error during container initialization has occurred. Try to generate the container again! Exiting.${END_COLOR_PRINT}"
      exit
    fi
    if [ "$cloud_init_outcome" == "status: error" ]; then
      echo
      echo -e "${COLOR_PRINT_RED}An error during container initialization has occurred. Check your internet connection/firewall and try to generate the container again! Exiting.${END_COLOR_PRINT}"
      exit
    fi
    [[ ! "${cloud_init_outcome}" == "status: done" ]]
  do 
    sleep 5
  done
  echo -e "\r${COLOR_PRINT_GREEN}Waiting cloud-init completion${END_COLOR_PRINT}"
  # Run the Autoinstaller with the install option (1)
  launch_xcash_install_autoinstaller
  sleep 1
  # Ask if user wants to install the firewall script host side
  install_firewall_host
  # Ask the user if he wants to register the delegate and run the corresponding option of the autoinstaller
  echo
  echo -ne "${COLOR_PRINT_YELLOW}Do you want to register a new delegate or update existing delegate info? (leave empty for default: NO): ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  if [ ! "$data" == "" ]; then
    launch_xcash_register_delegate
  fi
  echo
  echo -e "${COLOR_PRINT_GREEN}##############################################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}   X-Cash Node Container Creation (from Scratch) Has Completed Successfully  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}##############################################################################${END_COLOR_PRINT}"
  echo  
}


function create_image_from_container()
{
  echo
  echo -e "${COLOR_PRINT_RED}WARNING: The old image (if already existent) will be deleted.${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_RED}The current container will be left untouched. Are you sure you want to continue?${END_COLOR_PRINT}"
  echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
  read -r data
  echo -ne "\r"
  echo
  #Container must be started first
  start_xcash_container
  # Clean temp files and history inside container
  lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "apt-get clean"
  lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "rm -rf /var/lib/apt/lists/* /tmp/* 2&> /dev/null || true"
  lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "rm -f /var/log/syslog* 2&> /dev/null || true"
  lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "truncate --size 0 /var/log/syslog"
  lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "rm -f .bash_history"
  lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "history -c"
  # Create temporary snapshot
  lxc snapshot ${XCASH_LXC_CONTAINER_NAME} image_creation_snapshot
  # Remove Old image if exists
  if lxc image list | grep -q "${XCASH_LXC_IMAGE_NAME} "; then
    lxc image delete ${XCASH_LXC_IMAGE_NAME}
  fi
  # Create the image
  lxc publish ${XCASH_LXC_CONTAINER_NAME}/image_creation_snapshot --alias=${XCASH_LXC_IMAGE_NAME}
  # Remove the temporary snapshot
  lxc delete ${XCASH_LXC_CONTAINER_NAME}/image_creation_snapshot
}


function export_image_tar()
{
  if lxc image list | grep -q "${XCASH_LXC_IMAGE_NAME} "; then
    date=$(date +'%Y%m%d')
    echo
    echo -e "${COLOR_PRINT_GREEN}X-Cash image (not container) will be exported in current folder${END_COLOR_PRINT}"
    echo -e "${COLOR_PRINT_RED}WARNING: If ${XCASH_LXC_IMAGE_NAME}_${date}.tar.gz already exists it will be replaced!${END_COLOR_PRINT}"
    echo -ne "${COLOR_PRINT_YELLOW}Press ENTER to continue or press Ctrl + C to cancel! ${END_COLOR_PRINT}"
    read -r data
    echo -ne "\r"
    echo
    rm -f "${XCASH_LXC_IMAGE_NAME}_${date}.tar.gz"
    lxc image export ${XCASH_LXC_IMAGE_NAME} "${XCASH_LXC_IMAGE_NAME}_${date}"
  else
    #Image does not exist
    echo
    echo -e "${COLOR_PRINT_RED}Error: Image not exists! Please download X-Cash image or create one from the current container (if exists). Exiting.${END_COLOR_PRINT}"
    exit  
  fi
}


function set_chown_bind_mounts()
{
  if [ "$1" == "print_status" ]; then
    echo
    echo -e "${COLOR_PRINT_GREEN}Setting Chown for the host bind-mounted folders${END_COLOR_PRINT}"
  fi
  sudo chown -R 1000000:1000000 ${XCASH_HOST_DATA_BASE_DIR}{logs,xcash-wallets,xcash-services,xcash-blockchain,xcash-dbdata}
  # Must be correct owner for syslog folders and syslog, otherwise syslog will be lost after reboot
  sudo chown 1000000:1000106 ${XCASH_HOST_DATA_BASE_DIR}logs/xcash-node-syslogs
  sudo chown 1000102:1000004 ${XCASH_HOST_DATA_BASE_DIR}logs/xcash-node-syslogs/syslog* 2&> /dev/null || true
}


function start_xcash_container()
{
  # Check if container exists and already started. Start it only if exists and is stopped
  if lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
    # Container exists
    if lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Stopped"; then
      if [ "$1" == "print_status" ]; then
        echo
        echo -ne "${COLOR_PRINT_YELLOW}Starting the X-Cash node container${END_COLOR_PRINT}"
      fi
      lxc start ${XCASH_LXC_CONTAINER_NAME}
      sleep 1
      if [ "$1" == "print_status" ]; then
        echo -ne "\r${COLOR_PRINT_GREEN}Starting the X-Cash node container${END_COLOR_PRINT}"
        echo
        echo -e "${COLOR_PRINT_GREEN}Start command executed! Check the following LXC status${END_COLOR_PRINT}"
      fi
    elif lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Running"; then
      if [ "$1" == "print_status" ]; then
        echo
        echo -e "${COLOR_PRINT_GREEN}X-Cash Node container already running! Check the following LXC status${END_COLOR_PRINT}"
      fi
    fi
  else
    # Container not exists
    if [ "$1" == "print_status" ]; then
      echo
      echo -e "${COLOR_PRINT_RED}X-Cash Node container NOT exists! Check the following LXC status${END_COLOR_PRINT}"
    fi
  fi
  if [ "$1" == "print_status" ]; then
    echo
    lxc list
  fi
}

function stop_xcash_container()
{
  # Check if container exists and already stopped. Stop it only if exists and is running
  if lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
    # Container exists
    if lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Running"; then
      if [ "$1" == "print_status" ]; then
        echo
        echo -ne "${COLOR_PRINT_YELLOW}Stopping the X-Cash node container${END_COLOR_PRINT}"
      fi
      lxc stop ${XCASH_LXC_CONTAINER_NAME}
      sleep 1
      if [ "$1" == "print_status" ]; then
        echo -ne "${COLOR_PRINT_GREEN}Stopping the X-Cash node container${END_COLOR_PRINT}"
        echo
        echo -e "${COLOR_PRINT_GREEN}Stop command executed! Check the following LXC status${END_COLOR_PRINT}"
      fi
    elif lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Stopped"; then
      if [ "$1" == "print_status" ]; then
        echo
        echo -e "${COLOR_PRINT_GREEN}X-Cash Node container already stopped! Check the following LXC status${END_COLOR_PRINT}"
      fi
    fi
  else
    # Container not exists
    if [ "$1" == "print_status" ]; then
      echo
      echo -e "${COLOR_PRINT_RED}X-Cash Node container NOT exists! Check the following LXC status${END_COLOR_PRINT}"
    fi
  fi
  if [ "$1" == "print_status" ]; then
    echo
    lxc list
  fi
}


function update_container_autoinstaller_script()
{
  lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "rm -f ${XCASH_AUTOINSTALLER_NAME} 2&> /dev/null && wget ${XCASH_AUTOINSTALLER_SCRIPT_SCRATCH_URL} && chmod +x ${XCASH_AUTOINSTALLER_NAME}" 2&> /dev/null
}


function update_container_maintenance_script()
{
  lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "rm -f ${XCASH_MAINTENANCE_SCRIPT_NAME} 2&> /dev/null && wget ${XCASH_MAINTENANCE_SCRIPT_SCRATCH_URL} && chmod +x ${XCASH_MAINTENANCE_SCRIPT_NAME}" 2&> /dev/null  
}


function launch_xcash_configurator()
{
  update_container_autoinstaller_script
  if lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "ls -l" | grep -q "${XCASH_AUTOINSTALLER_NAME}"; then
    lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "(echo '${XCASH_AUTOINSTALLER_CONFIGURATOR_OPTION_NUMBER}'; cat) | ./${XCASH_AUTOINSTALLER_NAME}" 
  else
    # Autoinstaller script not exists. This should never occur
    echo
    echo -e "${COLOR_PRINT_RED}Error: Autoinstaller script does not exists inside the container! Exiting.${END_COLOR_PRINT}"
    exit
  fi
}

function launch_xcash_install_autoinstaller()
{
  update_container_autoinstaller_script
  if lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "ls -l" | grep -q "${XCASH_AUTOINSTALLER_NAME}"; then
    lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "(echo '${XCASH_AUTOINSTALLER_INSTALL_OPTION_NUMBER}'; cat) | ./${XCASH_AUTOINSTALLER_NAME}" 
  else
    # Autoinstaller script not exists. This should never occur
    echo
    echo -e "${COLOR_PRINT_RED}Error: Autoinstaller script does not exists inside the container! Exiting.${END_COLOR_PRINT}"
    exit
  fi
}

function launch_xcash_register_delegate()
{
  update_container_autoinstaller_script
  if lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "ls -l" | grep -q "${XCASH_AUTOINSTALLER_NAME}"; then
    lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "(echo '${XCASH_AUTOINSTALLER_REGISTER_DELEGATE_OPTION_NUMBER}'; cat) | ./${XCASH_AUTOINSTALLER_NAME}" 
  else
    # Autoinstaller script not exists. This should never occur
    echo
    echo -e "${COLOR_PRINT_RED}Error: Autoinstaller script does not exists inside the container! Exiting.${END_COLOR_PRINT}"
    exit
  fi
}


function enter_xcash_container_shell()
{
  # Check if container exists and already started
  if lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
    # Container exists
    if lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Stopped"; then
      echo
      echo -e "${COLOR_PRINT_RED}Container is not Running. Please start it before using this option!${END_COLOR_PRINT}"
      exit
    elif lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Running"; then
      echo
      echo -e "${COLOR_PRINT_GREEN}Entering the container shell...${END_COLOR_PRINT}"
      echo
      lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l 
    fi
  else
    # Container not exists
    echo
    echo -e "${COLOR_PRINT_RED}X-Cash Node container NOT exists! Probably you still have to run the X-Cash First Configuration...${END_COLOR_PRINT}"
    exit
  fi
}


function run_xcash_container_autoinstaller()
{
  # Check if container exists and already started
  if lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
    # Container exists
    if lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Stopped"; then
      echo
      echo -e "${COLOR_PRINT_RED}Container is not Running. Please start it before using this option!${END_COLOR_PRINT}"
      exit
    elif lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Running"; then
      # Use last version of the autoinstaller
      update_container_autoinstaller_script
      # Check if autoinstaller script is present
      if lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "ls -l" | grep -q "${XCASH_AUTOINSTALLER_NAME}"; then
        echo
        echo -e "${COLOR_PRINT_GREEN}Running the X-Cash Autoinstaller script (inside the container)...${END_COLOR_PRINT}"
        echo
        lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "./${XCASH_AUTOINSTALLER_NAME}"  
      else
        # Autoinstaller script not exists. This should never occur
        echo
        echo -e "${COLOR_PRINT_RED}Error: Autoinstaller script does not exists inside the container! Exiting.${END_COLOR_PRINT}"
        exit
      fi
    fi
  else
    # Container not exists
    echo
    echo -e "${COLOR_PRINT_RED}X-Cash Node container NOT exists! Probably you still have to run the X-Cash First Configuration...${END_COLOR_PRINT}"
    exit
  fi
}


function run_xcash_container_maintenance_script()
{
  # Check if container exists and already started
  if lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
    # Container exists
    if lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Stopped"; then
      echo
      echo -e "${COLOR_PRINT_RED}Container is not Running. Please start it before using this option!${END_COLOR_PRINT}"
      exit
    elif lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Running"; then
      # Use the last version of the maintenance script (or update it), but not when running the --overvirew
      # So if you need to open the maintenance script to update it
      if [ ! "$1" == "overview" ]; then
        update_container_maintenance_script
      fi
      # Check if maintenance script is present
      if lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "ls -l" | grep -q "${XCASH_MAINTENANCE_SCRIPT_NAME}"; then
        echo
        if [ "$1" == "overview" ]; then
          echo -e "${COLOR_PRINT_GREEN}Showing X-Cash Node Status Overview${END_COLOR_PRINT}"
          echo
          lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "./${XCASH_MAINTENANCE_SCRIPT_NAME} --overview"  
        else
          echo -e "${COLOR_PRINT_GREEN}Running the X-Cash Maintenance script (inside the container)...${END_COLOR_PRINT}"
          echo
          lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "./${XCASH_MAINTENANCE_SCRIPT_NAME}" 
        fi         
      else
        # Maintenance script not exists. This should never occur
        echo
        echo -e "${COLOR_PRINT_RED}Error: Maintenance script does not exists inside the container! Exiting.${END_COLOR_PRINT}"
        exit
      fi
    fi
  else
    # Container not exists
    echo
    echo -e "${COLOR_PRINT_RED}X-Cash Node container NOT exists! Probably you still have to run the X-Cash First Configuration...${END_COLOR_PRINT}"
    exit
  fi
}


function run_DPOPS_log_monitor_realtime()
{
  # Check if container exists and already started
  if lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
    # Container exists
    if lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Stopped"; then
      echo
      echo -e "${COLOR_PRINT_RED}Container is not Running. Please start it before using this option!${END_COLOR_PRINT}"
      exit
    elif lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Running"; then
      # Run the xcash shared node log monitor realtime
      lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "journalctl --unit=${XCASH_DPOPS_SRV} --follow -n 100 --output cat"
      exit     
    fi
  else
    # Container not exists
    echo
    echo -e "${COLOR_PRINT_RED}X-Cash Node container NOT exists! Probably you still have to run the X-Cash First Configuration...${END_COLOR_PRINT}"
    exit
  fi
}


function run_DPOPS_log_check_grep()
{
  # Check if container exists and already started
  if lxc list | grep -q "${XCASH_LXC_CONTAINER_NAME} "; then
    # Container exists
    if lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Stopped"; then
      echo
      echo -e "${COLOR_PRINT_RED}Container is not Running. Please start it before using this option!${END_COLOR_PRINT}"
      exit
    elif lxc info ${XCASH_LXC_CONTAINER_NAME} | grep -q "Status: Running"; then
      # Ask for a word
      echo -ne "${COLOR_PRINT_YELLOW}Enter a search string (for example your delegate name): ${END_COLOR_PRINT}"
      read -r grep_string
      echo -ne "\r"
      echo
      lxc exec ${XCASH_LXC_CONTAINER_NAME} -- bash -l -c "journalctl --unit=${XCASH_DPOPS_SRV} --output cat | grep -B 10 -A 15 -C 1 --color=always '${grep_string}' | less -R +G"
      exit     
    fi
  else
    # Container not exists
    echo
    echo -e "${COLOR_PRINT_RED}X-Cash Node container NOT exists! Probably you still have to run the X-Cash First Configuration...${END_COLOR_PRINT}"
    exit
  fi
}





MAIN_MENU_OPTIONS="$(cat << EOF
1  = Install and configure LXC (snap)
2  = Download Update X-cash Image
3  = Run X-Cash First configuration
4  = Install Firewall Script on Host
5  = Start X-Cash Container
6  = Stop X-Cash Container
7  = Enter X-Cash Container Shell
8  = Run X-Cash Maintenance Script (inside the container)
9  = Print X-Cash Maintenance Overview (status)
10 = Run X-Cash Autoinstaller Script (inside the container)
11 = Run DPOPS Logs Monitor (Realtime)
12 = Check DPOPS logs filtering with a given word (using grep)
13 = Set chown of bind mounts (use after wallet/services manual copy)
14 = Create X-Cash container from scratch using autoinstaller
15 = (Re)Create X-Cash base image from current container
16 = Export current X-Cash base image as tar.gz file
17 = Purge X-Cash (Select what to remove)

Enter the number of your choice: 
EOF
)"

function main_menu_selection()
{
  echo
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}         Welcome to X-Cash LXC install script  ${END_COLOR_PRINT}"
  echo -e "${COLOR_PRINT_GREEN}############################################################${END_COLOR_PRINT}"
  echo
  echo -e "${COLOR_PRINT_YELLOW}Please select what to do${END_COLOR_PRINT}"
  echo
  echo -ne "${COLOR_PRINT_YELLOW}${MAIN_MENU_OPTIONS}${END_COLOR_PRINT}"
  read -r XCASH_LXC_AUTOINSTALLER_MENU_OPTION
  echo -ne "\r"
  if [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "1" ]; then
    install_update_lxc
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "2" ]; then
    download_update_xcash_image
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "3" ]; then
    xcash_image_first_configuration
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "4" ]; then
    install_firewall_host
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "5" ]; then
    start_xcash_container "print_status"
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "6" ]; then
    stop_xcash_container "print_status"
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "7" ]; then
    enter_xcash_container_shell
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "8" ]; then
    run_xcash_container_maintenance_script
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "9" ]; then
    run_xcash_container_maintenance_script "overview"
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "10" ]; then
    run_xcash_container_autoinstaller
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "11" ]; then
    run_DPOPS_log_monitor_realtime
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "12" ]; then
    run_DPOPS_log_check_grep
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "13" ]; then
    set_chown_bind_mounts
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "14" ]; then
    xcash_container_create_scratch
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "15" ]; then
    create_image_from_container
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "16" ]; then
    export_image_tar
  elif [ "$XCASH_LXC_AUTOINSTALLER_MENU_OPTION" -eq "17" ]; then
    purge_xcash
  else
    # Option not available / invalid
    echo
    echo -e "${COLOR_PRINT_RED}Invalid option! Please run the script again and enter a valid menu number.${END_COLOR_PRINT}"
    echo
    exit
  fi
}


### RUN THE SCRIPT

# Disable script execution with sudo and warns the user if root install
if [ $SUDO_USER ] ; then
  # execution with sudo not allowed
  echo -e "\n${COLOR_PRINT_RED}Please don't use sudo with this script!${END_COLOR_PRINT}"
  exit 1
else
  # If not inside a LXC container (isolated, with official LXC installer) 
  if [ ! "$container" == "lxc" ]; then
    if [ $UID -eq 0 ] ; then
      echo -e "\n${COLOR_PRINT_RED}WARNING: You are running the script with a root user! This is NOT secure and NOT suggested. Please use a dedicated xcash user following the documentation. If you want to proceed with root and you know what you are doing press ENTER, otherwise press CTRL+C to exit.${END_COLOR_PRINT}"
      read -r data
      echo -ne "\r"
      echo
      if [ ! "${data}" == "" ]; then
        exit 1
      fi
    fi
  fi
fi

# Check for a compatible OS
check_ubuntu_version

# Enter the main menu
main_menu_selection









