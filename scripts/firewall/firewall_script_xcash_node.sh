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
iptables -A INPUT -s 10.12.242.0/24 -j ACCEPT
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
 
# Accept XCASH
iptables -A INPUT -p tcp --dport 18280 -j ACCEPT
iptables -A INPUT -p tcp --dport 18281 -j ACCEPT
 
# Allow ssh (allow 100 login attempts in 1 hour from the same ip, if more than ban them for 1 hour)
iptables -A INPUT -p tcp -m tcp --dport ${SSH_PORT_NUMBER} -m state --state NEW -m recent --set --name DEFAULT --rsource
iptables -A INPUT -p tcp -m tcp --dport ${SSH_PORT_NUMBER} -m state --state NEW -m recent --update --seconds 3600 --hitcount 100 --name DEFAULT --rsource -j DROP
iptables -A INPUT -p tcp -m tcp --dport ${SSH_PORT_NUMBER} -j ACCEPT
 
# DROP all INPUT and FORWARD packets if they have reached this point
iptables -A INPUT -j DROP
iptables -A FORWARD -j DROP
