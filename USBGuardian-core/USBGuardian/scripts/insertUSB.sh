#!/bin/bash

#Unmount usb stick in case there is an old one
sudo umount /mnt/usb

#Change directory
cd /opt/USBGuardian/logs

#Create the report file and write the date and the time in it
sudo truncate -s 0 report.log
sudo printf "Report created:  $(date)\n" >> ./report.log

#Check if the USB stick is partitioned
partitioned=1

while read x
do
	if [[ "$x" =~ ^sd[a-z][0-9] ]]; then
		partitioned="0"
	fi;
done << EOF
$(ls /dev)
EOF

#Mount the USB device depending on the number of partitions
if [ "$partitioned" = "0" ]; then
	sudo mount /dev/sd[a-z][0-9] /mnt/usb
	sudo printf "Partitioned: yes\n" >> ./report.log
else
	sudo mount /dev/sd[a-z] /mnt/usb
	sudo printf "Partitioned: no\n" >> ./report.log
fi;

#Create a file to store format info
sudo touch /opt/USBGuardian/scripts/checkFormat
sudo chmod +r+w /opt/USBGuardian/scripts/checkFormat


#Store format info about the key
sudo mount | grep /mnt/usb > /opt/USBGuardian/scripts/checkFormat

#Launch python script 
sudo python3 /opt/USBGuardian/scripts/checkFormat.py

#If the format is supported start the scan
#if [ "$partitioned" = "0" ]; then
#	sudo python3 /opt/USBGuardian/scripts/scan.py
#fi
