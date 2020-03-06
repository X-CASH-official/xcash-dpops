#!/bin/bash

rm -r /var/www/html/build/
mkdir /var/www/html/build/

lxc start ubuntu-1804
lxc exec ubuntu-1804 -- bash -c "source ~/.profile ; $(curl -sSL https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/build/build_script.sh)"
lxc file pull ubuntu-1804/root/x-network/build/ /var/www/html/build/ubuntu-1804
lxc stop ubuntu-1804

lxc start ubuntu-1910
lxc exec ubuntu-1910 -- bash -c "source ~/.profile ; $(curl -sSL https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/build/build_script.sh)"
lxc file pull ubuntu-1910/root/x-network/build/ /var/www/html/build/ubuntu-1910
lxc stop ubuntu-1910

lxc start ubuntu-1804-static-x-cash-build
lxc exec ubuntu-1804-static-x-cash-build -- bash -c "source ~/.profile ; $(curl -sSL https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/build/build_script_static.sh)"
lxc file pull ubuntu-1804-static-x-cash-build/root/x-network/build/STEP_10_XCASH_BUILD_STATIC.txt /var/www/html/build/ubuntu-1804/STEP_10_XCASH_BUILD_STATIC.txt
lxc stop ubuntu-1804-static-x-cash-build

lxc start ubuntu-1910-static-x-cash-build
lxc exec ubuntu-1910-static-x-cash-build -- bash -c "source ~/.profile ; $(curl -sSL https://raw.githubusercontent.com/X-CASH-official/xcash-dpops/master/scripts/build/build_script_static.sh)"
lxc file pull ubuntu-1910-static-x-cash-build/root/x-network/build/STEP_10_XCASH_BUILD_STATIC.txt /var/www/html/build/ubuntu-1910/STEP_10_XCASH_BUILD_STATIC.txt
lxc stop ubuntu-1910-static-x-cash-build
