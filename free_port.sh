sudo iptables -I INPUT -p tcp --dport 5434 -j 5001
sudo iptables -nL |grep 5434

sudo ufw allow from 127.0.0.1 proto tcp to any port 5001
sudo ufw allow from 127.0.0.1 proto tcp to any port 5434


sudo netstat -lpn |grep :5001
kill -9 5001
