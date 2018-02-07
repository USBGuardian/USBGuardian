#!/usr/bin/python3.5
#-*- coding: utf-8 -*-

import os


#Open the file in which format info is stored
checkFormat = open('/opt/USBGuardian/scripts/checkFormat',"r")
formatOK=0

#Read the file to search the format
for lignes in checkFormat:

	if "vfat" in lignes:
		format="VFAT"

	elif "fat32" in lignes:
		format="FAT32"

	elif "fat16" in lignes:
		format = "FAT16"

	else:
		format="Unsupported format"
		formatOK=1

#Write the format in the report
with open('/opt/USBGuardian/logs/report.log',"a") as report:
	report.write("USB stick format: " + format+"\n")

#Delete the checkFormat file
os.system("sudo rm /opt/USBGuardian/scripts/checkFormat")

#Execute the scan
if formatOK==0:
	os.system("sudo /opt/USBGuardian/scripts/scan.py")
