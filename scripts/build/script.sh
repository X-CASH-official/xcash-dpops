#!/bin/bash

rm -r /var/www/html/build/
mkdir /var/www/html/build/

docker start ubuntu-18.04
docker exec -e USER="root" -u root -t -i --privileged ubuntu-18.04 /root/script.sh
docker cp ubuntu-18.04:/root/x-network/build/ /var/www/html/build/ubuntu-18.04
docker stop ubuntu-18.04

docker start ubuntu-19.10
docker exec -e USER="root" -u root -t -i --privileged ubuntu-19.10 /root/script.sh
docker cp ubuntu-19.10:/root/x-network/build/ /var/www/html/build/ubuntu-19.10
docker stop ubuntu-19.10
