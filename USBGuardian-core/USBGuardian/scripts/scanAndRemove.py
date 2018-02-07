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

#Empty the log file
os.system("sudo truncate -s 0 /opt/USBGuardian/logs/lastAnalysis.log")

#Scan the USB device
os.system("sudo clamscan -r --remove --verbose /mnt/usb >> /opt/USBGuardian/logs/lastAnalysis.log")

#Get the log
with open("/opt/USBGuardian/logs/lastAnalysis.log") as logFile:

	linesLog = logFile.readlines()

	with open ("/opt/USBGuardian/logs/report.log",'a+') as report:
		#Copy the log summary at the end of the report
		os.system("sudo tail -n 10 /opt/USBGuardian/logs/lastAnalysis.log >> /opt/USBGuardian/logs/report.log")

		#Go through the log file
		for line in linesLog:

			#If the line is an infected file which is removed, copy it in the report
			if "Removed" in line:
				report.write("\n")
				report.write(line)

		report.write("End of analysis")

#Copy the report at the end of the history file
with open("/opt/USBGuardian/logs/report.log") as report2, open ("/opt/USBGuardian/logs/history.log",'a+') as history:

	#Separate from other analysis
	history.write("\n\n")
	history.write("#############################################\n\n")

	#Copy the content
	reportLines = report2.readlines()
	history.writelines(reportLines)
