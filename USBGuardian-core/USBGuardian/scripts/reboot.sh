#!/bin/bash

#Empty the log files
sudo truncate -s 0 /opt/USBGuardian/logs/report.log
sudo truncate -s 0 /opt/USBGuardian/logs/lastAnalysis.log
# Reboot the Raspberry
sudo reboot
