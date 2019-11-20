#!/bin/bash

rm -r /var/www/html/build/
mkdir /var/www/html/build/

docker start ubuntu-18.04
docker exec -e USER="root" -u root -t -i --privileged ubuntu-18.04 /bin/bash -c "source ~/.profile; $(curl -sSL https://raw.githubusercontent.com/X-CASH-official/XCASH_DPOPS/master/scripts/build/build_script.sh)"
docker cp ubuntu-18.04:/root/x-network/build/ /var/www/html/build/ubuntu-18.04
docker stop ubuntu-18.04

docker start ubuntu-19.10
docker exec -e USER="root" -u root -t -i --privileged ubuntu-19.10 /bin/bash -c "source ~/.profile; $(curl -sSL https://raw.githubusercontent.com/X-CASH-official/XCASH_DPOPS/master/scripts/build/build_script.sh)"
docker cp ubuntu-19.10:/root/x-network/build/ /var/www/html/build/ubuntu-19.10
docker stop ubuntu-19.10

docker start ubuntu-18.04-static-x-cash-build
docker exec -e USER="root" -u root -t -i --privileged ubuntu-18.04-static-x-cash-build /bin/bash -c "$(curl -sSL https://raw.githubusercontent.com/X-CASH-official/XCASH_DPOPS/master/scripts/build/build_script_static.sh)"
docker cp ubuntu-18.04-static-x-cash-build:/root/x-network/build/STEP_10_XCASH_BUILD_STATIC.txt /var/www/html/build/ubuntu-18.04/STEP_10_XCASH_BUILD_STATIC.txt
docker stop ubuntu-18.04-static-x-cash-build

docker start ubuntu-19.10-static-x-cash-build
docker exec -e USER="root" -u root -t -i --privileged ubuntu-19.10-static-x-cash-build /bin/bash -c "$(curl -sSL https://raw.githubusercontent.com/X-CASH-official/XCASH_DPOPS/master/scripts/build/build_script_static.sh)"
docker cp ubuntu-19.10-static-x-cash-build:/root/x-network/build/STEP_10_XCASH_BUILD_STATIC.txt /var/www/html/build/ubuntu-19.10/STEP_10_XCASH_BUILD_STATIC.txt
docker stop ubuntu-19.10-static-x-cash-build
