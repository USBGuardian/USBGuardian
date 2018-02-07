#!/usr/bin/python3.5
#-*- coding:utf-8 -*-

import os
import re

def deviceCount():

	#Open statistics file
	with open("/opt/USBGuardian/statistics.txt","r") as statisticsFile:
		lines = statisticsFile.readlines()

		for number,line in enumerate(lines):
			 #Increase the number of devices scanned by one
			if re.match("Number of devices scanned:",line):
				sentence,count = line.split(": ")
				newCount = int(count) + 1
				lines[number]=sentence + ": " +  str(newCount) + "\n"

	#Write the new value in the statistics file
	with open("/opt/USBGuardian/statistics.txt","w") as statisticsFile:
		statisticsFile.writelines(lines)

	pourcentageInfectedDevices()
	averageTimeOfScan()

def malwareCount( nbMalwares ):

        #Open statistics file
	with open("/opt/USBGuardian/statistics.txt","r") as statisticsFile:
		lines = statisticsFile.readlines()

		for number,line in enumerate(lines):
                         #Increase the number of malwares found  by the argument value
			if re.match("Number of malwares detected:",line):
				sentence,count = line.split(": ")
				newCount = int(count) + nbMalwares
				lines[number]=sentence + ": " +  str(newCount) + "\n"

	#Write the new value in the statistics file
	with open("/opt/USBGuardian/statistics.txt","w") as statisticsFile:
		statisticsFile.writelines(lines)

def fileCount( nbFiles ):

	#Open statistics file
	with open("/opt/USBGuardian/statistics.txt","r") as statisticsFile:
		lines = statisticsFile.readlines()
		for number,line in enumerate(lines):
			#Increase the number of malwares found  by the argument value
			if re.match("Number of files scanned:",line):
				sentence,count = line.split(": ")
				newCount = int(count) + nbFiles
				lines[number]=sentence + ": " +  str(newCount) + "\n"

	#Write the new value in the statistics file
	with open("/opt/USBGuardian/statistics.txt","w") as statisticsFile:
		statisticsFile.writelines(lines)


def pourcentageInfectedDevices():

	with open("/opt/USBGuardian/statistics.txt","r") as statisticsFile:
		lines = statisticsFile.readlines()
		for number,line in enumerate(lines):
			if re.match("Number of devices scanned:",line):
				sentence,countDevices = line.split(": ")
			elif re.match("Number of infected devices:",line):
				sentence,countInfected = line.split(": ")
			elif re.match("% of infected devices:",line):
				avgSentence,count = line.split(": ")
				lineNumber=number

		avgCount = (int(countInfected)/int(countDevices))*100
		lines[lineNumber]=avgSentence + ": " +  str(int(avgCount)) + "%" + "\n"

	#Write the new value in the statistics file
	with open("/opt/USBGuardian/statistics.txt","w") as statisticsFile:
		statisticsFile.writelines(lines)

def averageTimeOfScan():

	with open("/opt/USBGuardian/statistics.txt","r") as statisticsFile:
		lines = statisticsFile.readlines()
		for number,line in enumerate(lines):
			if re.match("Number of devices scanned:",line):
				sentence,countDevices = line.split(": ")
			elif re.match("Total time of scan:",line):
				sentence,totalTime = line.split(": ")
			elif re.match("Average time of scan:",line):
				avgSentence,count = line.split(": ")
				lineNumber=number
		avgCountMin = ((float(totalTime)/int(countDevices))//60)
		avgCountSec = ((float(totalTime)/int(countDevices))%60)
		lines[lineNumber]=avgSentence + ": " +  str(int(avgCountMin)) + " min " + str(int(avgCountSec)) + " sec" + "\n"

        #Write the new value in the statistics file
	with open("/opt/USBGuardian/statistics.txt","w") as statisticsFile:
		statisticsFile.writelines(lines)

def infectedDevicesCount():

	#Open statistics file
	with open("/opt/USBGuardian/statistics.txt","r") as statisticsFile:
		lines = statisticsFile.readlines()
		for number,line in enumerate(lines):

                        #Increase the number of infected devices  by one
			if re.match("Number of infected devices:",line):
				sentence,count = line.split(": ")
				newCount = int(count) + 1
				lines[number]=sentence + ": " +  str(newCount) + "\n"

        #Write the new value in the statistics file
	with open("/opt/USBGuardian/statistics.txt","w") as statisticsFile:
		statisticsFile.writelines(lines)

	pourcentageInfectedDevices()


def totalTimeOfScan( time ):

        #Open statistics file
	with open("/opt/USBGuardian/statistics.txt","r") as statisticsFile:
		lines = statisticsFile.readlines()
		for number,line in enumerate(lines):

                        #Increase the total time of scan  by one
			if re.match("Total time of scan:",line):
				sentence,count = line.split(": ")
				newCount = float(count) + float(time)
				lines[number]=sentence + ": " +  str(newCount) + "\n"

        #Write the new value in the statistics file
	with open("/opt/USBGuardian/statistics.txt","w") as statisticsFile:
		statisticsFile.writelines(lines)

	averageTimeOfScan()

def rebootStatistics():

        #Open the original statistics file
	with open("/opt/USBGuardian/emptyStatistics.txt","r") as statisticsFile:
		lines = statisticsFile.readlines()

	#Copy the content of the original file to the statistics file
	with open("/opt/USBGuardian/statistics.txt","w") as statisticsFile:
		statisticsFile.writelines(lines)

	#averageTimeOfScan()
