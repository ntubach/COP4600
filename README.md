# COP4600 Character Device Driver

##Basic Info
Character device driver made for Ubuntu v14.04.4 over Oracle VM VirtualBox.

Uses basic init and clean as well as open, close, read and write for device.

##References:   
[Writing a Linux Kernel Module](http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/)   
[The Linux Kernel Module Programming Guide](http://www.tldp.org/LDP/lkmpg/2.6/html/index.html)

##Acknowledgements:      
[Brandon Ashley](https://github.com/teebash)    
[Lucas Rosa]()

###TODO     
-Limit read-in amount by buffer allotted size (1024), currently allows for any size string (very large) to be read in
-Guarentee buffer clear after printing to terminal