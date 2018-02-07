#!/usr/bin/python3.5
#-*- coding:utf-8 -*-

import os
import sys
import re
from statistics import fileCount
from statistics import malwareCount
from statistics import infectedDevicesCount
from statistics import deviceCount
from statistics import totalTimeOfScan

#Check if autorun file exist on usb device
if os.path.isfile("/mnt/usb/Autorun.inf"):
	os.system("sudo mv /mnt/usb/Autorun.inf /mnt/usb/Autorun.inf.MALICIOUS")

#Empty the log file
os.system("sudo truncate -s 0 /opt/USBGuardian/logs/lastAnalysis.log")

#Scan the USB device
os.system("clamscan -r --verbose /mnt/usb >> /opt/USBGuardian/logs/lastAnalysis.log ")

#Update device count statistics
deviceCount()

#Get the log
with open("/opt/USBGuardian/logs/lastAnalysis.log") as logFile:

	#Copy the log summary at the end of the report
	os.system("sudo tail -n 10 /opt/USBGuardian/logs/lastAnalysis.log >> /opt/USBGuardian/logs/report.log")

	linesLog = logFile.readlines()
	with open ("/opt/USBGuardian/logs/report.log",'a+') as report:

		#Go through the log file
		for line in linesLog:

			#If the line is an infected file, copy it in the report
			if "FOUND" in line:
				report.write(line)

			#Update scanned files count statistics
			elif re.match("Scanned files:",line):
				sentence,count = line.split(": ")
				fileCount(int(count))

			#Update infected files and infected devices count statistics
			elif re.match("Infected files:",line):
				sentence,count = line.split(": ")
				if int(count)>0:
					malwareCount(int(count))
					infectedDevicesCount()

			#Update total time of scan count statistics
			elif re.match("Time:",line):
				sentence,value = line.split(": ")
				time,postString =value.split(" sec")
				totalTimeOfScan( time )

		report.write("End of analysis")

#Copy the report at the end of the history file
with open("/opt/USBGuardian/logs/report.log") as report2, open ("/opt/USBGuardian/logs/history.log",'a+') as history:

	#Separate from other analysis
	history.write("\n\n")
	history.write("#############################################\n\n")

	#Copy the content
	reportLines = report2.readlines()
	history.writelines(reportLines)
