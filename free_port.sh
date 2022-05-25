sudo iptables -I INPUT -p tcp --dport 5434 -j ACCEPT
sudo iptables -nL |grep 5434

sudo ufw allow from 127.0.0.1 proto tcp to any port 1433
sudo ufw allow from 127.0.0.1 proto tcp to any port 5434
