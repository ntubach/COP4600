# COP4600 Character Device Driver

##Basic Info
Character device driver made for Ubuntu v14.04.4 over Oracle VM VirtualBox.

Uses basic init and clean as well as open, close, read and write for device.

##HowTo       
	-Place cop.c, testcop.c and Makefile somewhere on your device (preferably in the same directory)       
	-From your terminal (bash or otherwise) change your directory to the one containing the above       
	-Run 'make'       
	-Insert the module by using 'insmod cop.ko'       
	-Test the device through the supporting file using './test'       
	-Follow the onscreen instructions (write a string, press enter, press enter some more, see the same string given back)       
	-Remove the module by using 'rmmod cop'        
	-View step by step events that have been posted to the system log at any time using 'tail -f /var/log/syslog'        
		(alternatively 'sudo tail -f /var/log/kern.log')      
	-Congratualations, you have initialized, run, tested and removed a character device driver successfully!      
	:boom:
##References:   
[Writing a Linux Kernel Module](http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/)   
[The Linux Kernel Module Programming Guide](http://www.tldp.org/LDP/lkmpg/2.6/html/index.html)

##Acknowledgements:      
[Brandon Ashley](https://github.com/teebash)    
[Lucas Rosa]()

###TODO     
-Limit read-in amount by buffer allotted size (1024), currently allows for any size string (very large) to be read in     
-Guarentee buffer clear after printing to terminal
