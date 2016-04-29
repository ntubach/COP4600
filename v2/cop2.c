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
#include <asm/uaccess.h>          /// Required for the put user function
#define  DEVICE_NAME "copchar"    /// The device will appear at /dev/coppchar using this value
#define  CLASS_NAME  "cop"        /// The device class -- this is a character device driver
#define BUFFER_LENGTH 2048        /// The buffer length
#define SUCCESS 0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Niko Tubach\n\t\tBrandon Ashley\n\t\tLucas Rosa");
MODULE_DESCRIPTION("Task to create a Linux char. device driver set to the authors by their benevolent overlord.");
MODULE_VERSION("4.20");

static int   	Major;                  /// Stores the major device number, determined by the system
static char  	message[BUFFER_LENGTH] = {0}; 	/// Memory for the string that is passed from userspace
static char 	*msg_Ptr;
static short  	size_of_message;        /// Stores size of read string
static int    	Open = 0;             	/// Counts the number of times the device is opened

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
int init_module(void){
	printk(KERN_INFO "copChar: Initializing the copChar device\n");

	// Register character device with 0 for dynamic major number, store the returned number to major variable
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	// Less than 0 means failure
	if (Major<0){
		printk(KERN_ALERT "copChar failed to register a major number with %d\n", Major);
		return Major;
	}
	// Else, success, print the major number
	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");

	return SUCCESS;
}

/// The cleanup function, undoes init, called upon removing the driver
void cleanup_module(void){
	unregister_chrdev(Major, DEVICE_NAME);
	printk(KERN_INFO "copChar: Goodnight young driver\n");
}

/// Function called whenever the device is opened
static int dev_open(struct inode *inodep, struct file *filep){
	static int numberOpens = 0;
	if (Open)
		return -EBUSY;
	
	Open++;			//Increments open counter (file is open)
	try_module_get(THIS_MODULE);
	printk(KERN_INFO "copChar: Device has been opened %d time(s)\n", ++numberOpens);
	return SUCCESS;
}

/// Function called whenever the userspace program releases the device
static int dev_release(struct inode *inodep, struct file *filep){
	Open--;
	module_put(THIS_MODULE);
	printk(KERN_INFO "copChar: Device successfully closed\n");
	return 0;
}

/// Function called whenever device sends data back to the terminal (user)
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
	//Bytes written to buffer
	int bytes_read = 0;

	//End of message
	if (*msg_Ptr == 0)
		return 0;

	//Put data into buffer
	while (len && *msg_Ptr) {
		//use put user to copy over to user side
		put_user(*(msg_Ptr++), buffer++);

		len--;
		bytes_read++;
	}

	/* 
	 * Most read functions return the number of bytes put into the buffer
	 */
	return bytes_read;
}

/// Function called whenever the terminal (user) sends data to the device
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
	
	sprintf(message, "%s", buffer);   				// Writes the buffer store to message array
	msg_Ptr = message;
	size_of_message = strlen(message);          	// Store the length of the stored message
	printk(KERN_INFO "copChar: Received %d characters from the user\n", len);
	return len;
}
