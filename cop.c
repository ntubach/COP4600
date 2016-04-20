/**
 * By: 	Niko Tubach
 * 		Brandon Ashley
 *		Lucas Rosa 
 *	COP4600 - Assignment 5 
 **/

#include <linux/init.h>           /// Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         /// Core header for loading LKMs into the kernel
#include <linux/device.h>         /// Header to support the kernel Driver Model
#include <linux/kernel.h>         /// Contains types, macros, functions for the kernel
#include <linux/fs.h>             /// Header for the Linux file system support
#include <asm/uaccess.h>          /// Required for the copy to user function
#define  DEVICE_NAME "copchar"    /// The device will appear at /dev/ebbchar using this value
#define  CLASS_NAME  "cop"        /// The device class -- this is a character device driver
//#define BUFFER_LENGTH 1024        /// The buffer length

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Niko Tubach\n\t\tBrandon Ashley\n\t\tLucas Rosa");
MODULE_DESCRIPTION("Task to create a Linux char. device driver set to the authors by their benevolent overlord.");
MODULE_VERSION("4.20");

static int    Major;                  		/// Stores the major device number, determined by the system
static char   message[1024] = {0}; /// Memory for the string that is passed from userspace
static short  size_of_message;              /// Stores size of read string
static int    numberOpens = 0;              /// Counts the number of times the device is opened
static struct class*  copClass  = NULL; 	/// The device-driver class struct pointer
static struct device* copDevice = NULL; 	/// The device-driver device struct pointer

/// Prototype functions of character driver
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/// Lists the callback functions from fs.h you wish to use, we need open, read, write and release
static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,   
};

/// This function runs once upon initialization of driver, returns 0 upon success
static int __init ebbchar_init(void){
   printk(KERN_INFO "copChar: Initializing the copChar device\n");

   // Register character device with 0 for dynamic major number, store the returned number to major variable
   Major = register_chrdev(0, DEVICE_NAME, &fops);
   // Less than 0 means failure
   if (Major<0){
      printk(KERN_ALERT "copChar failed to register a major number\n");
      return Major;
   }
   // Else, success, print the major number
   printk(KERN_INFO "copChar: registered successfully...major number = %d\n", Major);

   // Register the device class, save result to class variable
   copClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(copClass)){                			// Check for error and clean up if there is
      unregister_chrdev(Major, DEVICE_NAME); 		//Un-register the device by major number
      printk(KERN_ALERT "Failed to register device class\n");	//Send error message to user
      return PTR_ERR(copClass);          			// The "proper" way to return error pointer
   }
   // Else, successful class registration
   printk(KERN_INFO "copChar: device class registered correctly\n");

   // Register the device driver
   copDevice = device_create(copClass, NULL, MKDEV(Major, 0), NULL, DEVICE_NAME);
   if (IS_ERR(copDevice)){               // Clean up if there is an error
      class_destroy(copClass);           // Same as above, alternatively goto statements
      unregister_chrdev(Major, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(copDevice);
   }
   // Else device driver created successfully
   printk(KERN_INFO "copChar: device class created correctly\n"); // Made it! device was initialized
   return 0;
}

/// The cleanup function, undoes init, called upon removing the driver
static void __exit ebbchar_exit(void){
   device_destroy(copClass, MKDEV(Major, 0));     		// remove the device
   class_unregister(copClass);                          // unregister the device class
   class_destroy(copClass);                             // remove the device class
   unregister_chrdev(Major, DEVICE_NAME);             	// unregister the major number
   printk(KERN_INFO "copChar: Goodnight young driver\n");
}

/// Function called whenever the device is opened
static int dev_open(struct inode *inodep, struct file *filep){
   numberOpens++;	//Increments open counter 
   printk(KERN_INFO "copChar: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}

/// Function called whenever device sends data back to the terminal (user)
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
	int error_count = 0;
	// copy_to_user has the format ( * to, *from, size) and returns 0 on success
	error_count = copy_to_user(buffer, message, size_of_message);
	//Clear the buffer after sending bytes to terminal
		
	if (error_count==0){            // If true then copy was successful
		printk(KERN_INFO "copChar: Sent %d characters to the user\n", size_of_message);
		return (size_of_message=0);  // clear the position to the start and return 0
	}
	else {
		printk(KERN_INFO "copChar: Failed to send %d characters to the user\n", error_count);
		return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
	}
}

/// Function called whenever the terminal (user) sends data to the device
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
	
	sprintf(message, "%s", buffer);   				// Write message to the buffer
	size_of_message = strlen(message);          	// Store the length of the stored message
	printk(KERN_INFO "copChar: Received %d characters from the user\n", len);
	return len;
}

/// Function called whenever the userspace program releases the device
static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "copChar: Device successfully closed\n");
   return 0;
}

/// Defines the init and cleanup functions for use via init.h
module_init(ebbchar_init);
module_exit(ebbchar_exit);
