# COP4600 Character Device Driver

## Basic Info
Character device driver made for Ubuntu v14.04.4 over Oracle VM VirtualBox.

Uses basic init and clean as well as open, close, read and write for device.

## HowTo

* clone the repo: `git clone https://github.com/ntubach/cop4600.git`
* `cd cop4600`
* Run `make`
* Insert the module by using `sudo insmod cop.ko`     
* View the driver init info for the major number using `sudo tail -f /var/log/syslog`
* Use mknod for node setup 'mknod /dev/copchar c 247 x' where x is you guessing the minor number (start with 1);
* Test the device through the supporting file using `sudo ./test`
* Follow the onscreen instructions (write a string, press enter, press enter some more, see the same string given back)
* Remove the module by using `sudo rmmod cop`
* View step by step events that have been posted to the system log at any time using `tail -f /var/log/syslog`
	* (alternatively `sudo tail -f /var/log/kern.log`)
* Congratualations, you have initialized, run, tested and removed a character device driver successfully!

:point_right:  :boom::boom::sunglasses::boom::boom:  :point_left:

## References:

[Writing a Linux Kernel Module](http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/)   
[The Linux Kernel Module Programming Guide](http://www.tldp.org/LDP/lkmpg/2.6/html/index.html)

## Acknowledgements:

[Brandon Ashley](https://github.com/teebash)    
[Lucas Rosa](https://github.com/lrosa007)

### TODO
-Limit read-in amount by buffer allotted size (1024), currently allows for any size string (very large) to be read in     
-Guarentee buffer clear after printing to terminal
