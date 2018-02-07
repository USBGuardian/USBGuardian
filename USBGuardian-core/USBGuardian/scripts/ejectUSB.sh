#!/bin/bash

#Unmount the USB stick
sudo umount /mnt/usb

#Kill any clamscan running (if USB stick is removed before the end of analysis
sudo killall clamscan

#Empty the report of the current USB stick
sudo truncate -0 /opt/USBGuardian/logs/report.log
