
# How to use
Please download the following user manul explaining how to use the station:

https://usbguardian.wordpress.com/documentation/

# Where to download
## Pre-build image
You can download the prebuild image here:

https://usbguardian.wordpress.com/documentation/

While copying the pre-built image, make sure the destination disk is the SD card and not your disk.

* On Linux:
Find the raw disk identifying your SD card (something like /dev/sdX1 where X is a character):
```
df -h
```
The disk is most probably mounted and needs to be unmounted before the copy:
```
umount /dev/sdX1
```
Now you can copy the image to the SD card using dd:
```
dd bs=1M if=2018-02-07_USBGuardian.img of=/dev/sdX
```
* On Windows:
As recommended by the RaspberryPi, use the Win32DiskImager utility to copy the file image to the SD card. You can also use dd for Windows.

Now, you’re ready to go! Just insert your SD card into the Raspberry Pi and set it on!

# How it works
We also created a document explaining how to recreate this software step by step:
https://usbguardian.wordpress.com/documentation/
