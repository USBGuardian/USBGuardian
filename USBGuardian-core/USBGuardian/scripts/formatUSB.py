#!/usr/bin/python3.5
#-*- coding:utf-8 -*-

import os

#Unmount the USB stick
os.system("sudo umount /mnt/usb")

#Open the report file
with open("/opt/USBGuardian/logs/report.log") as report:

	for line in report:

		#If the USB stick is not partitioned, create a partition and format it
		if "Partitioned: no" in line:
			os.system("echo ',,7;' | sfdisk /dev/sd[a-z]")
			os.system("mkfs.vfat -I /dev/sd[a-z][0-9]")
			break

		#If the USB stick is partitioned, format the  USB stick depending  on the format
		elif "FAT16" in line:
			os.system("sudo mkfs.fat -F 16 -I /dev/sd[a-z][0-9]")
			break

		elif "FAT32" in line:
			os.system("sudo mkfs.fat -F 32 -I /dev/sd[a-z][0-9]")
			break

		elif "VFAT" in line:
			os.system("sudo mkfs.vfat -I /dev/sd[a-z][0-9]")
			break
