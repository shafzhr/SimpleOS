sudo tunctl -t tap0 -u `whoami`
sudo tunctl -t tap1 -u `whoami`

sudo ifconfig tap0 up
sudo ifconfig tap1 up